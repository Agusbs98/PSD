#include "master.h"

void masterStatic(unsigned short *world, unsigned short *newWorld, int worldWidth, int worldHeight, int size, SDL_Renderer* renderer){
    int totalSend = 0, totalRecv = 0,count = 0, grainSize = worldHeight/(size-1),tag = 0;
    int *indexTable = (int*) malloc(size*sizeof(int));
    unsigned short *area, *top, *bottom;
    MPI_Status status;

    area = world;
    top = world+(worldWidth * (worldHeight-1));
    bottom = world+(worldWidth*grainSize);
    for(int i = 1; i<size-1; i++){
        indexTable[i] = totalSend;
        MPI_Send(&grainSize, 1,MPI_INT,i,tag,MPI_COMM_WORLD);
        MPI_Send(top, worldWidth,MPI_SHORT,i,tag,MPI_COMM_WORLD);
        MPI_Send(area, grainSize*worldWidth,MPI_SHORT,i,tag,MPI_COMM_WORLD);
        MPI_Send(bottom, worldWidth,MPI_SHORT,i,tag,MPI_COMM_WORLD);
        totalSend += grainSize;
        area = bottom;
        top = world+(worldWidth*(totalSend-1));
        bottom = world+(worldWidth*(totalSend+grainSize));
    }
    bottom = world;
    indexTable[size-1] = totalSend;
    int send = worldHeight - totalSend;
    MPI_Send(&send, 1,MPI_INT,size-1,tag,MPI_COMM_WORLD);
    MPI_Send(top, worldWidth,MPI_SHORT,size-1,tag,MPI_COMM_WORLD);
    MPI_Send(area, send*worldWidth,MPI_SHORT,size-1,tag,MPI_COMM_WORLD);
    MPI_Send(bottom, worldWidth,MPI_SHORT,size-1,tag,MPI_COMM_WORLD);

    totalSend+= send;
    while(totalRecv < totalSend){
        MPI_Recv(&count, 1, MPI_INT, MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);
        MPI_Recv(newWorld + (indexTable[status.MPI_SOURCE]*worldWidth),count*worldWidth,MPI_SHORT,status.MPI_SOURCE,tag,MPI_COMM_WORLD,&status);
        drawWorld(world,newWorld,renderer, indexTable[status.MPI_SOURCE], indexTable[status.MPI_SOURCE]+count, worldWidth, worldHeight);
        totalRecv+=count;
    }
    free(indexTable);
}

void masterDinamic(unsigned short *world, unsigned short *newWorld, int worldWidth, int worldHeight,int grainSize,int size, SDL_Renderer* renderer){
    int totalSend = 0, totalRecv = 0,count = 0, tag = 0;
    int *indexTable = (int*) malloc(size*sizeof(int));
    unsigned short *area, *top, *bottom;
    MPI_Status status;

    area = world;
    top = world + (worldWidth * (worldHeight-1));
    bottom = world + (worldWidth*grainSize);

    for(int i= 1; i<size && totalSend < worldHeight; ++i){
        indexTable[i] = totalSend;

        if(worldHeight - totalSend > grainSize){
            MPI_Send(&grainSize, 1,MPI_INT,i,tag,MPI_COMM_WORLD);
            MPI_Send(top, worldWidth,MPI_SHORT,i,tag,MPI_COMM_WORLD);
            MPI_Send(area, grainSize*worldWidth,MPI_SHORT,i,tag,MPI_COMM_WORLD);
            MPI_Send(bottom, worldWidth,MPI_SHORT,i,tag,MPI_COMM_WORLD);
            totalSend += grainSize;
        }
        else {
            int numSend = worldHeight - totalSend;
            bottom = world;
            MPI_Send(&numSend, 1,MPI_INT,i,tag,MPI_COMM_WORLD);
            MPI_Send(top, worldWidth,MPI_SHORT,i,tag,MPI_COMM_WORLD);
            MPI_Send(area, numSend*worldWidth,MPI_SHORT,i,tag,MPI_COMM_WORLD);
            MPI_Send(bottom, worldWidth,MPI_SHORT,i,tag,MPI_COMM_WORLD);
            totalSend += numSend;
        }

        area = bottom;
        top = world+(worldWidth*(totalSend-1));
        bottom = world+(worldWidth*(totalSend+grainSize));

    }
    while(totalRecv < totalSend){
        MPI_Recv(&count, 1, MPI_INT, MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);
        MPI_Recv(newWorld + (indexTable[status.MPI_SOURCE]*worldWidth),count*worldWidth,MPI_SHORT,status.MPI_SOURCE,tag,MPI_COMM_WORLD,&status);
        drawWorld(world,newWorld,renderer, indexTable[status.MPI_SOURCE], indexTable[status.MPI_SOURCE]+count, worldWidth, worldHeight);
        totalRecv+=count;
        if(totalSend<worldHeight){
            indexTable[status.MPI_SOURCE] = totalSend;

            if(worldHeight - totalSend >= grainSize){
                MPI_Send(&grainSize, 1,MPI_INT,status.MPI_SOURCE,tag,MPI_COMM_WORLD);
                MPI_Send(top, worldWidth,MPI_SHORT,status.MPI_SOURCE,tag,MPI_COMM_WORLD);
                MPI_Send(area, grainSize*worldWidth,MPI_SHORT,status.MPI_SOURCE,tag,MPI_COMM_WORLD);
                MPI_Send(bottom, worldWidth,MPI_SHORT,status.MPI_SOURCE,tag,MPI_COMM_WORLD);
                totalSend += grainSize;
            }
            else {
                int numSend = worldHeight - totalSend;
                bottom = world;
                MPI_Send(&numSend, 1,MPI_INT,status.MPI_SOURCE,tag,MPI_COMM_WORLD);
                MPI_Send(top, worldWidth,MPI_SHORT,status.MPI_SOURCE,tag,MPI_COMM_WORLD);
                MPI_Send(area, numSend*worldWidth,MPI_SHORT,status.MPI_SOURCE,tag,MPI_COMM_WORLD);
                MPI_Send(world, worldWidth,MPI_SHORT,status.MPI_SOURCE,tag,MPI_COMM_WORLD);
                totalSend += numSend;
            }  

        area = bottom;
        top = world+(worldWidth*(totalSend-1));
        bottom = world+(worldWidth*(totalSend+grainSize));
        }

    }
    free(indexTable);
}

void endWorkers(int size){
    int fin = 0;
    for(int i = 1; i<size; ++i)
    MPI_Send(&fin, 1,MPI_INT,i,0,MPI_COMM_WORLD);
}