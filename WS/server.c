#include "server.h"

/** Flag to enable debugging */
#define DEBUG_SERVER 1

/** Array of games */
tGame games[MAX_GAMES];


void initServerStructures (){

	if (DEBUG_SERVER)
		printf ("Initializing...\n");

	// Init seed
	srand (time(NULL));

	// Init each game
	for (int i=0; i<MAX_GAMES; i++){

		// Allocate and init board
		games[i].board = (xsd__string) malloc (BOARD_WIDTH*BOARD_HEIGHT);
		initBoard (games[i].board);

		// Allocate and init player names
		games[i].player1Name = (xsd__string) malloc (STRING_LENGTH);
		memset (games[i].player1Name, 0, STRING_LENGTH);
		games[i].player2Name = (xsd__string) malloc (STRING_LENGTH);
		memset (games[i].player2Name, 0, STRING_LENGTH);
		games[i].player3Name = (xsd__string) malloc (STRING_LENGTH);
		memset (games[i].player3Name, 0, STRING_LENGTH);

		//TODO: Initialize the rest
		games[i].endOfGame = FALSE;
		games[i].status = gameEmpty;
		pthread_mutex_init(&games[i].mutexGame,NULL);
		pthread_mutex_init(&games[i].mutexPlayers,NULL);
		pthread_cond_init(&games[i].cond,NULL);
	}
}

void freeGame(int indexGame){
	games[indexGame].board = (xsd__string) malloc (BOARD_WIDTH*BOARD_HEIGHT);
	initBoard (games[indexGame].board);

	// Allocate and init player names
	games[indexGame].player1Name = (xsd__string) malloc (STRING_LENGTH);
	memset (games[indexGame].player1Name, 0, STRING_LENGTH);
	games[indexGame].player2Name = (xsd__string) malloc (STRING_LENGTH);
	memset (games[indexGame].player2Name, 0, STRING_LENGTH);
	games[indexGame].player3Name = (xsd__string) malloc (STRING_LENGTH);
	memset (games[indexGame].player3Name, 0, STRING_LENGTH);
	
	games[indexGame].status = gameEmpty;
	games[indexGame].endOfGame = FALSE;
	/* 
		pthread_mutex_init(&games[i].mutexGame,NULL);
		pthread_mutex_init(&games[i].mutexPlayers,NULL);
		pthread_cond_init(&games[i].cond,NULL);
	 */
	printf("game[%d] is free\n",indexGame);
}

conecta3x3ns__tPlayer calculateNextPlayer (conecta3x3ns__tPlayer currentPlayer){

	conecta3x3ns__tPlayer nextPlayer;

		if (currentPlayer == player1)
			nextPlayer = player2;
		else if (currentPlayer == player2)
			nextPlayer = player3;
		else
			nextPlayer = player1;

	return nextPlayer;
}

conecta3x3ns__tPlayer calculateRandomPlayer (){

	//TODO
	srand (time(NULL));

	return rand()% MAX_PLAYERS;
}

int searchEmptyGame (){

	//TODO
	int pos = 0; 
	int free = FALSE;
	while(pos<MAX_GAMES && !free){

		pthread_mutex_lock(&games[pos].mutexPlayers);
		if(games[pos].status == gameEmpty){
			free = TRUE;
		}
		else if(games[pos].status == gameWaitingPlayer){
			free = TRUE;
		}
		else{
			pthread_mutex_unlock(&games[pos].mutexPlayers);
		}
		pos++;
	}
	return free?pos-1:ERROR_SERVER_FULL;
}

int locateGameForPlayer (xsd__string name){

	//TODO

	// HINT: use (strcmp (games[i].playerXName, name) == 0)) to compare the player's name ;)
	int pos = 0;
	int located = FALSE;
	while(pos<MAX_GAMES && !located){
		if(games[pos].activePlayers>0){
			if( strcmp (games[pos].player1Name, name) == 0){
				located = TRUE;
			}else if(games[pos].activePlayers>1){
				if( strcmp (games[pos].player2Name, name) == 0){
					located = TRUE;
				} else if(games[pos].activePlayers>2){
					if( strcmp (games[pos].player3Name, name) == 0){
						located = TRUE;
					}
				}
			}
		}
		pos++;
	}

	return located?pos-1:ERROR_PLAYER_NOT_FOUND;
}

void copyGameStatusStructure (conecta3x3ns__tBlock* status, char* message, char* board, int newCode){
	// Copy the message
	strcpy ((status->msgStruct).msg, message);
	(status->msgStruct).__size = strlen ((status->msgStruct).msg);
	// Copy the board, only if it is not NULL
	if (board == NULL){
		status->board = NULL;
		status->__size = 0;
	}
	else{
		strncpy (status->board, board, BOARD_WIDTH*BOARD_HEIGHT);
		status->__size = BOARD_WIDTH*BOARD_HEIGHT;
	}

	// Set the new code
	status->code = newCode;
}

