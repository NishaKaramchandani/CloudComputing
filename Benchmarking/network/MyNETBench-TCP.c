#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 3000 /*port*/
#define N_BYTES 1000000000
long block_size;
int th;

void server()
{
	struct sockaddr_in saddr, caddr;
	char buf[block_size];
	int fd1, fd2, n;
	socklen_t len;
	pid_t childpid;

	fd1 = socket (AF_INET, SOCK_STREAM, 0);

	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(PORT);
	saddr.sin_family = AF_INET;

	bind (fd1, (struct sockaddr *) &saddr, sizeof(saddr));

	listen (fd1, 10);

	len = sizeof(caddr);
	fd2 = accept (fd1, (struct sockaddr *) &caddr, &len);
	printf("%s\n","Received request...");

	while ( (n = recv(fd2, buf, block_size,0)) > 0)  {
		send(fd2, buf, block_size, 0);
	}
	if (n < 0) {
		perror("Read error");
		exit(1);
	}
	close(fd2);
	close(fd1);
}
void client()
{
	int sockfd;
	struct sockaddr_in servaddr;
	char rec[block_size];
	char buf[block_size], *src;
	src = (char *) malloc(sizeof(char) * N_BYTES);

	//basic check of the arguments
	//additional checks can be inserted

	//Create a socket for the client
	//If sockfd<0 there was an error in the creation of the socket
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
		perror("Problem in creating the socket");
		exit(2);
	}

	//Creation of the socket
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr= inet_addr("127.0.0.1");
	servaddr.sin_port =  htons(PORT); //convert to big-endian order

	//Connection of the client to the socket
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
		perror("Problem in connecting to the server");
		exit(3);
	}

	for(int i = 0; i < N_BYTES; i += block_size)
	{
		send(sockfd, buf, block_size, 0);
		if (recv(sockfd, rec, block_size,0) == 0){
			//error: server terminated prematurely
			perror("The server terminated prematurely");
			exit(4);
		}
	}
	//for(int j = 0; j < loop_value; j++)
	//{
		/*long bs = block_size;
		for(int i = 0; i < N_BYTES; i += bs)
		{
			if(i+bs > N_BYTES)
			{
				bs = N_BYTES - i;
				//printf("%ld\n", bs);
			}
			memcpy(buf, &src[i], bs);
			send(sockfd, buf, strlen(buf), 0);
			if (recv(sockfd, recvline, block_size,0) == 0){
				//error: server terminated prematurely
				perror("The server terminated prematurely");
				exit(4);
			}
		}*/
	//	}
	//}
	/*while (fgets(sendline, MAXLINE, stdin) != NULL) {

		send(sockfd, sendline, strlen(sendline), 0);

		if (recv(sockfd, recvline, MAXLINE,0) == 0){
			//error: server terminated prematurely
			perror("The server terminated prematurely");
			exit(4);
		}
		printf("%s", "String received from the server: ");
		fputs(recvline, stdout);
	}*/
}
int main (int argc, char **argv)
{
		FILE    *input;
		char proto[20];
		char line[256];
		int i = 0, res;
		pthread_t threads[4];
		clock_t begin, end;
		input = fopen(argv[1], "r");
		if (input == NULL)
		{
				printf("\nError opening file\n");
				exit (1);
		}
		while(fgets(line, sizeof(line), input) != NULL)
		{
				switch(i)
				{
						case 0:

								sscanf(line, "%s", proto);
								i++;
								break;
						case 1:
								sscanf(line, "%ld", &block_size);
								i++;
								break;
						case 2:
								sscanf(line, "%d", &th);
								break;
				}
		}
		//call_server();
		//call_client();
		double cpu_clocks;
		begin = clock();
		if(!strcmp(argv[2], "server"))
		{
			for(int i = 0; i < th; i++)
			{
					res = pthread_create(&threads[i], NULL, server, (void *) (i));
			}
			for(i = 0; i < th; i++)
			{
					res = pthread_join(threads[i], NULL);
					if(res)
					{
							printf("Could not join thread\n");
							exit(0);
					}
			}
		} else {
			for(int i = 0; i < th; i++)
			{
					res = pthread_create(&threads[i], NULL, client, (void *) (i));
			}
			for(i = 0; i < th; i++)
			{
					res = pthread_join(threads[i], NULL);
					if(res)
					{
							printf("Could not join thread\n");
							exit(0);
					}
			}
		}
		end = clock();
		cpu_clocks = (double) (end - begin) / CLOCKS_PER_SEC;
		if(strcmp(argv[2], "server"))
			printf("cpu_clocks %f\n", cpu_clocks);

		return 0;
}
