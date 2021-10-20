#include <iostream>
#include <random>
#include <sys/time.h>
#include <omp.h>

#define ARRAY_SIZE 1024*1024*1
#define THREADS_NUM 6
using namespace std;

int main()
{
    cout << "Rasta Tadayon \t 810196436\nDiyar Mohammadi \t 810196553" << endl;

	struct timeval start1, end1, time1;
	struct timeval start2, end2, time2;

    float* array = new float[ARRAY_SIZE];
    for (int i=0; i<ARRAY_SIZE; i++)
        array[i] = rand();

    int max_id_S = -1;
    float max_value_S = -1;

    //Serial
    gettimeofday(&start1, NULL);
    for (int i=0; i<ARRAY_SIZE; i++)
        if (max_value_S < array[i])
        {
            max_value_S = array[i];
            max_id_S = i;
        }
    gettimeofday(&end1, NULL);

    //Parallel
    float max_values_P[THREADS_NUM];
    for (int i=0; i<THREADS_NUM; i++)
        max_values_P[i] = -1;

    int max_ids_P[THREADS_NUM];
    
    float max_value_P = -1;
    int max_id_P;
    int block_size = ARRAY_SIZE / THREADS_NUM;
    gettimeofday(&start2, NULL);
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int start = tid*block_size;
        int stop = min( (tid+1)*block_size, ARRAY_SIZE);
        for (int i=start; i<stop; i++)
            if (max_values_P[tid] < array[i])
            {
                max_values_P[tid] = array[i];
                max_ids_P[tid] = i;
            }
    }
    for (int i=0; i<THREADS_NUM; i++)
        if (max_value_P < max_values_P[i])
        {
            max_value_P = max_values_P[i];
            max_id_P = max_ids_P[i];
        }
    gettimeofday(&end2, NULL);

    long seconds1 = (end1.tv_sec - start1.tv_sec);
	long micros1 = ((seconds1 * 1000000) + end1.tv_usec) - (start1.tv_usec);

	long seconds2 = (end2.tv_sec - start2.tv_sec);
	long micros2 = ((seconds2 * 1000000) + end2.tv_usec) - (start2.tv_usec);

    printf("Serial result: Max value = %f, Max value id = %d\n", max_value_S, max_id_S);
    printf("Parallel result: Max value = %f, Max value id = %d\n", max_value_P, max_id_P);

	printf ("Serial Run time = %ld \n", micros1);
	printf ("Parallel Run time = %ld \n", micros2);
	printf ("Speedup = %4.2f\n", (float) (micros1)/(float) micros2);	
    delete [] array;
    return 0;
}
