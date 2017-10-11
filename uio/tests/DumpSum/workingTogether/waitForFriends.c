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

#include "Nahanni.h"
#include "generalFunctions.h"

const static int COUNT = 100000000;
//adds 10 million onto the integer in increments of 1.
void add10M(Nahanni *NN, int *I, int *runnerID) {
	for (int x = 0; x < COUNT;) {
		while (*runnerID != NN->ID) {}
			(*I)++;
			x++;
			if ((*I) % (COUNT/10) == 0) {
				printf("My Number is:%d\n", *I);
			}
			*runnerID = (*runnerID)++;
			*runnerID %= NN->numMachines;
	}
}

void initialize(int *runnerID) {
	*runnerID = 0; //start the machines running
}

void wait(int *runnerID) {
	*runnerID = -1; //initialise the waiter to stall
}

int main (int argc, char*argv[]) {
	if (argc != 5) {
		errPrint("Usage file, size, id, numMachines \n");
		exit(EXIT_FAILURE);
	}
	Nahanni *NN = NewNahanni(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4])); //make the Nahanni.
	int *ints = (int *) NN->Memory;

	int *I = &ints[0]; 
	int *runnerID = &ints[1];
	*I = 0;
	if (atoi(argv[3]) == atoi(argv[4])-1) {
		initialize(runnerID);
	} else {
		wait(runnerID);
	}
	printf("The before Value:%d\n", *I);
	add10M(NN, I, runnerID);
	printf("The after Value:%d\n", *I);
	freeNahanni(NN);
}

