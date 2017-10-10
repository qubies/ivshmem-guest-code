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
#include <semaphore.h>

#include "Nahanni.h"
#include "generalFunctions.h"

sem_t *lock;
//adds 10 million onto the integer in increments of 1.
void add10M(int *L, int *I, sem_t *lock) {
	while (*L != 1) { //cheezy sleep while it waits for the start flag to be set. 
	} 
	for (int x = 0; x < 100000000; x++) {
		if (sem_wait(lock) != 0)  {
			perror("Sem Wait Failed.\n");
		}
		(*I)++;
		if ((*I) % 10000000 == 0) {
			printf("My Number is:%d\n", *I);
		}
		if (sem_post(lock) != 0) {
			perror("Sem Post Failed.\n");
		}
	}
}

void initialize(Nahanni *NN, int *L, int *I) {
	
	if (sem_init(lock, 1, 1) != 0) {
		errPrint("Problem Initializing Semaphore. Aborting.\n");
		exit(EXIT_FAILURE);
	}
	*I = 0; //initialize the counter too ... because its prettier. Its not really needed
	*L = 1; //initialise the waiter to stall

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
	int *I = (int *) &NN->page1; 
	int *L = (int *) &NN->page2; 
	lock = (sem_t *) &NN->page3;
	if (atoi(argv[3]) == 1) {
		initialize(NN, L, I);
	} else {
		wait(NN, L, I);
	}
	printf("The before Value:%d\n", *I);
	add10M(L,I,lock);
	printf("The after Value:%d\n", *I);
	freeNahanni(NN);
}

