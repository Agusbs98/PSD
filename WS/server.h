#include "soapH.h"
#include "conecta3x3.nsmap"
#include "game.h"
#include <pthread.h>

/** Maximum number of active games in the server */
#define MAX_GAMES 5

/** Type for game status */
typedef enum {gameEmpty, gameWaitingPlayer, gameReady} tGameState;


/**
 * Struct that contains a game for 3 players
 */
typedef struct game{
	xsd__string board;						/** Board of the game */
	conecta3x3ns__tPlayer currentPlayer;	/** Current player */
	xsd__string player1Name;				/** Name of player 1 */
	xsd__string player2Name;				/** Name of player 2 */
	xsd__string player3Name;				/** Name of player 3 */
	int endOfGame;							/** Flag to control the end of the game*/
	tGameState status;						/** Flag to indicate the status of this game */
	int activePlayers;						/** Number of active players in the game */

	//TODO: include mutex and condition variables
	pthread_mutex_t mutexGame;
	pthread_mutex_t mutexPlayers;
	pthread_cond_t cond;
}tGame;


/**
 * Initialize server structures
 */
void initServerStructures ();

/**
 * Calculates the next player
 *
 * @param currentPlayer Current player
 * @return Player that is currently awaiting.
 */
conecta3x3ns__tPlayer calculateNextPlayer (conecta3x3ns__tPlayer currentPlayer);

/**
 * Calculates a random player to start the game
 *
 * @return Player that starts the game.
 */
conecta3x3ns__tPlayer calculateRandomPlayer ();


/**
 * Search for a game.
 *
 * This function returns a number between [0-MAX_GAMES] if there
 * exists an empty game, or ERROR_SERVER_FULL in other case.
 */
int searchEmptyGame ();


/**
 * Locate the game where a given player is active.
 *
 * @param player - Player name
 * @return If the player is found, then the index of the game is
 * returned [0-MAX_GAMES]. In other case, this function returns ERROR_PLAYER_NOT_FOUND
 */
int locateGameForPlayer (xsd__string player);


/**
 * Copy the three parameters (message, board and newCode) into a conecta3x3ns__tBlock structure.
 *
 * @param message Message to the player
 * @param board Current board
 * @param newCode Result of the current operation
 */
void copyGameStatusStructure (conecta3x3ns__tBlock* status, char* message, char* board, int newCode);


/**
 * Thread function to process client requests
 */
void *processRequest(void*);
