#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <iomanip>

using namespace std;

const int N = 6;
const int M = 40;

int main(int argc, char* argv[])
{
    long int send_array[M] = {};
    long int result_array[M] = {};

    int myid = 0;
    int numprocs = 0;

    double startwtime = 0.0;
    double endwtime = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (myid == 0)
    {
        startwtime = MPI_Wtime();
    }

    srand(time(0) + myid);
    for (int i = 0; i < 40; i++)
    {
        send_array[i] = 10000000000 + rand() % 100000000;
    }

    for (int i = 0; i < M; i++)
    {
        cout << "process #" << myid << " element #" << setw(2) << i << " = " << send_array[i] << " | bit:";
        for (int j = 31; j >= 0; --j)
        {
            cout << ((send_array[i] >> j) & 1);
        }
        cout << endl;
    }

    MPI_Reduce(send_array, result_array, M, MPI_LONG, MPI_BAND, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (myid == 0)
    {
        endwtime = MPI_Wtime(); // Засекаем время окончания вычисления
        cout << endl << "wall clock time = " << endwtime - startwtime << endl << endl; // Вывод общего времени вычисления
    }

    if (myid == 0)
    {
        cout << "result" << endl;
        for (int i = 0; i < M; i++)
        {
            cout << "process #" << myid << " element #" << setw(2) << i << " = " << send_array[i] << " | bit:";
            for (int j = 31; j >= 0; --j)
            {
                cout << ((result_array[i] >> j) & 1);
            }
            cout << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
