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

typedef struct {
	int val;
	pthread_spinlock_t lock;
}lockableInt;

//adds 10 million onto the integer in increments of 1.
void add10M(lockableInt **ints) {
	while (ints[1]->val != 1) { //cheezy sleep while it waits for the start flag to be set. 
	} 
	printf("starting...\n");
	for (int x = 0; x < 100000000; x++) {
		pthread_spin_lock(&(ints[0]->lock));
		(ints[0]->val)++;
		if ((ints[0]->val) % 10000000 == 0) {
			printf("My Number is:%d\n", ints[0]->val);
		}
		pthread_spin_unlock(&(ints[0]->lock));
	}
}

void initialize(lockableInt **ints) {
	
	if (pthread_spin_init(&(ints[0]->lock), 1) != 0) {
		errPrint("Problem Initializing Semaphore. Aborting.\n");
		exit(EXIT_FAILURE);
	}
	ints[0]->val = 0;
	ints[1]->val = 1;

}

void wait(lockableInt **ints) {
	ints[0]->val = 0; //initialise the waiter to stall
	ints[1]->val = 0; //initialize the counter too ... because its prettier. Its not really needed
}

int main (int argc, char*argv[]) {
	if (argc != 4) {
		errPrint("Usage file, size, bool start\n");
		exit(EXIT_FAILURE);
	}
	Nahanni *NN = NewNahanni(argv[1], atoi(argv[2])); //make the Nahanni.
	
	lockableInt **ints = (lockableInt **) NN->Memory;

	if (atoi(argv[3]) == 1) {
		initialize(ints);
	} else {
		wait(ints);
	}
	printf("The before Value:%d\n", ints[0]->val);
	add10M(ints);
	printf("The after Value:%d\n", ints[0]->val);
	freeNahanni(NN);
}

