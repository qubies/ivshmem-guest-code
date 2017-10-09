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
	pthread_mutex_t lock;
} lockableInt;
//adds 10 million onto the integer in increments of 1.
void add10M(lockableInt *L, lockableInt *I) {
	for (int x = 0; x < 100000000; x++) {
		while (L->val != 1) { //cheezy sleep while it waits for the start flag to be set. 
		} 
		printf("after\n");
		pthread_mutex_lock(&(I->lock));
		(I->val)++;
		if ((I->val) % 10000000 == 0) {
			printf("My Number is:%d\n", I->val);
		}
		pthread_mutex_unlock(&(I->lock));
	}
}

void initialize(Nahanni *NN) {
	
	lockableInt *lockables = (lockableInt *)(NN->Memory);
	lockableInt *I = &lockables[0]; 
	lockableInt *L = &lockables[1]; 
	if (pthread_mutex_init(&(I->lock), NULL) != 0) {
		errPrint("Problem Initializing Mutex. Aborting.\n");
		exit(EXIT_FAILURE);
	}
	I->val = 0; //initialize the counter too ... because its prettier. Its not really needed
	L->val = 1; //initialise the waiter to stall
	printf("The before Value:%d\n", I->val);
	add10M(L,I);
	printf("The after Value:%d\n", I->val);
	freeNahanni(NN);

}

void wait(Nahanni *NN) {
	lockableInt *lockables = (lockableInt *)(NN->Memory);
	lockableInt *I = &lockables[0]; 
	lockableInt *L = &lockables[1]; 
	L->val = 0; //initialise the waiter to stall
	I->val = 0; //initialize the counter too ... because its prettier. Its not really needed
	printf("The before Value:%d\n", I->val);
	add10M(L,I);
	printf("The after Value:%d\n", I->val);
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


