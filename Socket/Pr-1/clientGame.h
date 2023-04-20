#include "game.h"

void connectServer(int *socketfd, int port, char* serverIP);
/**
 * Reads a move entered by the player
 * @return A number between [0-BOARD_WIDTH) that represents the column where the chip is going to be inserted
 */
unsigned int readMove ();


