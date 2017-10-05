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
    long * long_array;
    int i;
    SHA_CTX context;
    char md[20];
    memset(md,0,20);

    if (argc != 3){
        printf("USAGE: dump <filename> <size>\n");
        exit(-1);
    }

    printf("[DUMP] opening file %s\n", argv[1]); //debug printing
    length=atoi(argv[2])*1024*1024; //transform to MB

    printf("[DUMP] size is %d\n", length); //debug printing again.

    fd=open(argv[1], O_RDWR);// open the file read write mode. 

    /* With UIO the offset selects the memory region --> N * getpagesize() for the Nth memory region */
    //creating the memory map. 
    //void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
    if ((memptr = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 1 * getpagesize())) == MAP_FAILED) {
        printf("mmap failed (0x%x)\n", memptr); //checks if the mmap failed
        close (fd);
        exit (-1);
    }

    srand(time());// seeding the rand
    
    long_array=(long *)memptr; //cast the mmap region to an array of longs. 
    
    for (i=0; i < length/sizeof(long); i++){ //this assumes that only longs are stored in the array
        long_array[i]=rand(); //set a bunch of crappy random values in the long_array
    }


    SHA1_Init(&context); //initialize the sha1 hash
    SHA1_Update(&context,memptr,length); // point the hash at the 
    SHA1_Final(md,&context); 

    printf("[DUMP] ");
    for(i = 0; i < SHA_DIGEST_LENGTH; ++i )
    {
        unsigned char c = md[i];
        printf("%2.2x",c);
    }
    printf("\n");

    printf("munmap is unmapping %x\n", memptr);
    munmap(memptr, length);

    close(fd);

}
