#include <stdio.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    const int G = 78222;
    int curr;
    int evr;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &evr);
    MPI_Comm_rank(MPI_COMM_WORLD, &curr);
    float val = 0;
    if(curr != 0){
        MPI_Status status;
        int nadawca = curr-1;
        MPI_Recv(&val, 1, MPI_FLOAT, nadawca, G, MPI_COMM_WORLD, &status);
        printf("Wartosc procesu %d : %f\n", curr, val);
    }
    float act = 4 * powf(-1, curr) / (2.0f * (curr+1) -1 );
    act += val;
    if(curr != evr-1){
        int adresat = curr + 1;
        MPI_Send(&act, 1, MPI_FLOAT, adresat, G, MPI_COMM_WORLD);
        printf("Wartosc procesu %d : %f\n", curr, act);
    }else{
        printf("Wartosc to: %f\n", act);
    }
    MPI_Finalize();
    return 0;
}
