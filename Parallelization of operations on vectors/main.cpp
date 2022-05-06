#include <omp.h>
#include <iostream> 

using namespace std;

const int N = 11;

int main(int argc, char* argv[])
{
    int num = 0;
    long int min = 0;
    long int check = 0;
    long int array[N] = {};
    unsigned int start_time = 0;
    unsigned int end_time = 0;
    
    start_time = clock();
    
    #pragma omp parallel firstprivate (array, min)
    {
        srand(int(time(NULL)) ^ omp_get_thread_num());
        #pragma omp critical
        {
            for (int i = 0; i < N; i++)
            {
                array[i] = -1000 + rand() % 2000;
                cout << array[i] << "  ";
            }
            cout << endl;
        }

        min = array[0];
        num = omp_get_num_threads();

        for (int i = 1; i < N; i++)
        {
            if (array[i] < min)
            {
                min = array[i];
            }
        }
        #pragma omp barrier

        #pragma omp critical
        {
            cout << "min = " << min << endl;
        }
    }
    end_time = clock();

    cout << "Time: " << end_time - start_time << endl;
    cout << "num = " << num << endl;

    return 0;
}
