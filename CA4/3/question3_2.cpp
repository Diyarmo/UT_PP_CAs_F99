#include <stdio.h>
#include <math.h>
#include <sys/time.h> 
#include <sys/resource.h>  
#include <omp.h>

#define THREADS_NUM 8

double timeGetTime() 
{     
	struct timeval time;     
	struct timezone zone;     
	gettimeofday(&time, &zone);     
	return time.tv_sec + time.tv_usec*1e-6; 
}  


const long int VERYBIG = 100000;

int main( void )
{
  printf("Rasta Tadayon \t 810196436\nDiyar Mohammadi \t 810196553\n");
  int i;
  long int j, k, sum;
  double sumx, sumy, total, z;
  double starttime, elapsedtime, total_time=0;
  double starttimes[THREADS_NUM], elapsedtimes[THREADS_NUM], total_times[THREADS_NUM];

  // ---------------------------------------------------------------------
  // Output a start message
  printf( "OpenMP Parallel Timings for %ld iterations \n\n", VERYBIG );


  for (int i=0; i<THREADS_NUM; i++)
      total_times[i] = 0;
  

  // repeat experiment several times
  for( i=0; i<6; i++ )
  {
    for (int i=0; i<THREADS_NUM; i++)
      elapsedtimes[i] = 0;
  
    // get starting time
    starttime = timeGetTime();
    // reset check sum and total
    sum = 0;
    total = 0.0;
    
    // Work loop, do some work by looping VERYBIG times
    #pragma omp parallel for     \
      num_threads (THREADS_NUM) \
      private( sumx, sumy, k )   \
      reduction( +: sum, total ) \
      schedule( dynamic, 2000 )
    for( int j=0; j<VERYBIG; j++ )
    {
      starttimes[omp_get_thread_num()] = timeGetTime();
      // increment check sum
      sum += 1;
    
      // Calculate first arithmetic series
      sumx = 0.0;
      for( k=0; k<j; k++ )
      sumx = sumx + (double)k;

      // Calculate second arithmetic series
      sumy = 0.0;
      for( k=j; k>0; k-- )
      sumy = sumy + (double)k;

      if( sumx > 0.0 )total = total + 1.0 / sqrt( sumx );
      if( sumy > 0.0 )total = total + 1.0 / sqrt( sumy );
      elapsedtimes[omp_get_thread_num()] += timeGetTime() - starttimes[omp_get_thread_num()];

    }
    for (int i=0; i<THREADS_NUM; i++)
      total_times[i] += elapsedtimes[i];
      
    
    // get ending time and use it to determine elapsed time
    elapsedtime = timeGetTime() - starttime;
    total_time += elapsedtime;
  
    // report elapsed time
    printf("Time Elapsed %10d mSecs Total=%lf Check Sum = %ld\n",
                   (int)(elapsedtime * 1000), total, sum );
    printf("Time elapsed in each thread:\n");
    for (int i=0; i<THREADS_NUM; i++)
      printf("\tT%d: %10d mSecs\n", i, (int)(elapsedtimes[i] * 1000));

  }
  printf("Mean Time Elapsed %10d mSecs \n",(int)(total_time * 1000 / 6));
  printf("Mean Time elapsed in each thread:\n");
  for (int i=0; i<THREADS_NUM; i++)
    printf("\tT%d: %10d mSecs\n", i, (int)(total_times[i] * 1000 / 6));  


  // return integer as required by function header
  return 0;
}
// **********************************************************************
