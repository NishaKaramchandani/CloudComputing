CC=gcc
CFLAGS=-g -Wall -O2 -lpthread

all:
	$(CC) $(CFLAGS) -o MyNETBenchServer-UDP MyNETBenchServer-UDP.c
	$(CC) $(CFLAGS) -o MyNETBenchClient-UDP MyNETBenchClient-UDP.c
	$(CC) $(CFLAGS) -o MyNETBenchServer-TCP MyNETBenchServer-TCP.c
	$(CC) $(CFLAGS) -o MyNETBenchClient-TCP MyNETBenchClient-TCP.c

clean:
	$(RM) *.o MyNETBench-TCP MyNETBench-UDP
