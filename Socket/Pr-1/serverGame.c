#include "serverGame.h"

void createSocket(int* socketfd, int port){
	struct sockaddr_in serverAddress;	/** Server address structure */

	// Create the socket
	*socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Check
	if (*socketfd < 0)
	showError("ERROR while opening socket");

	// Init server structure
	memset(&serverAddress, 0, sizeof(serverAddress));

	// Fill server structure
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);

	// Bind
	if (bind(*socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
		showError("ERROR while binding");

}

void conectSockets(tThreadArgs *players, int socketfd){
	struct sockaddr_in clientAddress;	/** Client address structure */
	unsigned int clientLength;			/** Length of client structure */
    tString message;					/** Message recv to the players */
    int msgLength;						/** Length of the message */
	// Get length of client structure
	clientLength = sizeof(clientAddress);

	// Accept!
	players->socketPlayer1 = accept(socketfd, (struct sockaddr *) &clientAddress, &clientLength);
	// Check accept result
	if (players->socketPlayer1 < 0)
		showError("ERROR while accepting");
	send(players->socketPlayer1,CONNECT_MESSAGE,strlen(CONNECT_MESSAGE),0);
	printf("player 1 is connected\n");
	players->socketPlayer2 = accept(socketfd, (struct sockaddr *) &clientAddress, &clientLength);
	// Check accept result
	if (players->socketPlayer2 < 0)
		showError("ERROR while accepting");	
	send(players->socketPlayer2,CONNECT_MESSAGE,sizeof(CONNECT_MESSAGE),0);
	printf("player 2 is connected\n");

	players->socketPlayer3 = accept(socketfd, (struct sockaddr *) &clientAddress, &clientLength);
	// Check accept result
	if (players->socketPlayer3 < 0)
		showError("ERROR while accepting");	
	send(players->socketPlayer3,CONNECT_MESSAGE,sizeof(CONNECT_MESSAGE),0);
	printf("player 3 is connected\n");
}

void getNames(tThreadArgs players,tPlayersNames *playersNames){
    int msgLength;						/** Length of the message */
	memset(&playersNames->player1, 0, STRING_LENGTH);
	msgLength = recv(players.socketPlayer1, &playersNames->player1, STRING_LENGTH-1, 0);
	printf("Name of player 1 received: %s\n",&playersNames->player1);
	memset(&playersNames->player2, 0, STRING_LENGTH);
	msgLength = recv(players.socketPlayer2, &playersNames->player2, STRING_LENGTH-1, 0);
	printf("Name of player 2 received: %s\n",&playersNames->player2);
	memset(&playersNames->player3, 0, STRING_LENGTH);
	msgLength = recv(players.socketPlayer3, &playersNames->player3, STRING_LENGTH-1, 0);
	printf("Name of player 3 received: %s\n",&playersNames->player3);
}

void move(tPlayer turn, tThreadArgs players, tBoard board){
	if(turn == player1){
		turnMove(players.socketPlayer1, board,PLAYER_1_CHIP);
		turnWait(players.socketPlayer2, board,PLAYER_2_CHIP);
		turnWait(players.socketPlayer3, board,PLAYER_3_CHIP);
	}
	else if(turn == player2){
		turnWait(players.socketPlayer1, board,PLAYER_1_CHIP);
		turnMove(players.socketPlayer2, board,PLAYER_2_CHIP);
		turnWait(players.socketPlayer3, board,PLAYER_3_CHIP);
	}
	else if(turn == player3){
		turnWait(players.socketPlayer1, board,PLAYER_1_CHIP);
		turnWait(players.socketPlayer2, board,PLAYER_2_CHIP);
		turnMove(players.socketPlayer3, board,PLAYER_3_CHIP);
	}
}

void turnMove(int playerSocket, tBoard board, char chip){
	tString msg;
	int code = TURN_MOVE;
	send(playerSocket, &code, sizeof(code),0);
	memset(msg,0,STRING_LENGTH);
	sprintf(msg,"%s%c",TURN_MOVE_MSG,chip);
	send(playerSocket, msg, STRING_LENGTH-1,0);
	send(playerSocket, board, BOARD_WIDTH * BOARD_HEIGHT,0);
}

void turnWait(int playerSocket, tBoard board, char chip){
	tString msg;
	int code = TURN_WAIT;
	send(playerSocket, &code, sizeof(code),0);
	memset(msg,0,STRING_LENGTH);
	sprintf(msg,"%s%c",TURN_WAIT_MSG,chip);
	send(playerSocket, msg, STRING_LENGTH-1,0);
	send(playerSocket, board, BOARD_WIDTH * BOARD_HEIGHT,0);
}

void recvMove(tBoard board, tPlayer turn, tThreadArgs players){
	unsigned int column;

	if(turn == player1){
		recv(players.socketPlayer1,&column, sizeof(unsigned int),0);
	}
	else if(turn == player2){
		recv(players.socketPlayer2,&column, sizeof(unsigned int),0);
	}
	else if(turn == player3){
		recv(players.socketPlayer3,&column, sizeof(unsigned int),0);
	}
	insertChip(board, turn, column);
}

int isFinal(tBoard board, tPlayer player, tThreadArgs players){
	if(checkWinner(board, player)){
		if(player == player1){
			sendWin(players.socketPlayer1,board);
			sendLose(players.socketPlayer2,board);
			sendLose(players.socketPlayer3,board);
		}
		else if(player == player2){
			sendLose(players.socketPlayer1,board);
			sendWin(players.socketPlayer2,board);
			sendLose(players.socketPlayer3,board);
		}
		else if(player == player3){
			sendLose(players.socketPlayer1,board);
			sendLose(players.socketPlayer2,board);
			sendWin(players.socketPlayer3,board);
		}		
		return TRUE;
	}
	else if(isBoardFull(board)){
		sendDraw(players.socketPlayer1,board);
		sendDraw(players.socketPlayer2,board);
		sendDraw(players.socketPlayer3,board);
	}	
	return FALSE;
}

void sendWin(int socketPlayer, tBoard board){
	tString msg;
	int code = GAMEOVER_WIN;
	send(socketPlayer,&code,sizeof(code),0);
	memset(msg,0,STRING_LENGTH);
	strcat(msg,"Winner");
	send(socketPlayer,msg,STRING_LENGTH-1,0);
	send(socketPlayer,board,BOARD_WIDTH * BOARD_HEIGHT,0);
}

void sendLose(int socketPlayer, tBoard board){
	tString msg;
	int code = GAMEOVER_LOSE;
	send(socketPlayer,&code,sizeof(code),0);
	memset(msg,0,STRING_LENGTH);
	strcat(msg,"Game over");
	send(socketPlayer,msg,STRING_LENGTH-1,0);
	send(socketPlayer,board,BOARD_WIDTH * BOARD_HEIGHT,0);
}

void sendDraw(int socketPlayer, tBoard board){
	tString msg;
	int code = GAMEOVER_DRAW;
	send(socketPlayer,&code,sizeof(code),0);
	memset(msg,0,STRING_LENGTH);
	strcat(msg,"Draw");
	send(socketPlayer,msg,STRING_LENGTH-1,0);
	send(socketPlayer,board,BOARD_WIDTH * BOARD_HEIGHT,0);
}

void readPoints(tListPlayerPoints *points){
	FILE *scores;
	int cont = 0;
	scores = fopen(SCORES_FILE_NAME,"r");
	while(cont < MAX_SCORES && !feof(scores)){
		fscanf(scores,"%s %d",&points->players[cont].name,&points->players[cont].points);
		cont++;
	}
	points->nPlayers = cont-1;
	fclose(scores);
}

void writePoints(tString player){
	tListPlayerPoints points;
	readPoints(&points);
	FILE *scores;
	scores = fopen(SCORES_FILE_NAME,"w");
	int cont = 0;
	int located = FALSE;
	while(cont < points.nPlayers && !feof(scores)){
		fputs(points.players[cont].name,scores);
		fputs(" ",scores);
		tString s;
		if(strcmp(points.players[cont].name,player) == 0){
			points.players[cont].points++;
			located = TRUE;
		}
		sprintf(s,"%d",points.players[cont].points);
		fputs(s,scores);
		fputs("\n",scores);
		cont++;
	}
	if(!located){
		fputs(player,scores);
		fputs(" 1\n",scores);
	}
	fclose(scores);
}

void game(void * param){
	tThreadArgs* players = (tThreadArgs*)param;				/** Socket players*/
	char message[STRING_LENGTH];		/** Message */
	tBoard board;						/** Board of the game */
	int gameOver = FALSE;				/** Condicion of game loop*/
	tPlayer turn = player1;				/** player to play*/
	tPlayersNames names;				/** players names */
	tListPlayerPoints points;			/** list names of players points */

	getNames(*players,&names);
	printf("Connection completed\n");
	
	initBoard(board);

	while(!gameOver){ // loop of game
		move(turn, *players, board);
		recvMove(board, turn, *players);
		if(isFinal(board, turn, *players)){
			gameOver = TRUE;
		}else{
			turn = ++turn % 3;
		}
	}
	int i = 0; 
	int located = FALSE;
	tString nameWin;
	if(turn == player1){
		strcpy(nameWin,names.player1);
	}
	else if(turn == player2){
		strcpy(nameWin,names.player2);
	}
	else if(turn == player3){
		strcpy(nameWin,names.player3);
	}
	// Close sockets
	close(players->socketPlayer1);
	close(players->socketPlayer2);
	close(players->socketPlayer3);
	pthread_mutex_lock(&mutex);

	readPoints(&points);
	tString topList;
	memset(topList,0,STRING_LENGTH);
	sprintf(topList,"%s %d\n%s %d\n%s %d\n",points.players[0].name,points.players[0].points,points.players[1].name,points.players[1].points,points.players[2].name,points.players[2].points);
	send(players->socketPlayer1,topList,STRING_LENGTH-1,0);
	send(players->socketPlayer2,topList,STRING_LENGTH-1,0);
	send(players->socketPlayer3,topList,STRING_LENGTH-1,0);

	//write new points
	if(checkWinner(board,turn)){
		writePoints(nameWin);
	}
	
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[]){
	int socketfd;						/** Socket descriptor */
	tThreadArgs* players;				/** Socket players*/
	
	// Check arguments
	if (argc < 2) {
		fprintf(stderr,"ERROR wrong number of arguments\n");
		fprintf(stderr,"Usage:\n&>%s port\n",argv[0]);
		exit(1);
	} 
	// Create the server socket
	createSocket(&socketfd, atoi(argv[1]));
	
	// Listen
	listen(socketfd, 10);

	// Connect to client
	while(1){
		pthread_t pthread;
		players = (tThreadArgs*)malloc(sizeof(struct threadArgs));
		conectSockets(players, socketfd);
		pthread_create(&pthread, NULL,game,(void*)players);
	}
		
	
	// Close socket server
	close(socketfd);
    return 0; 
}


