#include 	"stdio.h"
#include 	<sys/time.h>
#include 	"x86intrin.h"
#include 	"math.h"

#define		VECTOR_SIZE		  	1048576		

int main (void)
{
	printf("Diyar Mohammadi 810196553\n");
	printf("Rasta Tadayon 810196436\n");
	struct timeval start1, end1;
	struct timeval start2, end2;
	 
	float fSTmpRes[4];
	float fSMean, fSSTD;
	float fVMean, fVSTD;

	float *fArray;
	fArray = new float [VECTOR_SIZE];

	if (!fArray) {
		printf ("Memory allocation error!!\n");
		return 1;
	}
	// Initialize vectors with random numbers
	for (long i = 0; i < VECTOR_SIZE; i++)
		fArray[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));

	// Serial implementation
	gettimeofday(&start1, NULL);

	fSTmpRes[0] = fSTmpRes[1] = fSTmpRes[2] = fSTmpRes[3] = 0.0;
	for (int j = 0; j < 4; j++)
		for (long i = 0; i < VECTOR_SIZE; i+=4)
				fSTmpRes[j] += fArray[i + j];
	fSMean = (fSTmpRes[0] + fSTmpRes[1] + fSTmpRes[2] + fSTmpRes[3]) / float(VECTOR_SIZE);

	fSTmpRes[0] = fSTmpRes[1] = fSTmpRes[2] = fSTmpRes[3] = 0.0;
	for (int j = 0; j < 4; j++)
		for (long i = 0; i < VECTOR_SIZE; i+=4)
				fSTmpRes[j] += (fArray[i + j] - fSMean) * (fArray[i + j] - fSMean);

	fSSTD = (fSTmpRes[0] + fSTmpRes[1] + fSTmpRes[2] + fSTmpRes[3]) / float(VECTOR_SIZE);
	fSSTD = sqrt(fSSTD);

	gettimeofday(&end1, NULL);

	// Parallel implementation
	gettimeofday(&start2, NULL);
	__m128 vec, mean;
	__m128 sum = _mm_set1_ps(0.0);
	for (long i = 0; i < VECTOR_SIZE; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
		sum = _mm_add_ps (sum, vec);
	}
	fVMean = (sum[0] + sum[1] + sum[2] + sum[3]) / float(VECTOR_SIZE);
	mean = _mm_load_ps1(&fVMean);
	sum = _mm_set1_ps(0.0);

	for (long i = 0; i < VECTOR_SIZE; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
		vec = _mm_sub_ps(mean, vec);
		vec = _mm_mul_ps(vec, vec);
		sum = _mm_add_ps (sum, vec);
	
	}

	sum = _mm_hadd_ps (sum, sum);
	sum = _mm_hadd_ps (sum, sum);
	fVSTD = _mm_cvtss_f32 (sum) / float(VECTOR_SIZE);
	fVSTD = sqrt(fVSTD);
	gettimeofday(&end2, NULL);

	printf ("Mean:\nSerial Result   = %f \nParallel Result = %f\n", fSMean, fVMean);
	printf ("STD: \nSerial Result   = %f \nParallel Result = %f\n", fSSTD, fVSTD);
	long seconds1 = (end1.tv_sec - start1.tv_sec);
	long micros1 = ((seconds1 * 1000000) + end1.tv_usec) - (start1.tv_usec);

	long seconds2 = (end2.tv_sec - start2.tv_sec);
	long micros2 = ((seconds2 * 1000000) + end2.tv_usec) - (start2.tv_usec);
        
	printf ("Serial Run time = %ld \n", micros1);
	printf ("Parallel Run time = %ld \n", micros2);
	printf ("Speedup = %f\n\n", (float) (micros1)/(float) micros2);

	return 0;
}
