#ifndef NAHANNI
#define NAHANNI

typedef struct {
	char *filePath;
	int fd;
	int size;
	int numMachines;
	int ID;
	void *Memory;

} Nahanni;


//create a new Nahanni Accessor
Nahanni *NewNahanni(char *filePath, int length, int id, int numMachines);

//auto free the nahanni structure
void freeNahanni(Nahanni *NN);

#endif
