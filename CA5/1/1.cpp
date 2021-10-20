#include "stdio.h"
#include <iostream>
#include "pthread.h"
#include <chrono> 
#include <iomanip> 



#define MAX_NUM 1e6
#define MIN_NUM 0.0
#define N 1048576
#define N_THREADS 6

float array[N];

float Max = MIN_NUM;
int Idx;
pthread_mutex_t lock;


void *get_maximum(void* arg)
{
    int thread_num = *(int *) arg;
    int n = N / N_THREADS;  //number of elements to handle for each thread
    int start = thread_num * n;
    int end = (thread_num != N_THREADS - 1) ? start + n : N;

    float local_max = MIN_NUM;
    int local_idx;
    for (int i = start; i < end; i++)
    {
        if(array[i] > local_max)
        {
            local_max = array[i];
            local_idx = i;
        }
    }
    pthread_mutex_lock(&lock);
    if(local_max > Max)
    {
        Max = local_max;
        Idx = local_idx;
    }
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}


int main()
{
    std::cout << "Rasta Tadayon:\t\t 810196436" << std::endl << "Diyar Mohammadi:\t 810196553\n"<< std::endl;
    using namespace std::chrono; 
    std::chrono::time_point<std::chrono::system_clock> start_c, end_c; 

    //initialization
    for (int i=0; i < N; i++)
    {
        array[i] = rand()/float(RAND_MAX)*MAX_NUM+0.08;
    }

    //serial
    start_c = std::chrono::system_clock::now();
    float max = 0.0;
    int max_idx;
    for (int i = 0; i < N; i++)
    {
        if(array[i] > max)
        {
            max = array[i];
            max_idx = i;
        }
    }
    end_c = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds_serial = end_c - start_c; 
    std::cout << std::setprecision(3) << std::fixed;
    std::cout << "serial results: " << std::endl << "max = " << max << "\tindex = " <<
         max_idx << "\nserial time = " << elapsed_seconds_serial.count() * 1000 << " ms" << std::endl;


    //parallel
    int thread_nums[N_THREADS];
    pthread_t threads[N_THREADS];
    
    start_c = std::chrono::system_clock::now();
    for (int i = 0; i < N_THREADS; i++)
    {
        thread_nums[i] = i;
        pthread_create(&threads[i], NULL, get_maximum, (void *)&thread_nums[i]);
    }
    for (int i = 0; i < N_THREADS; i++)
        pthread_join(threads[i], NULL);

    end_c = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds_parallel = end_c - start_c; 


    //results
    std::cout << "\n\nparallel results: " << std::endl << "max = " << std::setprecision(3) << Max << "\tindex = " <<
         Idx << "\nparallel time = " << elapsed_seconds_parallel.count() * 1000 << " ms" << std::endl;

    float speedup = (float) elapsed_seconds_serial.count() / elapsed_seconds_parallel.count();

    std::cout << "SPEEDUP = " << speedup << std::endl;
    
    return 0;
}