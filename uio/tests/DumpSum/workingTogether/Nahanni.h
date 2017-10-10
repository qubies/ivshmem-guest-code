#ifndef NAHANNI
#define NAHANNI

typedef struct {
	char *filePath;
	int fd;
	int size;
	void *Memory;
} Nahanni;


//create a new Nahanni Accessor
Nahanni *NewNahanni(char *filePath, int length);

//auto free the nahanni structure
void freeNahanni(Nahanni *NN);

#endif
