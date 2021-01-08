#include <stdio.h>
#include <math.h>
#include "mpi.h"
float F(float x)
{
    return x*x;
}
float Trapez(int a, int b, int number_SE, int Whole_collection)
{
    float dx = (b-a) / (float)Whole_collection;
    if(number_SE == Whole_collection-1)
    {
        return F(b) / 2 * dx;
    }
    else if (number_SE == 0)
    {
        return F(a) / 2 * dx;
    }
    else
    {
        return F(a + dx * number_SE) * dx;
    }
}
int main(int argc, char **argv)
{
    const int TAG = 62087;
    const int a = 0;
    const int b = 10;

    int current;
    int Whole_collection;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &Whole_collection);
    MPI_Comm_rank(MPI_COMM_WORLD, &current);
    float odebrana_wartosc = 0;
    if(current != Whole_collection-1)
    {
        MPI_Status status;
        int nadawca = current+1;
        MPI_Recv(&odebrana_wartosc, 1, MPI_FLOAT, nadawca, TAG, MPI_COMM_WORLD, &status);
        printf("Process %d got value: %f\n", current, odebrana_wartosc);
    }

    float current_value = Trapez(a, b, current, Whole_collection);
    current_value += odebrana_wartosc;

    if(current != 0)
    {
        int sender = current - 1;
        MPI_Send(&current_value, 1, MPI_FLOAT, sender, TAG, MPI_COMM_WORLD);
        printf("Process %d sent value: %f\n", current, current_value);
    }
    else
    {
        printf("Calculated value is: %f\n", current_value);
    }


    MPI_Finalize();
    return 0;
}
