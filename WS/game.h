#include "utils.h"

/**
 * Init the board
 *
 * @param board Board of the game
 */
void initBoard (xsd__string board);

/**
 * Check if a given move is valid
 *
 * @param board Board of the game
 * @param column Number of the column where the chip is going to be inserted
 * @return Result of the current move
 */
conecta3x3ns__tMove checkMove (xsd__string board, unsigned int column);

/**
 * Inserts a chip in the board
 *
 * @param board Board of the game
 * @param player Player that must move
 * @param column Column to insert the chip
 */
void insertChip (xsd__string board, conecta3x3ns__tPlayer player, unsigned int column);

/**
 * Check if a player is the winner
 *
 * @param board Board of the game
 * @param player Player that has performed the last move
 * @return TRUE is player is the winner of FALSE in another case
 */
int checkWinner (xsd__string board, conecta3x3ns__tPlayer player);

/**
 * Check if the board is full
 *
 * @param board Board of the game
 * @return TRUE if the board is full or FALSE in another case
 */
int isBoardFull (xsd__string board);