int conecta3x3ns__register (struct soap *soap, conecta3x3ns__tMessage playerName, int* code){

	int gameIndex;
	char messageToPlayer [STRING_LENGTH];

		// Init...
		gameIndex = -1;

		// Set \0 at the end of the string
		playerName.msg[playerName.__size] = 0;

		if (DEBUG_SERVER)
			printf ("[Register] new player -> [%s]\n", playerName.msg);

		// Search fon an empty game

		gameIndex = searchEmptyGame(); 

		// Is server full?
		if (gameIndex == ERROR_SERVER_FULL){
			printf ("Server is full! Player [%s] cannot be registered\n", playerName.msg);
			*code = ERROR_SERVER_FULL;
		}

		// Check if this player is already registered
		else if (locateGameForPlayer(playerName.msg) != ERROR_PLAYER_NOT_FOUND){
			printf ("Player already registered! Repeated name [%s]\n", playerName.msg);
			*code = ERROR_NAME_REPEATED;
		}

		// Player can be registered...
		else{

			// TODO
			*code = OK_NAME_REGISTERED;
			if(games[gameIndex].activePlayers == 0){
				strcpy(games[gameIndex].player1Name, playerName.msg);
				games[gameIndex].currentPlayer = calculateRandomPlayer();
				games[gameIndex].status = gameWaitingPlayer;
			} else if(games[gameIndex].activePlayers == 1){
				strcpy(games[gameIndex].player2Name, playerName.msg);
			} else if(games[gameIndex].activePlayers == 2){
				strcpy(games[gameIndex].player3Name, playerName.msg);
				games[gameIndex].status = gameReady;
			}
				games[gameIndex].activePlayers++;

			pthread_mutex_unlock(&games[gameIndex].mutexPlayers);
		}


  return SOAP_OK;
}

int conecta3x3ns__getStatus (struct soap *soap, conecta3x3ns__tMessage playerName, conecta3x3ns__tBlock* status){

	int gameIndex, isWinner;
	char messageToPlayer [STRING_LENGTH];

		// Set \0 at the end of the string
		playerName.msg[playerName.__size] = 0;
		isWinner = FALSE;
		memset(messageToPlayer,0,STRING_LENGTH);

		// Allocate memory for the status structure
		(status->msgStruct).msg = (xsd__string) malloc (STRING_LENGTH);
		memset ((status->msgStruct).msg, 0, STRING_LENGTH);
		status->board = (xsd__string) malloc (BOARD_WIDTH*BOARD_HEIGHT);
		memset (status->board, 0, BOARD_WIDTH*BOARD_HEIGHT);
		status->__size = BOARD_WIDTH*BOARD_HEIGHT;

		if (DEBUG_SERVER)
			printf ("[getStatus] request from -> [%s]\n", playerName.msg);

		// Locate the game
		gameIndex = locateGameForPlayer (playerName.msg);
		// Player not found...
		if (gameIndex == ERROR_PLAYER_NOT_FOUND){

			// Allocate memory and copy message into status struct
			strcpy(messageToPlayer,"Player is not currently active in the system! Please, register again...\0");
			copyGameStatusStructure (status, messageToPlayer, NULL, ERROR_PLAYER_NOT_FOUND);
			printf ("[getStatus] - Player %s not found!\n", playerName.msg);
		}

		// Player is already registered...
		else{
			//TODO
			pthread_mutex_lock(&games[gameIndex].mutexGame);
			if(games[gameIndex].endOfGame){
				if(isBoardFull(games[gameIndex].board)){
					strcpy(messageToPlayer,"Game Over, board is full\0");
					copyGameStatusStructure(status,messageToPlayer,games[gameIndex].board, GAMEOVER_WIN);

				}
				else{
					if(games[gameIndex].currentPlayer == player1 && strcmp(games[gameIndex].player1Name,playerName.msg)==0){
						strcpy(messageToPlayer,"Game Over. You win\0");
						copyGameStatusStructure(status,messageToPlayer,games[gameIndex].board, GAMEOVER_WIN);
					}
					else if (games[gameIndex].currentPlayer == player2 && strcmp(games[gameIndex].player2Name,playerName.msg)==0){
						strcpy(messageToPlayer,"Game Over. You win\0");
						copyGameStatusStructure(status,messageToPlayer,games[gameIndex].board, GAMEOVER_WIN);
					}
					else if	(games[gameIndex].currentPlayer == player3 && strcmp(games[gameIndex].player3Name,playerName.msg)==0){
						strcpy(messageToPlayer,"Game Over. You win\0");
						copyGameStatusStructure(status,messageToPlayer,games[gameIndex].board, GAMEOVER_WIN);
					}
					else{
						strcpy(messageToPlayer,"Game Over\0");
						copyGameStatusStructure(status,messageToPlayer,games[gameIndex].board, GAMEOVER_LOSE);	
					}
				}
				if(--games[gameIndex].activePlayers == 0){
					freeGame(gameIndex);
				}
				pthread_mutex_unlock(&games[gameIndex].mutexGame);
			}
			else{
				int final = FALSE;
				strcpy(messageToPlayer,"Its your turn\0");

				while((games[gameIndex].currentPlayer != player1 && strcmp(games[gameIndex].player1Name,playerName.msg)==0)||
					(games[gameIndex].currentPlayer != player2 && strcmp(games[gameIndex].player2Name,playerName.msg)==0)||
					(games[gameIndex].currentPlayer != player3 && strcmp(games[gameIndex].player3Name,playerName.msg)==0)){

					pthread_cond_wait(&games[gameIndex].cond,&games[gameIndex].mutexGame);
				}
				copyGameStatusStructure(status,messageToPlayer,games[gameIndex].board, TURN_MOVE);
			}
		}

	return SOAP_OK;
}

