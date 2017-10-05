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

int main(int argc, char ** argv){

    int fd, length=4*1024; 
    void * memptr;
    char * text_array;
    int i;

    if (argc != 3){
        printf("USAGE: helloWorld <filename> <size>\n");
        exit(-1);
    }

    printf("Opening IVSHMEM file %s\n", argv[1]); //debug printing
    
    length=atoi(argv[2])*1024*1024; //transform to MB

    printf("Mapped size is %d\n", length); //debug printing again.

    fd=open(argv[1], O_RDWR);// open the file read write mode. 

    /* With UIO the offset selects the memory region --> N * getpagesize() for the Nth memory region */
    //creating the memory map. 
    //void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
    if ((memptr = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 1 * getpagesize())) == MAP_FAILED) {
        printf("mmap failed (0x%x)\n", memptr); //checks if the mmap failed
        close (fd);
        exit (-1);
    }

    text_array=(char *)memptr; //cast the mmap region to a big ass string
    
    for (i=0; i < length/sizeof(char); i++){ //this assumes that only longs are stored in the array
        text_array[i]='\0'; // null terminators in the end 
    }
    strncpy(text_array, "Hello from IVSHMEM!\n", 20);
		

    printf("%s", text_array);

    printf("munmap is unmapping %x\n", memptr);
    munmap(memptr, length);

    close(fd);

}
