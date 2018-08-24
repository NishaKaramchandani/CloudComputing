// Server side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8081

pthread_mutex_t mutex;

//clock_t iteration_start, iteration_end;

struct timeval tv;

time_t iteration_start, iteration_end;

double time_taken;

int no_of_threads;

int block_size = 1;

long long total_size = 1000000000;

char * connection_type = NULL;

int ports[]={3000, 3001, 3002, 3003};

void call_TCP_server(void *args){
    int server_fd, new_socket, valread, n;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    char stringrecieved[block_size];
    long thread_id = (long)args;
      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(ports[thread_id]);
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    int i = 0;
    
	while((valread = recv( new_socket , stringrecieved, block_size, 0)) > 0){
    	send(new_socket , stringrecieved , block_size, 0 );
    	//printf("Read for iteration:: %d", i++);
    }
    	
 	close(server_fd);
}

void create_threads_call_server(){
		pthread_t threads[no_of_threads]; long tid; int rc;
        pthread_mutex_init(&mutex, NULL);
		gettimeofday(&tv, NULL); 
		double operations_per_second;
		double giga_ops;
 		iteration_start = tv.tv_sec;
        for (tid = 0; tid < no_of_threads; tid++) {
                rc = pthread_create(&threads[tid], NULL, call_TCP_server, (void *) tid);
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
        	create_threads_call_server();
        }else{
        	printf("Invalid connection type!");
        }
        pthread_exit(NULL);
}

