#include "graph.h"
#include "mpi.h"

// Enables/Disables the log messages from the master process
#define DEBUG_MASTER 0

void masterStatic(unsigned short *world, unsigned short *newWorld, int worldWidth, int worldHeight, int size, SDL_Renderer* renderer);
void masterDinamic(unsigned short *world, unsigned short *newWorld, int worldWidth, int worldHeight,int grainSize,int size, SDL_Renderer* renderer);
void endWorkers(int size);