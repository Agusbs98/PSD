#include "client.h"

#define DEBUG_CLIENT 1


unsigned int readMove (){

	xsd__string enteredMove;
	unsigned int move;
	unsigned int isRightMove;

		// Init...
		enteredMove = (xsd__string) malloc (STRING_LENGTH);
		memset (enteredMove, 0, STRING_LENGTH);
		isRightMove = FALSE;
		move = STRING_LENGTH;

		while (!isRightMove){

			printf ("Enter a move [0-6]:");

			// Read move
			fgets (enteredMove, STRING_LENGTH-1, stdin);

			// Remove new-line char
			enteredMove[strlen(enteredMove)-1] = 0;

			// Length of entered move is not correct
			if (strlen(enteredMove) != 1){
				printf ("Entered move is not correct. It must be a number between [0-6]\n");
			}

			// Check if entered move is a number
			else if (isdigit(enteredMove[0])){

				// Convert move to an int
				move =  enteredMove[0] - '0';

				if (move > 6)
					printf ("Entered move is not correct. It must be a number between [0-6]\n");
				else
					isRightMove = TRUE;
			}

			// Entered move is not a number
			else
				printf ("Entered move is not correct. It must be a number between [0-6]\n");
		}

	return move;
}


int main(int argc, char **argv){

	struct soap soap;					/** Soap struct */
	char *serverURL;					/** Server URL */
	unsigned int endOfGame = FALSE;		/** Flag to control the end of the game */
	conecta3x3ns__tMessage playerName;	/** Player name */
	conecta3x3ns__tBlock gameStatus;	/** Game status */
	unsigned int playerMove;			/** Player move */
	int resCode;						/** Return code from server */


	// Init gSOAP environment
	soap_init(&soap);

	// Obtain server address
	serverURL = argv[1];

	// Allocate memory for player name and init
	playerName.msg = (xsd__string) malloc (STRING_LENGTH);
	memset(playerName.msg, 0, STRING_LENGTH);

	// Allocate memory for game status and init
	gameStatus.msgStruct.msg = (xsd__string) malloc (STRING_LENGTH);
	memset(gameStatus.msgStruct.msg, 0, STRING_LENGTH);
	gameStatus.board = (xsd__string) malloc (BOARD_WIDTH*BOARD_HEIGHT);
	memset (gameStatus.board, 0, BOARD_WIDTH*BOARD_HEIGHT);
	gameStatus.__size = BOARD_WIDTH*BOARD_HEIGHT;

	// Init


	// Check arguments
	if (argc !=2) {
		printf("Usage: %s http://server:port\n",argv[0]);
		exit(0);
	}
	system("clear");

	// While player is not registered in the server side
	printf("Enter player name: ");
	memset(gameStatus.msgStruct.msg, 0, STRING_LENGTH);
	fgets(playerName.msg,STRING_LENGTH-1,stdin);
	playerName.__size = strlen(playerName.msg)-1;
	soap_call_conecta3x3ns__register(&soap,serverURL,"",playerName,&resCode);

	while(resCode != OK_NAME_REGISTERED){
		printf("Enter other player name: ");
		fgets(playerName.msg,STRING_LENGTH-1,stdin);
		playerName.__size = strlen(playerName.msg)-1;
		soap_call_conecta3x3ns__register(&soap,serverURL,"",playerName,&resCode);
	}
	printf("wait your turn...\n");

	// While game continues...
	while(!endOfGame){
		soap_call_conecta3x3ns__getStatus(&soap,serverURL,"",playerName,&gameStatus);
		system("clear");
		gameStatus.msgStruct.msg[gameStatus.msgStruct.__size] = 0;
		printBoard(gameStatus.board, gameStatus.msgStruct.msg);
		if(gameStatus.code == TURN_MOVE){
			int pos = readMove();

			soap_call_conecta3x3ns__insertChip(&soap,serverURL,"",playerName,pos,&gameStatus);
			system("clear");
			gameStatus.msgStruct.msg[gameStatus.msgStruct.__size] = 0;

			printBoard(gameStatus.board, gameStatus.msgStruct.msg);
		}
		if(gameStatus.code == GAMEOVER_WIN || gameStatus.code == GAMEOVER_LOSE || gameStatus.code == GAMEOVER_DRAW){
			endOfGame = TRUE;
		}

	}


	// Clear the environment
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);


  return 0;
}
