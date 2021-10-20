#include <iostream>
#include <random>
#include <sys/time.h>
#include "pthread.h"

#define ARRAY_SIZE 1024*1024
#define SUB_ARRAY_LEN_CUTOFF ARRAY_SIZE / 12
using namespace std;


struct thread_data {
    int st_idx;
    int en_idx;
};

float* arrayP;

void partition(float* array, int &i, int &j)
{
    float tmp;
	float pivot = array[(i + j) / 2];

	while (i <= j) {
		while (array[i] < pivot)
			i++;
		while (array[j] > pivot)
			j--;
		if (i <= j) {
			tmp = array[i];
			array[i] = array[j];
			array[j] = tmp;
			i++;
			j--;
		}
	}
}


void partitionP(float* array, int &i, int &j)
{
    float tmp;
	float pivot;
    int N = j - i + 1;
    int count;
    while ( (count < float(N) * 0.25) || (count > float(N) * 0.75) )
    {
        pivot = array[rand() % N + i];
        count = 0;
        for (int k=i; k<=j; k++)
            if (pivot < array[k])
                count++;
    }
        

	while (i <= j) {
		while (array[i] < pivot)
			i++;
		while (array[j] > pivot)
			j--;
		if (i <= j) {
			tmp = array[i];
			array[i] = array[j];
			array[j] = tmp;
			i++;
			j--;
		}
	}
}

void quickSortS(float* array, int left, int right) 
{
	int i = left, j = right;

    partition(array, i, j);
	
    if (left < j)
        quickSortS(array, left, j);

	if (i< right) 
        quickSortS(array, i, right); 
}

void *quickSortP(void *arg_struct) 
{
	struct thread_data *args;
    args = (struct thread_data *) arg_struct;
	int left = args->st_idx, right = args->en_idx;
    int i = left, j = right;

    pthread_t first_thread;
    pthread_t second_thread;

    struct thread_data index_left, index_right;
	

	if ( ((right-left)<SUB_ARRAY_LEN_CUTOFF) )
    {
        partition(arrayP, i, j);
		quickSortS(arrayP, left, j);			
		quickSortS(arrayP, i, right);

	}
    else
    {
        partitionP(arrayP, i, j);
        index_left.st_idx = left;
        index_left.en_idx = j;

        index_right.st_idx = i;
        index_right.en_idx = right;
        pthread_create(&first_thread, NULL, quickSortP,
                                (void *) &index_left);

        pthread_create(&second_thread, NULL, quickSortP,
                            (void *) &index_right);
        pthread_join(first_thread, NULL);
        pthread_join(second_thread, NULL);
	}
    pthread_exit(NULL);
}

int main()
{
    cout << "Rasta Tadayon \t 810196436\nDiyar Mohammadi \t 810196553" << endl;

	struct timeval start1, end1, time1;
	struct timeval start2, end2, time2;

    float* arrayS = new float[ARRAY_SIZE];
    arrayP = new float[ARRAY_SIZE];
    for (int i=0; i<ARRAY_SIZE; i++)
        arrayS[i] = arrayP[i] = rand();

    
    //Serial
    gettimeofday(&start1, NULL);
        quickSortS(arrayS, 0, ARRAY_SIZE-1);
    gettimeofday(&end1, NULL);

    //Parallel
    gettimeofday(&start2, NULL);
    pthread_t first_thread;
    pthread_attr_t attr;

    struct thread_data index_main;
    void *status;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    index_main.st_idx = 0;
    index_main.en_idx = ARRAY_SIZE - 1;
    pthread_create(&first_thread, &attr, quickSortP,
                        (void *) &index_main);
    pthread_join(first_thread, &status);

    pthread_attr_destroy(&attr);

    gettimeofday(&end2, NULL);

    long seconds1 = (end1.tv_sec - start1.tv_sec);
	long micros1 = ((seconds1 * 1000000) + end1.tv_usec) - (start1.tv_usec);

	long seconds2 = (end2.tv_sec - start2.tv_sec);
	long micros2 = ((seconds2 * 1000000) + end2.tv_usec) - (start2.tv_usec);

    int valid_result = 1;
    int same_results = 1;

    for (int i=0; i<ARRAY_SIZE; i++)
    {
        if ((i+1 != ARRAY_SIZE) && (arrayS[i] > arrayS[i+1]))
        {
            valid_result = 0;
            printf("Results are not valid. arrayS[%d] = %f > arrayS[%d] = %f\n", i, arrayS[i], i+1, arrayS[i+1]);
            break;
        }
        if (arrayS[i] != arrayP[i])
        {
            printf("arrayS[%d] = %f, arrayP[%d] = %f\n", i, arrayS[i], i, arrayP[i]);
            same_results = 0;
        }
    }
        
    if (valid_result && same_results)
        printf("Results are valid and the same.\n");
    
    
	printf ("Serial Run time = %ld \n", micros1);
	printf ("Parallel Run time = %ld \n", micros2);
	printf ("Speedup = %4.2f\n", (float) (micros1)/(float) micros2);	
    delete [] arrayS;
    delete [] arrayP;
    pthread_exit(NULL);
    return 0;
}
