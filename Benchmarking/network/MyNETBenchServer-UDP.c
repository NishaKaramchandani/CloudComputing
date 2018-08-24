#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

int block_size;
int initial_block_size = 1000;
long long total_size = 1000000000;

pthread_mutex_t mutex;

//clock_t iteration_start, iteration_end;

struct timeval tv;

time_t iteration_start, iteration_end;

double time_taken;

int no_of_threads;

char * connection_type = NULL;

int ports[]={3000, 3001, 3021, 3031, 3005, 3006, 3007, 3008};

void call_UDP_server_throughput(void *args){
	int udpSocket, nBytes;
	char *string;
	struct sockaddr_in serverAddr, clientAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size, client_addr_size;
	long thread_id = (long)args;
  
	string = (char *) malloc(sizeof(char) * block_size);
 
	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(ports[thread_id]);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	addr_size = sizeof serverStorage;

	while((nBytes = recvfrom(udpSocket,string, initial_block_size,0,(struct sockaddr *)&serverStorage, &addr_size))>0){
		//printf("Received from client for iteration: %d\n",string);
		sendto(udpSocket,string, initial_block_size, 0, (struct sockaddr *)&serverStorage, addr_size);
	}
	close(udpSocket);
}

void call_UDP_server_latency(void *args){
	int udpSocket, nBytes;
	char *string;
	struct sockaddr_in serverAddr, clientAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size, client_addr_size;
	long thread_id = (long)args;
  
	string = (char *) malloc(sizeof(char) * block_size);
 
	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(ports[thread_id]);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	addr_size = sizeof serverStorage;

	while((nBytes = recvfrom(udpSocket,string, block_size,0,(struct sockaddr *)&serverStorage, &addr_size))>0){
		//printf("Received from client: %s\n",string);
		sendto(udpSocket,string, block_size, 0, (struct sockaddr *)&serverStorage, addr_size);
	}
	close(udpSocket);
}

void create_threads_call_server(void * function_name){
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
        if(strcmp(connection_type,"UDP")==0){
        	if(block_size == 1){
        		create_threads_call_server(call_UDP_server_latency);
        	}else{
        		create_threads_call_server(call_UDP_server_throughput);
        	}
        }else{
        	printf("Invalid connection type!");
        }
        pthread_exit(NULL);
}

