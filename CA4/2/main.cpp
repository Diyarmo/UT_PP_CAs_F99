#include <iostream>
#include <random>
#include <sys/time.h>
#include <omp.h>

#define ARRAY_SIZE 1024*1024
#define THREADS_NUM 8
#define SUB_ARRAY_LEN_CUTOFF 1500
using namespace std;

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

void quickSortS(float* array, int left, int right) 
{
	int i = left, j = right;

    partition(array, i, j);
	
    if (left < j)
        quickSortS(array, left, j);

	if (i< right) 
        quickSortS(array, i, right); 
}

void quickSortP(float* array, int left, int right) 
{
	
	int i = left, j = right;
	
    partition(array, i, j);

	if ( ((right-left)<SUB_ARRAY_LEN_CUTOFF) )
    {
		if (left < j){ quickSortP(array, left, j); }			
		if (i < right){ quickSortP(array, i, right); }

	}
    else
    {
		#pragma omp task 	
		{ 
            quickSortP(array, left, j); 
        }
		#pragma omp task 	
		{ 
            quickSortP(array, i, right);
        }		
	}
}

int main()
{
    cout << "Rasta Tadayon \t 810196436\nDiyar Mohammadi \t 810196553" << endl;

	struct timeval start1, end1, time1;
	struct timeval start2, end2, time2;

    float* arrayS = new float[ARRAY_SIZE];
    float* arrayP = new float[ARRAY_SIZE];
    for (int i=0; i<ARRAY_SIZE; i++)
        arrayS[i] = arrayP[i] = rand();

    
    //Serial
    gettimeofday(&start1, NULL);
        quickSortS(arrayS, 0, ARRAY_SIZE-1);
    gettimeofday(&end1, NULL);

    //Parallel
    gettimeofday(&start2, NULL);
    #pragma omp parallel num_threads(THREADS_NUM)
	{	
		#pragma omp single nowait
		{
			quickSortP(arrayP, 0, ARRAY_SIZE-1);	
		}
	}	
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
    return 0;
}
