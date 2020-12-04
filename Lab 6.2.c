#include <stdio.h>
#include <math.h>
#include "mpi.h"

float f(float x){
    return x*x;
}

float M_trapez(int a, int b, int c, int d){
    float dx = (b-a) / (float)d;
    if(c == d-1){
        return f(b) / 2 * dx;
    }
    else if (c == 0){
        return f(a) / 2 * dx;
    }
    else{
        return f(a + dx * c) * dx;
    }
}

int main(int argc, char **argv){
    const int TAG = 62087;
    const int a = 0;
    const int b = 10;

    int c;
    int d;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &d);
    MPI_Comm_rank(MPI_COMM_WORLD, &c);
    float val = 0;
    if(c != d-1){
        MPI_Status state;
        int sen = c+1;
        MPI_Recv(&val, 1, MPI_FLOAT, sen, TAG, MPI_COMM_WORLD, &state);
        printf("Procces %d odebral wartosc: %f\n", c, val);
    }
    float curr = M_trapez(a, b, c, d);
    curr += val;
    if(c != 0){
        int adresat = c- 1;
        MPI_Send(&curr, 1, MPI_FLOAT, adresat, TAG, MPI_COMM_WORLD);
        printf("Wartosc procesu %d : %f\n", c, curr);
    }else{
        printf("Waartosc to: %f\n", curr);
    }
    MPI_Finalize();
    return 0;
}
