#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char ** argv){

    long size;
    char * file;
    void * map_region;
    char * text_array;
    int i,fd;

    if (argc!=3){
        fprintf(stderr, "USAGE: sum <file> <size in MB>\n");
        exit(-1);
    }

    size=atol(argv[2])*1024*1024;
 
    if ((fd=open(argv[1], O_RDWR)) < 0){
        fprintf(stderr, "ERROR: cannot open file\n");
        exit(-1);
    }

    /* With UIO drivers, the offset selects the memory region beginning at 0 and counting by page offsets */
    if ((map_region=mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 1 * getpagesize())) == MAP_FAILED){
        fprintf(stderr, "ERROR: cannot mmap file\n");
    }

    text_array = (char *) map_region;
    printf("%s", text_array);
    printf("%s", &text_array[21]);

    munmap(map_region,size);
    close(fd);
}
