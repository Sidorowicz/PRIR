#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <assert.h>
#include <string.h>
int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

int main(int argc, char **argv)
{
    int global_grid[256];
    for(inti=0;i<256;i++){
    gloabal_grid[i]=0;
    }
    global_grid[52]=1;
    global_grid[53]=1;
    global_grid[54]=1;
    global_grid[38]=1;
    global_grid[21]=1;//Setpu glidera z gry o życie
    int num_procs,ID, j,k = 0,steps;
    if (argc == 1){steps = 64;}
    else if (argc == 2){steps = atoi(argv[1]);}
    else{exit(1);}
    MPI_Status stat;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);
    assert(16 % num_procs == 0);
    int *arr = (int *)malloc(16 * ((16 / num_procs) + 2) * sizeof(int));
    for (k = 0; k < steps; k++){
        j = 16;
        for (int i = ID * (256 / num_procs); i < (ID + 1) * (256 / num_procs); i++){
            arr[j] = global_grid[i];
            j++;
        }
        if (num_procs != 1){
            int incoming_1[16];
            int incoming_2[16];
            int send_1[16];
            int send_2[16];
            if (ID % 2 == 0){
                for (int i = 0; i < 16; i++){
                    send_1[i] = arr[i + 16];
                }
                MPI_Ssend(&send_1, 16, MPI_INT, mod(ID - 1, num_procs), 1, MPI_COMM_WORLD);

                for (int i = 0; i < 16; i++){
                    send_2[i] = arr[(16 * (16 / num_procs)) + i];
                }
                MPI_Ssend(&send_2, 16, MPI_INT, mod(ID + 1, num_procs), 1, MPI_COMM_WORLD);
            }
            else{
                MPI_Recv(&incoming_2, 16, MPI_INT, mod(ID + 1, num_procs), 1, MPI_COMM_WORLD, &stat);
                MPI_Recv(&incoming_1, 16, MPI_INT, mod(ID - 1, num_procs), 1, MPI_COMM_WORLD, &stat);
            }
            if (ID % 2 == 0){
                MPI_Recv(&incoming_2, 16, MPI_INT, mod(ID + 1, num_procs), 1, MPI_COMM_WORLD, &stat);
                MPI_Recv(&incoming_1, 16, MPI_INT, mod(ID - 1, num_procs), 1, MPI_COMM_WORLD, &stat);
            }
            else{
                for (int i = 0; i < 16; i++){
                    send_1[i] = arr[i + 16];
                }
                MPI_Ssend(&send_1, 16, MPI_INT, mod(ID - 1, num_procs), 1, MPI_COMM_WORLD);
                for (int i = 0; i < 16; i++){
                    send_2[i] = arr[(16 * (16 / num_procs)) + i];
                }
                MPI_Ssend(&send_2, 16, MPI_INT, mod(ID + 1, num_procs), 1, MPI_COMM_WORLD);
            }
            for (int i = 0; i < 16; i++){
                arr[i] = incoming_1[i];
                arr[(16 * ((16 / num_procs) + 1)) + i] = incoming_2[i];
            }
        }
        else{
            for (int i = 0; i < 16; i++){
                arr[i + 256 + 16] = global_grid[i];
            }
            for (int i = 256; i < 256 + 16; i++){
                arr[i - 256] = global_grid[i - 16];
            }
        }

        int * final = (int *)malloc(16 * ((16 / num_procs)) * sizeof(int));
        for (int k = 16; k < 16 * ((16 / num_procs) + 1); k++){
            int total_rows = 16 * (16 / num_procs) + 2;
            int r = k / 16;
            int c = k % 16;
            int prev_r = mod(r - 1, total_rows);
            int prev_c = mod(c - 1, 16);
            int next_r = mod(r + 1, total_rows);
            int next_c = mod(c + 1, 16);
            int count = arr[prev_r * 16 + prev_c] + arr[prev_r * 16 + c] + arr[prev_r * 16 + next_c] + arr[r * 16 + prev_c] + arr[r * 16 + next_c] + arr[next_r * 16 + prev_c] + arr[next_r * 16 + c] + arr[next_r * 16 + next_c];
            if (arr[k] == 1){
                if (count < 2)
                final[k - 16] = 0;
                else if (count > 3)
                final[k - 16] = 0;
                else
                final[k - 16] = 1;
            }
            else{
                if (count == 3)
                final[k - 16] = 1;
                else
                final[k - 16] = 0;
            }
        }
        j = 0;
        for (int i = ID * (256 / num_procs); i < (ID + 1) * (256 / num_procs); i++){
            global_grid[i] = final[j];
            j++;
        }
        MPI_Gather(final, 16 * (16 / num_procs), MPI_INT, &global_grid, 16 * (16 / num_procs), MPI_INT, 0, MPI_COMM_WORLD);
        if (ID == 0){
            printf("\n %d: 256:\n", k);
            for (j = 0; j < 256; j++){
                if (j % 16 == 0){
                    printf("\n");
                }
                printf("%d  ", global_grid[j]);
            }
            printf("\n");
        }
    }
    free(arr);
    MPI_Finalize();
}
