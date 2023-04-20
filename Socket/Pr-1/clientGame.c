#include "clientGame.h"

void connectServer(int *socketfd, int port, char* serverIP){
	struct sockaddr_in server_address;	/** Server address structure */
    tString message;					/** Message sent to the server side */
    int msgLength;						/** Length of the message */

	*socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Check if the socket has been successfully created
	if (*socketfd < 0)
		showError("ERROR while creating the socket");

	// Get the server address
	// Fill server address structure
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(serverIP);

	server_address.sin_port = htons(port);

	// Connect with server
	if (connect(*socketfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
		showError("ERROR while establishing connection");


	// Init and read message
	memset(message, 0, STRING_LENGTH);
	msgLength = recv(*socketfd, message, STRING_LENGTH-1, 0);

	// Check read bytes
	if (msgLength < 0)
		showError("ERROR while reading from socket");

	// Show message
	printf("%s\n", message);

	printf("Enter player name: ");
	memset(message, 0, STRING_LENGTH);
	fgets(message, STRING_LENGTH-1, stdin);

	// Send message to the server side
	msgLength = send(*socketfd, message, strlen(message)-1, 0);

	// Check the number of bytes sent
	if (msgLength < 0)
		showError("ERROR while writing to the socket");

}

unsigned int readMove (){

	int i, isValid, column;
	tString enteredMove;

		// Init...
		column = 0;

		// While player does not enter a correct move...
		do{

			// Init...
			bzero (enteredMove, STRING_LENGTH);
			isValid = TRUE;

			// Show input message
			printf ("Enter a move [0-%d]:", BOARD_WIDTH-1);

			// Read move
			fgets(enteredMove, STRING_LENGTH-1, stdin);

			// Remove new-line char
			enteredMove[strlen(enteredMove)-1] = 0;

			// Check if each character is a digit
			for (i=0; i<strlen(enteredMove) && isValid; i++){

				if (!isdigit(enteredMove[i]))
					isValid = FALSE;
			}

			// Entered move is not a number
			if (!isValid){
				printf ("Entered move is not correct. It must be a number between [0-%d]\n", BOARD_WIDTH-1);
			}

			// Entered move is a number...
			else{

				// Conver entered text to an int
				column = atoi (enteredMove);
			}
			
			isValid = isValid && (column<5) && (column >= 0);

		}while (!isValid);

	return ((unsigned int) column);
}

int main(int argc, char *argv[]){
    int socketfd;						/** Socket descriptor */
    tString message;					/** Message sent to the server side */
	int code = TURN_MOVE;				/** Code */
	tBoard board;						/** Board of the game */
	int gameOver = FALSE;
	//tString board;
	unsigned int column;
	// Check arguments!
	if (argc < 3){
		fprintf(stderr,"ERROR wrong number of arguments\n");
		fprintf(stderr,"Usage:\n$>%s host port\n", argv[0]);
		exit(0);
	}
	// Connect to the server
	connectServer(&socketfd, atoi(argv[2]),argv[1]);
	initBoard(board);
		

	while(!gameOver){
		recv(socketfd, &code,sizeof(code),0);
		memset(message, 0, STRING_LENGTH-1);
		recv(socketfd, message,STRING_LENGTH-1,0);
		recv(socketfd, board,BOARD_WIDTH * BOARD_HEIGHT,0);
		system("clear");

		printBoard(board, message);
		if(code == TURN_MOVE){
			column = readMove();
			send(socketfd,&column,sizeof(column),0);
		}
		else if(code == GAMEOVER_WIN || code == GAMEOVER_DRAW || code == GAMEOVER_LOSE){
			gameOver = TRUE;
		}
	}
	memset(message, 0, STRING_LENGTH-1);
	recv(socketfd, message,STRING_LENGTH-1,0);
	printf("TOP SCORES:\n%s\n",message);

	
	// Close socket
	close(socketfd);

	return 0;
}