// TODO: Include a service to insert chips here!
int conecta3x3ns__insertChip (struct soap *soap, conecta3x3ns__tMessage playerName, int column, conecta3x3ns__tBlock* status){
	char messageToPlayer [STRING_LENGTH];
	memset(messageToPlayer,0,STRING_LENGTH);

	// Set \0 at the end of the string
	playerName.msg[playerName.__size] = 0;
	
	// Allocate memory for the status structure
	(status->msgStruct).msg = (xsd__string) malloc (STRING_LENGTH);
	memset ((status->msgStruct).msg, 0, STRING_LENGTH);
	status->board = (xsd__string) malloc (BOARD_WIDTH*BOARD_HEIGHT);
	memset (status->board, 0, BOARD_WIDTH*BOARD_HEIGHT);
	status->__size = BOARD_WIDTH*BOARD_HEIGHT;
	
	int pos = locateGameForPlayer(playerName.msg);

	conecta3x3ns__tMove move = checkMove(games[pos].board,column);

	if(move == OK_move){
		insertChip(games[pos].board,games[pos].currentPlayer,column);
		if(checkWinner(games[pos].board,games[pos].currentPlayer)){
			games[pos].endOfGame = TRUE;
			strcpy(messageToPlayer,"Game over. You win\0");
			copyGameStatusStructure(status,messageToPlayer,games[pos].board, GAMEOVER_WIN);
			--games[pos].activePlayers;
		}
		else if(isBoardFull(games[pos].board)){
			games[pos].endOfGame = TRUE;
			strcpy(messageToPlayer,"Game Over, board is full\0");
			copyGameStatusStructure(status,messageToPlayer,games[pos].board, GAMEOVER_DRAW);
			--games[pos].activePlayers;
		}
		else{
			strcpy(messageToPlayer,"Wait your turn...\0");
			games[pos].currentPlayer = calculateNextPlayer(games[pos].currentPlayer);
			copyGameStatusStructure(status,messageToPlayer,games[pos].board, TURN_WAIT);

		}
		pthread_mutex_unlock(&games[pos].mutexGame);
		pthread_cond_broadcast(&games[pos].cond);
	}
	else{
		pthread_mutex_unlock(&games[pos].mutexGame);
	}

	return SOAP_OK;
}

void *processRequest(void *soap){
	//TODO
	soap_serve(soap);
	soap_end(soap);
	soap_done(soap);
	
}

int main(int argc, char **argv){ 

	struct soap soap;
	struct soap *tsoap;
	int port;
	SOAP_SOCKET m, s;

	// Init soap environment
	soap_init(&soap);

	// Configure timeouts
	soap.send_timeout = 60; // 60 seconds
	soap.recv_timeout = 60; // 60 seconds
	soap.accept_timeout = 3600; // server stops after 1 hour of inactivity
	soap.max_keep_alive = 100; // max keep-alive sequence

	initServerStructures();

	// Get listening port
	port = atoi(argv[1]);

	// bind
	m = soap_bind(&soap, NULL, port, 100);

	if(!soap_valid_socket(m)){
		exit(1);
	}

	printf("Server is ON\n");

	while (TRUE){

		// Accept a new connection
		s = soap_accept(&soap);

		// Socket is not valid :(
		if (!soap_valid_socket(s)){

			if (soap.errnum){
				soap_print_fault(&soap, stderr);
				exit(1);
			}

			fprintf(stderr, "Time out!\n");
			break;
		}

		// Copy the SOAP environment
		tsoap = soap_copy(&soap);

		if (!tsoap){
			printf ("SOAP copy error!\n");
			break;
		}
		pthread_t tid;
		// Create a new thread to process the request
		pthread_create(&tid, NULL, (void*(*)(void*))processRequest, (void*)tsoap);
	}

	// Detach SOAP environment
	soap_done(&soap);


	return 0;
}
/*
int conecta3x3ns__register  	(conecta3x3ns__tMessage playerName, int* code){
	printf("New user\n");
	
	return SOAP_OK;
}

int conecta3x3ns__getStatus 	(conecta3x3ns__tMessage playerName, conecta3x3ns__tBlock* status){
	
	
	return SOAP_OK;
}
*/