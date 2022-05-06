#include <iostream>
#include <omp.h>

using namespace std;

const int N = 80;

void printArr(long int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << setw(4) << arr[i] << " ";
        if ((i + 1) % 10 == 0)
        {
            cout << endl;
        }
    }
    cout << endl;
}

int partition(long int arr[], int l, int r)
{
    int i = l + 1;
    int j = r;
    long int key = arr[l];
    long int temp;
    while (true)
    {
        while (i < r && key >= arr[i])
        {
            i++;
        }
        while (key < arr[j])
        {
            j--;
        }
        if (i < j)
        {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
        else
        {
            temp = arr[l];
            arr[l] = arr[j];
            arr[j] = temp;
            return j;
        }
    }
}

void quickSort(long int arr[], int l, int r)
{
    if (l < r)
    {
        int p = partition(arr, l, r);

        #pragma omp parallel
        {
            #pragma omp sections
            {
                #pragma omp section
                {
                    quickSort(arr, l, p - 1);
                }

                #pragma omp section
                {
                    quickSort(arr, p + 1, r);
                }
            }
        }
    }
}

int main()
{
    srand(time(0));
    unsigned int start_time = 0;
    unsigned int end_time = 0;

    long int arr[N] = {};
    for (int i = 0; i < N; i++)
    {
        arr[i] = -1000 + rand() % 2000;
    }

    cout << "generated array:" << endl;
    printArr(arr, N);
    start_time = clock();
    quickSort(arr, 0, N - 1);
    end_time = clock();
    cout << "ordered array:" << endl;
    printArr(arr, N);
    cout << "Time: " << end_time - start_time << endl;

    return 0;
}
