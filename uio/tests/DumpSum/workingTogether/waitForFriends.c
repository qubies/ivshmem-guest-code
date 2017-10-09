#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#include "Nahanni.h"
#include "generalFunctions.h"


//adds 10 million onto the integer in increments of 1.
void add10M(int *L, int *I, pthread_mutex_t *mutex) {
	for (int x = 0; x < 1000000000; x++) {
		while (*L != 1) { //cheezy sleep while it waits for the start flag to be set. 
		} 
		pthread_mutex_lock(mutex);
		(*I)++;
		if ((*I) % 100000000 == 0) {
			printf("My Number is:%d\n", *I);
		}
		pthread_mutex_unlock(mutex);
	}
}

void initialize(Nahanni *NN) {
	pthread_mutex_t *mutex = (pthread_mutex_t *)&NN->Memory[200]; //initialize the mutex at 1000
	
	if (pthread_mutex_init(mutex, NULL) != 0) {
		errPrint("Problem Initializing Mutex. Aborting.\n");
		exit(EXIT_FAILURE);
	}
	
	int *I = (int *) &NN->Memory[100]; //lol....
	int *L = (int *) &NN->Memory[0]; //init is at the start...
	*I = 0; // start I out as 0
	*L = 1; //start the others coutning
	printf("The before Value:%d\n", *I);
	add10M(L,I,mutex);
	printf("The after Value:%d\n", *I);
	freeNahanni(NN);
}

void wait(Nahanni *NN) {
	pthread_mutex_t *mutex = (pthread_mutex_t *)&NN->Memory[200]; //initialize the mutex at 1000
	int *I = (int *) &NN->Memory[100]; //lol....
	int *L = (int *) &NN->Memory[0]; //init is at the start...
	(*L) = 0; //initialise the waiter to stall
	(*I) = 0; //initialize the counter too ... because its prettier. Its not really needed
	printf("The before Value:%d\n", *I);
	add10M(L,I,mutex);
	printf("The after Value:%d\n", *I);
	freeNahanni(NN);
}
int main (int argc, char*argv[]) {
	if (argc != 4) {
		errPrint("Usage file, size, bool start\n");
		exit(EXIT_FAILURE);
	}
	Nahanni *NN = NewNahanni(argv[1], atoi(argv[2])); //make the Nahanni.
	if (atoi(argv[3]) == 1) {
		initialize(NN);
	} else {
		wait(NN);
	}
}


