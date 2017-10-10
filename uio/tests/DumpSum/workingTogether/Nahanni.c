#include <string.h>
#include "Nahanni.h"
#include "generalFunctions.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
Nahanni *NewNahanni(char *filePath, int length) {
	//create the main structure
	Nahanni *NN = allocationCheck(malloc(sizeof(Nahanni)));
	//build the filePath
	NN->filePath = allocationCheck(calloc(strlen(filePath)+1, sizeof(char))); 
	NN->filePath = strndup(filePath, strlen(filePath));
	//define the size in bytes
	NN->size = length*1024*1024;
	//open the file
	NN->fd = -1;
	NN->Memory = NULL;
	NN->fd = open(filePath, O_RDWR);// open the file read write mode. 
	if (NN->fd <= 0) {
		errPrint("Could not open SHM file");
		exit(EXIT_FAILURE);
	}
	//creating the memory map
	// With UIO the offset selects the memory region --> N * getpagesize() for the Nth memory region 
	if ((NN->Memory = mmap(NULL, NN->size, PROT_READ|PROT_WRITE, MAP_SHARED, NN->fd, getpagesize())) == MAP_FAILED) {
		errPrint("mmap failed\n"); //checks if the mmap failed
		close(NN->fd);
		exit(EXIT_FAILURE);
	}
	return NN;
}

void freeNahanni(Nahanni *NN) {
	strFree(NN->filePath);
	close(NN->fd);
	free(NN);
}


