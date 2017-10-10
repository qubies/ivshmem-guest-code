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

sem_t *booger;

typedef struct {
	int val;
	sem_t lock;
} lockableInt;
//adds 10 million onto the integer in increments of 1.
void add10M(lockableInt *L, lockableInt *I) {
	while (L->val != 1) { //cheezy sleep while it waits for the start flag to be set. 
	} 
	booger = sem_open("BOOGER1", 0);
	for (int x = 0; x < 100000000; x++) {
		if (sem_wait(booger) != 0)  {
			perror("Sem Wait Failed.\n");
		}
		(I->val)++;
		if ((I->val) % 10000000 == 0) {
			printf("My Number is:%d\n", I->val);
		}
		if (sem_post(booger) != 0) {
			perror("Sem Post Failed.\n");
		}
	}
}

int main (int argc, char*argv[]) {
	if (argc != 4) {
		errPrint("Usage file, size, bool start\n");
		exit(EXIT_FAILURE);
	}
	Nahanni *NN = NewNahanni(argv[1], atoi(argv[2])); //make the Nahanni.

	lockableInt *lockables = (lockableInt *)(NN->Memory);
	lockableInt *I = &lockables[0]; 
	lockableInt *L = &lockables[1]; 
	

	if (atoi(argv[3]) == 1) {
		booger = sem_open("BOOGER1", O_CREAT, 0644, 1);
		I->val = 0; //initialize the counter too ... because its prettier. Its not really needed
		L->val = 1; //initialise the waiter to stall
	} else {
		L->val = 0; //initialise the waiter to stall
		I->val = 0; //initialize the counter too ... because its prettier. Its not really needed
	}
	sem_unlink("BOOGER1");
	
	printf("The before Value:%d\n", I->val);
	
	add10M(L,I);
	
	printf("The after Value:%d\n", I->val);
	
	freeNahanni(NN);
	sem_close(booger);
}


