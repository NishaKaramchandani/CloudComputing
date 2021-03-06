// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8081

pthread_mutex_t mutex;

struct timeval tv;

time_t iteration_start, iteration_end;
  
long long total_size = 1000000000;

int block_size;

char * connection_type = NULL;

int no_of_threads;

double time_taken;

int ports[]={3000, 3001, 3002, 3003};

void call_TCP_client(void *args){
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    char stringreceived[block_size];
	char stringsent[block_size], *src;
	long thread_id = (long)args;
	
	src = (char *) malloc(sizeof(char) * block_size);
	memset(src, 'a', block_size*sizeof(char));
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Socket creation error \n");
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ports[thread_id]);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\nConnection Failed \n");
        
    }
    for(int j = 0; j < 100; j++){
    	for(int i = 0; i < total_size/no_of_threads; i += block_size){
    		//printf("Sending data for iteration:: %d", i);
			send(sock , src , block_size , 0 );
			recv( sock , stringreceived, block_size, 0);
   		}
   }
}

void create_threads_call_client(){
		pthread_t threads[no_of_threads]; long tid; int rc;
        pthread_mutex_init(&mutex, NULL);
		gettimeofday(&tv, NULL); 
		double operations_per_second;
		double giga_ops;
 		iteration_start = tv.tv_sec;
        for (tid = 0; tid < no_of_threads; tid++) {
                rc = pthread_create(&threads[tid], NULL, call_TCP_client, (void *) tid);
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
        printf("Time taken:: %f", time_taken);
        pthread_mutex_destroy(&mutex);
}

int main(int argc, char **argv)
{
        FILE * fp;
        size_t len = 0;
        char * threads = NULL;
        char * block_value = NULL;

        fp = fopen(argv[1], "r");
        if (fp == NULL)
                exit(EXIT_FAILURE);

        if(getline(&connection_type, &len, fp) != -1) {
                strtok(connection_type, "\n");
                printf("Connection Type: %s\n", connection_type);
        }

		if(getline(&block_value, &len, fp) != -1) {
                strtok(block_value, "\n");
            	block_size = atoi(block_value);
                printf("Block Size: %d\n", block_size);
        }

        if(getline(&threads, &len, fp) != -1) {
                no_of_threads = atoi(threads);
                printf("\nNumber of threads: %d\n", no_of_threads);
        }

        fclose(fp);
        if(strcmp(connection_type,"TCP")==0){
        	create_threads_call_client();
        }else{
        	printf("Invalid connection type!");
        }
        pthread_exit(NULL);
}