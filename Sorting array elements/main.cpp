#include <stdio.h>
#include <mpi.h>
#include <iostream>

using namespace std;

const int N = 6;
const int M = 7;

int comp1(const void* a, const void* b)
{
	return (*(long int*)b - *(long int*)a);
}

int main(int argc, char* argv[])
{
	int myid = 0;
	int numprocs = 0;

	double startwtime = 0.0;
	double endwtime = 0.0;

	long int send_array[N][M] = {};

	long int mas[M] = {};
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	srand(time(0));
	if (myid == 0)
	{
		cout << endl << endl << "Process #" << myid << ", generate and sent matrix to processes: " << endl;
		for (int j = 0; j < N; j++)
		{
			for (int i = 0; i < M; i++)
			{
				send_array[j][i] = (100 + rand() % 100) + 10000000;
				cout << send_array[j][i] << " ";
			}
			cout << endl;
		}

		startwtime = MPI_Wtime();

		if (N / numprocs == 0)
		{
			qsort(send_array[0], M, sizeof(long int), comp1);

			for (int i = 1; i < N; i++)
			{
				MPI_Send(&send_array[i][0], M, MPI_LONG, i, 100, MPI_COMM_WORLD);
			}

			for (int i = 1; i < N; i++)
			{
				MPI_Recv(&send_array[i][0], M, MPI_LONG, i, 100, MPI_COMM_WORLD, &status);
			}
		}

		if (N / numprocs > 0)
		{
			for (int a = 0; a < (N / numprocs); a++)
			{
				qsort(send_array[a], M, sizeof(int), comp1);
			}

			for (int i = 1; i < numprocs; i++)
			{
				for (int j = ((i)*N / numprocs); j < ((i + 1) * N / numprocs); j++)
				{
					MPI_Send(&send_array[j][0], M, MPI_LONG, i, 100, MPI_COMM_WORLD);
				}
			}

			for (int i = 1; i < numprocs; i++)
			{
				for (int j = ((i)*N / numprocs); j < ((i + 1) * N / numprocs); j++)
				{
					MPI_Recv(&send_array[j][0], M, MPI_LONG, i, 100, MPI_COMM_WORLD, &status);
				}
			}
		}

		endwtime = MPI_Wtime(); // Засекаем время окончания вычисления
		cout << endl << "wall clock time = " << endwtime - startwtime << endl << endl; // Вывод общего времени вычисления

		cout << "New_maatrix" << endl;
		for (int j = 0; j < N; j++)
		{
			for (int i = 0; i < M; i++)
			{
				cout << send_array[j][i] << " ";
			}
			cout << endl;
		}
	}

	if (myid > 0 && myid < numprocs && N / numprocs > 0)
	{
		for (int j = ((myid)*N / numprocs); j < ((myid + 1) * N / numprocs); j++)
		{
			MPI_Recv(&mas, M, MPI_LONG, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, &status);
			qsort(mas, M, sizeof(long int), comp1);
			MPI_Send(&mas, M, MPI_LONG, 0, 100, MPI_COMM_WORLD);
		}
	}

	if (myid > 0 && myid < N && N / numprocs == 0)
	{
		MPI_Recv(&mas, M, MPI_LONG, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, &status);
		qsort(mas, M, sizeof(long int), comp1);
		MPI_Send(&mas, M, MPI_LONG, 0, 100, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
