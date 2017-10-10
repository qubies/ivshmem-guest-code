#ifndef NAHANNI
#define NAHANNI

typedef struct {
	char *filePath;
	int fd;
	int size;
	void *Memory;
	void *page1;
	void *page2;
	void *page3;
} Nahanni;


//create a new Nahanni Accessor
Nahanni *NewNahanni(char *filePath, int length);

//auto free the nahanni structure
void freeNahanni(Nahanni *NN);

#endif
