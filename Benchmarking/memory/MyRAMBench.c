#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>

long long total_size = 1000000000;
long long latency_total_size = 100000000;
int block_size;
int no_of_threads = 1;
char * operation = NULL;

void *read_write_sequential_throughput(){
	char *source, *destination;
	int i = 0, j = 0;
	source = (char *)calloc(total_size, sizeof(char));
	destination = (char *)malloc(total_size);
	for(j = 0; j < 100/no_of_threads; j++){
		for(i = 0; i < total_size; i+=block_size){
			memcpy(destination+i, source+i, block_size);
		}
	}
}

void *read_write_random_throughput(){
	char *source, *destination;
	long long i = 0, j = 0, index = 0;
	source = (char *)calloc(total_size, sizeof(char));
	destination = (char *)malloc(total_size);
	for(j = 0; j < 100/no_of_threads; j++){
		for(i = 0; i < total_size; i+=block_size){
			if(total_size == block_size){
				index = i;
			}else{
				index = (rand()%(total_size-block_size));
			}
			memcpy(destination+index, source+index, block_size);
		}
	}
}

void *read_write_sequential_latency(){
	char *source, *destination;
	int i = 0, j = 0;
	source = (char *)calloc(latency_total_size, sizeof(char));
	destination = (char *)malloc(latency_total_size);
	for(i = 0; i < latency_total_size; i+=block_size){
		memcpy(destination+i, source+i, block_size);
	}
}

void *read_write_random_latency(){
	char *source, *destination;
	long long i = 0, j = 0, index = 0;
	source = (char *)calloc(latency_total_size, sizeof(char));
	destination = (char *)malloc(latency_total_size);
	for(i = 0; i < latency_total_size; i+=block_size){
		index = (rand()%(latency_total_size-block_size));
		memcpy(destination+index, source+index, block_size);
	}
}

void route_operation_calls(void * function_name){
	int no_of_threads = 1;
	struct timeval tv;
	pthread_mutex_t mutex;
	double time_taken;
	double operations_per_second;
	double throughput, latency;
	time_t iteration_start, iteration_end; 
    pthread_t threads[no_of_threads]; long tid; int rc;
    pthread_mutex_init(&mutex, NULL);
	gettimeofday(&tv, NULL); 
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
    printf("\nTime taken:: %f\n", time_taken);
    FILE * fp;
    if(block_size == 1){
    	latency = time_taken/(double)latency_total_size;
    	fp = fopen("./output/MyRAMBenchLatency.out.dat", "a+");
    	fprintf(fp, "\n%s\t%d\t%d\t%f", operation, no_of_threads, block_size, latency*1000000);
    }else{
    	operations_per_second = (total_size * 100)/time_taken;
    	throughput = operations_per_second/1000000000;
    	fp = fopen("./output/MyRAMBenchThroughput.out.dat", "a+");
    	fprintf(fp, "\n%s\t%d\t%d\t%f", operation, no_of_threads, block_size, throughput);
    }
    fclose(fp);
    pthread_mutex_destroy(&mutex);
}

int main(int argc, char *argv[]) {
	FILE * fp;
    size_t len = 0;
    char * block_data = NULL;
    char * threads = NULL;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    if(getline(&operation, &len, fp) != -1) {
    		strtok(operation, "\n");
            printf("\nValue read for Operation: %s", operation);
    }


    if(getline(&block_data, &len, fp) != -1) {
    	strtok(block_data, "\n");
        block_size = atoi(block_data);
        printf("\nValue read for block_size: %d", block_size);
    }
    
    if(getline(&threads, &len, fp) != -1) {
    	strtok(threads, "\n");
        no_of_threads = atoi(threads);
        printf("\nValue read for threads: %d", no_of_threads);
    }

    fclose(fp);
    if(strcmp(operation,"RWR") == 0){
        printf("\nCalling read_write_random\n");
        if(block_size == 1){
        	route_operation_calls(read_write_random_latency);
        }else{
        	route_operation_calls(read_write_random_throughput);
        }
    }else if(strcmp(operation,"RWS") == 0){
        printf("\nCalling read_write_sequential\n");
        if(block_size == 1){
        	route_operation_calls(read_write_sequential_latency);
        }else{
        	route_operation_calls(read_write_sequential_throughput);
        }
    }
    
	pthread_exit(NULL);
}


