#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>

using namespace std;

const int N = 3;
const int M = 10;

int main(int argc, char* argv[])
{
    double startwtime = 0.0; // начальное время вычисления
    double endwtime = 0.0; // конечное время вычисления

    MPI_Init(&argc, &argv);

    int buffer[M] = {};
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int root_rank = 0;

    int my_rank = 0;
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int* my_value = new int[3] {};

    if (my_rank == root_rank)
    {
        cout << "Values to scatter from process " << my_rank << ":";
        for (int i = 0; i < M; i++)
        {
            buffer[i] = i;
            cout << " " << buffer[i];
        }
        cout << endl;

        startwtime = MPI_Wtime(); // Засекаем время начала вычисления
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(buffer, 3, MPI_INT, my_value, 3, MPI_INT, root_rank, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    if (my_rank == root_rank)
    {
        endwtime = MPI_Wtime();
        cout << "wall clock time = " << endwtime - startwtime << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    cout << "Process "<< my_rank << " received value =";
    for (int i = 0; i < 3; i++)
    {
        cout << " " << my_value[i];
    }

    MPI_Finalize();

    return 0;
}
