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
	for (int x = 0; x < 10000000; x++) {
		while (*L != 1) { //cheezy sleep while it waits for the start flag to be set. 
			sleep(1);
		} 
		pthread_mutex_lock(mutex);
		*I++;
		pthread_mutex_unlock(mutex);
	}
}

void initialize(Nahanni *NN) {
	pthread_mutex_t *mutex = (pthread_mutex_t *)&NN->Memory[1000]; //initialize the mutex at 1000
	
	if (pthread_mutex_init(mutex, NULL) != 0) {
		errPrint("Problem Initializing Mutex. Aborting.\n");
		exit(EXIT_FAILURE);
	}
	
	int *I = (int *) &NN->Memory[10000]; //lol....
	int *L = (int *) &NN->Memory[0]; //init is at the start...
	*I = 0; // start I out as 0
	*L = 1; //start the others coutning
	add10M(L,I,mutex);
	printf("The Current Value:%d\n", *I);
	freeNahanni(NN);
}

void wait(Nahanni *NN) {
	pthread_mutex_t *mutex = (pthread_mutex_t *)&NN->Memory[1000]; //initialize the mutex at 1000
	int *I = (int *) &NN->Memory[10000]; //lol....
	int *L = (int *) &NN->Memory[0]; //init is at the start...
	add10M(L,I,mutex);
	printf("The Current Value:%d\n", *I);
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


