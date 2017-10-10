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
void add10M(int *L, int *I) {
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
	
	*I = 0; //initialize the counter too ... because its prettier. Its not really needed
	*L = 1; //initialise the waiter to stall
	add10M(L,I);
	lock = sem_open("/SEMA1\0", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	printf("The after Value:%d\n", *I);

}

void wait(Nahanni *NN, int *L, int *I) {
	*L = 0; //initialise the waiter to stall
	*I = 0; //initialize the counter too ... because its prettier. Its not really needed
	lock = sem_open("/SEMA1\0",0);
	add10M(L,I);
	printf("The after Value:%d\n", *I);
}

int main (int argc, char*argv[]) {
	if (argc != 4) {
		errPrint("Usage file, size, bool start\n");
		exit(EXIT_FAILURE);
	}
	Nahanni *NN = NewNahanni(argv[1], atoi(argv[2])); //make the Nahanni.
	lock = (sem_t *) &NN->Memory;
	int *ints = (int *) &(((sem_t *)NN->Memory)[1]);//move to the end of the semaphore, and make some ints!

	int *I = &ints[0]; 
	int *L = &ints[1];
	if (atoi(argv[3]) == 1) {
		initialize(NN, L, I);
	} else {
		wait(NN, L, I);
	}
	freeNahanni(NN);
}

