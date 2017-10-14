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

pthread_spinlock_t *lock;
//adds 10 million onto the integer in increments of 1.
void add10M(int *L, int *I) {
	while (*L != 1) { //cheezy sleep while it waits for the start flag to be set. 
	} 
	printf("starting...\n");
	for (int x = 0; x < 100000000; x++) {
		if (pthread_spin_lock(lock) != 0)  {
			perror("Sem Wait Failed.\n");
		}
		(*I)++;
		if ((*I) % 10000000 == 0) {
			printf("My Number is:%d\n", *I);
		}
		if (pthread_spin_unlock(lock) != 0) {
			perror("Sem Post Failed.\n");
		}
	}
}

void initialize(Nahanni *NN, int *L, int *I) {
	
	if (pthread_spin_init(lock, 1) != 0) {
		errPrint("Problem Initializing Semaphore. Aborting.\n");
		exit(EXIT_FAILURE);
	}
	*I = 0; //initialize the counter too ... because its prettier. Its not really needed
	*L = 1; //initialise the waiter to start

}

void wait(Nahanni *NN, int *L, int *I) {
	*L = 0; //initialise the waiter to stall
	*I = 0; //initialize the counter too ... because its prettier. Its not really needed
}

int main (int argc, char*argv[]) {
	if (argc != 4) {
		errPrint("Usage file, size, bool start\n");
		exit(EXIT_FAILURE);
	}
	Nahanni *NN = NewNahanni(argv[1], atoi(argv[2])); //make the Nahanni.
	lock = (pthread_spinlock_t *) &NN->Memory;
	int *ints = (int *) &(((pthread_spinlock_t *)NN->Memory)[1]);

	int *I = &ints[0]; 
	int *L = &ints[1];
	if (atoi(argv[3]) == 1) {
		initialize(NN, L, I);
	} else {
		wait(NN, L, I);
	}
	printf("The before Value:%d\n", *I);
	add10M(L,I);
	printf("The after Value:%d\n", *I);
	freeNahanni(NN);
}

