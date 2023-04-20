#include "game.h"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void createSocket(int *socketfd, int port);
void conectSockets(tThreadArgs *players, int socketfd);
void getNames(tThreadArgs players,tPlayersNames *playersNames);
void turnMove(int playerSocket, tBoard board, char chip);
void turnWait(int playerSocket, tBoard board, char chip);
void recvMove(tBoard board, tPlayer player, tThreadArgs players);
int isFinal(tBoard board, tPlayer player, tThreadArgs players);
void sendWin(int socketPlayer, tBoard board);
void sendLose(int socketPlayer, tBoard board);
void sendDraw(int socketPlayer, tBoard board);
void readPoints(tListPlayerPoints *points);
void writePoints(tString player);
void game(void*players);