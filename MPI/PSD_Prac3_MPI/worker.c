#include "worker.h"

void worker(int worldWidth, int worldHeight){
    
    unsigned short * currentWorld = (unsigned short *)malloc (worldWidth * worldHeight * sizeof (unsigned short));
    unsigned short * newWorld = (unsigned short *)malloc (worldWidth * worldHeight * sizeof (unsigned short));

    int count, tag = 0;
    MPI_Status status;
    while(1){
        MPI_Recv(&count,1,MPI_INT,MASTER,tag,MPI_COMM_WORLD,&status);
        if(count == 0)break;
        MPI_Recv(currentWorld, worldWidth, MPI_SHORT, MASTER, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(currentWorld+(worldWidth), count*worldWidth, MPI_SHORT, MASTER, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(currentWorld+(worldWidth + count*worldWidth), worldWidth, MPI_SHORT, MASTER, tag, MPI_COMM_WORLD, &status);

        tCoordinate cell;
        for (int row=1; row<=count; row++){
            cell.row = row;
            for (int col=0; col<worldWidth; col++){
                cell.col = col;
                updateCell (&cell, currentWorld, newWorld, worldWidth, worldHeight);
            }
        }
        MPI_Send(&count, 1, MPI_INT, MASTER, tag, MPI_COMM_WORLD);
        MPI_Send(newWorld+(worldWidth), count*worldWidth, MPI_SHORT, MASTER, tag, MPI_COMM_WORLD);   
    }
}
