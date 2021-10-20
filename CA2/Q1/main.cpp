#include    <sys/time.h>
#include    "stdio.h"
#include    "x86intrin.h"
#include    <iostream>
#include    <limits> 


#define		VECTOR_SIZE		  	1048576		


int main()
{
    printf("Diyar Mohammadi 810196553\n");
	printf("Rasta Tadayon 810196436\n");
	struct timeval start1, end1;
	struct timeval start2, end2;

	float *fArray = new float [VECTOR_SIZE];

    
	if (!fArray) {
		printf ("Memory allocation error!!\n");
		return 1;
	}
	// Initialize vectors with random numbers
	for (long i = 0; i < VECTOR_SIZE; i++)
		fArray[i] = static_cast <float> (rand());


    //SERIAL
    float maxtmp[4];
    int idxtmp[4];
    for (int i=0; i<4; i++)
    {    
        maxtmp[i] = fArray[i];
        idxtmp[i] = i;}

    gettimeofday(&start1, NULL);
    for (int j = 0; j < 4; j++)
        for(int i=4; i<VECTOR_SIZE; i+=4)
            if (fArray[i+j] > maxtmp[j])
            {
                maxtmp[j] = fArray[i+j];   
                idxtmp[j] = i+j;
            }    
    float fSmax = maxtmp[0];
    int fSidx = idxtmp[0];
    for (int i=1; i<4; i++)
        if (maxtmp[i] > fSmax)
        {
            fSmax = maxtmp[i];
            fSidx = idxtmp[i];
        }
    gettimeofday(&end1, NULL);


    // PARALLEL

    gettimeofday(&start2, NULL);

    const __m128 increment = _mm_set1_ps(4);
    __m128 indices         = _mm_setr_ps(0, 1, 2, 3);
    __m128 maxindices      = indices;
    __m128 maxvalues       = _mm_load_ps(fArray);
    __m128 gt, vec;
    int flag = 5;
    for(int i=4; i<VECTOR_SIZE; i+=4)
    {

        indices = _mm_add_ps(indices, increment);
        vec = _mm_load_ps(&fArray[i]); 
        gt = _mm_cmpgt_ps(vec, maxvalues);
        maxvalues = _mm_max_ps(maxvalues, vec);
        maxindices = _mm_blendv_ps(maxindices, indices, gt);
        
    }
    
    float fVmax = maxvalues[0];
    int fVidx = maxindices[0];
    for (int i=1; i<4; i++)
    {
        if (maxvalues[i] > fVmax)
        {
            fVmax = maxvalues[i];
            fVidx = maxindices[i];
        }
    }

    gettimeofday(&end2, NULL);
	printf ("Max:\nSerial Result   = %f \nParallel Result = %f\n", fSmax, fVmax);
	printf ("idx: \nSerial Result   = %d \nParallel Result = %d\n", fSidx, fVidx);
	long seconds1 = (end1.tv_sec - start1.tv_sec);
	long micros1 = ((seconds1 * 1000000) + end1.tv_usec) - (start1.tv_usec);

	long seconds2 = (end2.tv_sec - start2.tv_sec);
	long micros2 = ((seconds2 * 1000000) + end2.tv_usec) - (start2.tv_usec);
        
	printf ("Serial Run time = %ld \n", micros1);
	printf ("Parallel Run time = %ld \n", micros2);
	printf ("Speedup = %f\n\n", (float) (micros1)/(float) micros2);

}