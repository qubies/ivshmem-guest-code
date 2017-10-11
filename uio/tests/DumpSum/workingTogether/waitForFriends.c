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

const static int COUNT = 1000000;
//adds 10 million onto the integer in increments of 1.
void add10M(int *Begin, int *Run, int *I) {
	while (*Begin != 1) { //cheezy sleep while it waits for the start flag to be set. 
		nanoSleep(10000);
	} 
	for (int x = 0; x < COUNT;) {
		while (*Run != 1) {} //spinLock
		*Run = 0;
		for (;;) {
			(*I)++;
			x++;
			if ((*I) % COUNT/10 == 0) {
				printf("My Number is:%d\n", *I);
				*Run = 1;
				nanoSleep(100000);
				break;
			}
		}
	}
}

void initialize(int *Begin) {
	*Begin = 1; //start the machines running
}

void wait(int *Begin) {
	*Begin = 0; //initialise the waiter to stall
}

int main (int argc, char*argv[]) {
	if (argc != 4) {
		errPrint("Usage file, size, bool start\n");
		exit(EXIT_FAILURE);
	}
	Nahanni *NN = NewNahanni(argv[1], atoi(argv[2])); //make the Nahanni.
	int *ints = (int *) NN->Memory;

	int *I = &ints[0]; 
	int *Begin = &ints[1];
	int *Run = &ints[2];
	*Run = 1;
	*I = 0;
	if (atoi(argv[3]) == 1) {
		initialize(Begin);
	} else {
		wait(Begin);
	}
	printf("The before Value:%d\n", *I);
	add10M(Begin,Run,I);
	printf("The after Value:%d\n", *I);
	freeNahanni(NN);
}

