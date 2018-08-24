#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

long long total_size = 10000000000;
long long latency_total_size = 1000000000;
long long block_size;
int no_of_threads;
char * operation = NULL;

void write_for_read(char *filename, long long size_to_write){
	char *source;
	long long i = 0;
	source = (char *)malloc(block_size);
	memset(source, 'a', block_size*sizeof(char));
	FILE *fp;
    fp = fopen (filename, "w+");
    if (fp == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
    for(i = 0;i < size_to_write/block_size;i++){
     	fwrite(source, block_size, sizeof(char), fp);
    }
}

void *read_sequential_throughput(){
	char *destination;
	long long i = 0;
	destination = (char *)malloc(block_size);
	FILE *fp;
   
    fp = fopen ("/tmp/sequentialWriteFile.txt", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
	for(i = 0;i < total_size/no_of_threads;i+=block_size){
    	//fseek(fp, i, SEEK_SET);
     	fread(destination, block_size, sizeof(char), fp);
     }
}

void *read_random_throughput(){
	char *destination;
	destination = (char *)malloc(block_size);
	long long i = 0, index = 0;
	FILE *fp;
    fp = fopen ("/tmp/randomWriteFile.txt", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
    
    for(i = 0;i < total_size/no_of_threads;i+=block_size){
 		index = random()%(total_size);
    	fseek(fp, index, SEEK_SET);
     	fread(destination, block_size, sizeof(char), fp);
    }
}

void *write_sequential_throughput(void *args){
	char *source;
	long long i = 0;
	source = (char *)malloc(block_size);
	memset(source, 'a', block_size*sizeof(char));
	FILE *fp;
    fp = fopen ("/tmp/sequentialWriteFile.txt", "w+");
    if (fp == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
    for(i = 0;i < total_size/(no_of_threads*block_size);i++){
     	fwrite(source, block_size, sizeof(char), fp);
    }
}

void *write_random_throughput(){
	char *source;
	long long i = 0;
	long long index = 0;
	source = (char *)malloc(block_size);
	memset(source, 'a', block_size*sizeof(char));
	FILE *fp;
   
    fp = fopen ("/tmp/randomWriteFile.txt", "w+");
    if (fp == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
   
    for(i = 0;i < total_size/block_size;i++){
		index = random()%(total_size);
    	fseek(fp, index, SEEK_SET);
     	fwrite(source, block_size, sizeof(char), fp);
    }
    printf("Value of i after for loop %lld",i);
    
}

void *read_random_latency(){
	char *destination;
	destination = (char *)malloc(block_size);
	long long i = 0, index = 0;
	FILE *fp;
    fp = fopen ("/tmp/randomWriteFile.txt", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
    
    for(i = 0;i < latency_total_size/no_of_threads;i+=block_size){
 		index = random()%(latency_total_size);
    	fseek(fp, index, SEEK_SET);
     	fread(destination, block_size, sizeof(char), fp);
    }
}

void *write_random_latency(){
	char *source;
	long long i = 0;
	long long index = 0;
	source = (char *)malloc(block_size);
	memset(source, 'a', block_size*sizeof(char));
	FILE *fp;
   
    fp = fopen ("/tmp/randomWriteFile.txt", "w+");
    if (fp == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
   
    for(i = 0;i < latency_total_size/block_size;i++){
		index = random()%(latency_total_size);
    	fseek(fp, index, SEEK_SET);
     	fwrite(source, block_size, sizeof(char), fp);
    }
    printf("Value of i after for loop %lld",i);
    
}


void route_operation_calls(void * function_name){
	struct timeval tv;
	pthread_mutex_t mutex;
	double time_ellapsed;
	double latency_milliseconds;
	double throughput, latency_IOPS;
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
    time_ellapsed = (double)(iteration_end - iteration_start);
    printf("\nTime taken:: %f\n", time_ellapsed);
    FILE * fp;
    if(block_size == 1){
    	latency_milliseconds = (time_ellapsed/latency_total_size)*1000;
    	latency_IOPS = (latency_total_size/1000);
    	fp = fopen("./output/MyDiskBenchLatency.out.dat", "a+");
    	fprintf(fp, "\n%s\t%d\t%lld\t%f\t%f", operation, no_of_threads, block_size, latency_milliseconds, latency_IOPS);
    }else{
    	throughput = (total_size/time_ellapsed)/1000000;
    	fp = fopen("./output/MyDiskBenchThroughput.out.dat", "a+");
    	fprintf(fp, "\n%s\t%d\t%lld\t%f", operation, no_of_threads, block_size, throughput);
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
    if (fp == NULL){
    	printf("\nError reading parameters file. Please check file name.\n");
        exit(EXIT_FAILURE);
    }

    if(getline(&operation, &len, fp) != -1) {
    		strtok(operation, "\n");
            printf("\nValue read for Operation: %s", operation);
    }


    if(getline(&block_data, &len, fp) != -1) {
    	strtok(block_data, "\n");
        block_size = atoi(block_data)*1000;
        printf("\nValue read for block_size: %lld", block_size);
    }
    
    if(getline(&threads, &len, fp) != -1) {
    	strtok(threads, "\n");
        no_of_threads = atoi(threads);
        printf("\nValue read for threads: %d", no_of_threads);
    }

    fclose(fp);
    if(block_size == 1){
    	if(strcmp(operation,"RR") == 0){
        	printf("\nCalling read_random\n");
        	write_for_read("/tmp/randomWriteFile.txt", latency_total_size);
        	route_operation_calls(read_random_latency);
    	}else if(strcmp(operation,"WR") == 0){
        	printf("\nCalling write_random\n");
        	route_operation_calls(write_random_latency);
    	}
    }else{
    	if(strcmp(operation,"RR") == 0){
        	printf("\nCalling read_random\n");
        	write_for_read("/tmp/randomWriteFile.txt", total_size);
        	route_operation_calls(read_random_throughput);
    	}else if(strcmp(operation,"RS") == 0){
        	printf("\nCalling read_sequential\n");
        	write_for_read("/tmp/sequentialWriteFile.txt", total_size);
        	route_operation_calls(read_sequential_throughput);
    	}else if(strcmp(operation,"WR") == 0){
        	printf("\nCalling write_random\n");
        	route_operation_calls(write_random_throughput);
    	}else if(strcmp(operation,"WS") == 0){
        	printf("\nCalling write_sequential\n");
        	route_operation_calls(write_sequential_throughput);
    	}
    }
	pthread_exit(NULL);
}

