#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

int block_size;
int initial_block_size = 1000;
long long total_size = 1000000000;
long long total_size_latency = 1000000;

pthread_mutex_t mutex;

//clock_t iteration_start, iteration_end;

struct timeval tv;

time_t iteration_start, iteration_end;

double time_taken;

int no_of_threads;

char * connection_type = NULL;

int ports[] = {3000, 3001, 3021, 3031, 3005, 3006, 3007, 3008};

void call_UDP_client_throughput(void *args){
	int clientSocket, portNum, nBytes;
	char *string_to_send;
	char *string_to_receive;
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	long thread_id = (long)args;
	
	string_to_send = (char *) malloc(sizeof(char) * block_size);
	memset(string_to_send, 'a', block_size*sizeof(char));
	//printf("Size of string to send:: %d", strlen(string_to_send));
	 
	string_to_receive = (char *) malloc(sizeof(char) * block_size);
	clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(ports[thread_id]);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	addr_size = sizeof serverAddr;

	for(int z = 0; z < 100; z++){
		printf("Received from server for iteration:: %d\n",z);
		for(int k = 0; k < total_size/(block_size*no_of_threads); k++){
			for(int i = 0; i < block_size/initial_block_size; i++){
				sendto(clientSocket,string_to_send, initial_block_size,0,(struct sockaddr *)&serverAddr,addr_size);
				nBytes = recvfrom(clientSocket,string_to_receive, initial_block_size,0,NULL, NULL);
				//printf("Received from server for iteration:: %d\n",i);
			}
		}
	}
}

void call_UDP_client_latency(void *args){
	int clientSocket, portNum, nBytes;
	char *string_to_send;
	char *string_to_receive;
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	long thread_id = (long)args;
	
	string_to_send = (char *) malloc(sizeof(char) * block_size);
	memset(string_to_send, 'a', block_size*sizeof(char));
	//printf("Size of string to send:: %d", strlen(string_to_send));
	 
	string_to_receive = (char *) malloc(sizeof(char) * block_size);
	clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(ports[thread_id]);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	addr_size = sizeof serverAddr;

	for(int i = 0; i < total_size_latency/no_of_threads; i++){
		printf("Received from server for iteration:: %d\n",i);
		sendto(clientSocket, string_to_send, block_size, 0, (struct sockaddr *)&serverAddr, addr_size);
		nBytes = recvfrom(clientSocket, string_to_receive, block_size, 0, NULL, NULL);
		//printf("Received from server: %s\n",string_to_receive);
	}
}

void create_threads_call_client(void * function_name){
		pthread_t threads[no_of_threads]; long tid; int rc;
        pthread_mutex_init(&mutex, NULL);
		double latency_milliseconds;
		double throughput;
		
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
        
        FILE * fp;
    	if(block_size == 1){
    		latency_milliseconds = (time_taken/total_size_latency)*1000;
    		fp = fopen("./output/MyNETBenchServerLatency-UDP.out.dat", "a+");
    		fprintf(fp, "\n%s\t%d\t%d\t%f", connection_type, no_of_threads, block_size, latency_milliseconds);
    	}else{
    		throughput = (total_size/time_taken)/1000000;
    		fp = fopen("./output/MyNETBenchServerThroughput-UDP.out.dat", "a+");
    		fprintf(fp, "\n%s\t%d\t%d\t%f", connection_type, no_of_threads, block_size, throughput);
    	}
    	fclose(fp);
    	
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
        		create_threads_call_client(call_UDP_client_latency);
        	}else{
        		create_threads_call_client(call_UDP_client_throughput);
        	}
        }else{
        	printf("Invalid connection type!");
        }
        pthread_exit(NULL);
}