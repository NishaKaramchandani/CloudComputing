#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include<time.h>
#include<math.h>
#include<immintrin.h>

pthread_mutex_t mutex;

//clock_t iteration_start, iteration_end;

struct timeval tv;

time_t iteration_start, iteration_end;

double time_taken;

int no_of_threads;

long long no_of_iterations = 1000000000000;

char * precision = NULL;

void *work_SP()
{
        long long i = 0;
        
        float temp[16];
  		for(int k = 0; k < 16; k++) {
    		temp[k] = (float) k;
  		}
  		
  		__m256 data1 = _mm256_load_ps(temp);
  		__m256 data2 = _mm256_load_ps(temp+8);
  		__m256 data3;
		
		
		for( i = 0; i < no_of_iterations/(16 * no_of_threads); i++) {
			data3 = _mm256_add_ps(data1, data2);
			data1 = _mm256_sub_ps(data3, data2);	
		} 
        _mm256_store_ps(temp, data3);
  		printf("\nAfter Operations::%f", temp[0]);
}


void *work_DP()
{
        long long i = 0;
        
        double temp[8];
  		for(int k = 0; k < 8; k++) {
    		temp[k] = (double) k;
  		}
  		
  		__m256d data1 = _mm256_load_pd(temp);
  		__m256d data2 = _mm256_load_pd(temp+4);
  		__m256d data3;
		
        for( i = 0; i < no_of_iterations/(8 * no_of_threads); i++) {
			data3 = _mm256_add_pd(data1, data2);
			data1 = _mm256_sub_pd(data3, data2);	
		}
		_mm256_store_pd(temp, data3);
  		printf("\nAfter Operations::%f", temp[0]);
}


void *work_HP()
{
        long long i = 0;
		
  		__m256i data1 = _mm256_set_epi16(10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160);
  		__m256i data2 = _mm256_set_epi16(10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160);
  		__m256i data3;
    
		for(i = 0; i < no_of_iterations/(32 * no_of_threads); i++)
		{
			data3 = _mm256_add_epi16(data1, data2);
			data1 = _mm256_sub_epi16(data3, data2);
		
		} 
  		_mm256_cmpeq_epi16(data3,data1); 
        
}


void *work_QP()
{
       	long long int i = 0;
       	
		__m256i data1 = _mm256_set_epi8('a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f');
		__m256i data2 = _mm256_set_epi8('a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f');
		__m256i data3;
    
		for(i = 0; i < no_of_iterations/(64 * no_of_threads); i++)
		{
			data3 = _mm256_add_epi8(data1, data2);
			data1 = _mm256_sub_epi8(data3, data2);
		
		}
		
		_mm256_cmpeq_epi8(data3,data1); 
}

void route_precision_calls(void * function_name){

        pthread_t threads[no_of_threads]; long tid; int rc;
        pthread_mutex_init(&mutex, NULL);
		gettimeofday(&tv, NULL); 
		double operations_per_second;
		double giga_ops;
 		iteration_start = tv.tv_sec;
        for (tid = 0; tid < no_of_threads; tid++) {
                rc = pthread_create(&threads[tid], NULL, function_name, (void *) tid);
                if (rc) {
                        printf("Could not create thread %ld\n", tid);
                }
        }
        for (tid = 0; tid < no_of_threads; tid++) {
                rc = pthread_join(threads[tid], NULL);
                if (rc) {
                        printf("Could not join thread %ld\n", tid);
                }
        }
        gettimeofday(&tv, NULL); 
 		iteration_end = tv.tv_sec;
        time_taken = (double)(iteration_end - iteration_start);
        printf("\nTime taken:: %f", time_taken);
        operations_per_second = no_of_iterations/time_taken;
        giga_ops = operations_per_second/1000000000;
        printf("\nOperations per second:: %f", operations_per_second);
        printf("\nGiga Ops:: %f\n", giga_ops);
        FILE * fp;
        fp = fopen("./output/MyCPUBench.out.dat", "a+");
        fprintf(fp, "%s\t%d\t%f", precision, no_of_threads, giga_ops);
        fclose(fp);
        pthread_mutex_destroy(&mutex);
}

int main(int argc, char **argv)
{

        FILE * fp;
        size_t len = 0;
        char * threads = NULL;

        //fp = fopen("cpu_SP_1thread.dat", "r");
        fp = fopen(argv[1], "r");
        if (fp == NULL)
                exit(EXIT_FAILURE);

        if(getline(&precision, &len, fp) != -1) {
                //printf("Retrieved line of length %zu :\n", read);
                strtok(precision, "\n");
                printf("Precision: %s", precision);
        }


        if(getline(&threads, &len, fp) != -1) {
                //printf("Retrieved line of length %zu :\n", read);
                no_of_threads = atoi(threads);
                printf("\nNumber of threads: %d", no_of_threads);
        }

        fclose(fp);
        if(precision[0] == 'D'){
                printf("\nCalling DP function!");
                route_precision_calls(work_DP);
        }else if(precision[0] == 'S'){
                printf("\nCalling SP function!");
                route_precision_calls(work_SP);
        }else if(precision[0] == 'H'){
                printf("\nCalling HP function!");
                route_precision_calls(work_HP);
        }else if(precision[0] == 'Q'){
                printf("\nCalling QP function!");
                route_precision_calls(work_QP);
        }else{
                printf("\nCalling Default function!");
                route_precision_calls(work_DP);
        }
        pthread_exit(NULL);
}
