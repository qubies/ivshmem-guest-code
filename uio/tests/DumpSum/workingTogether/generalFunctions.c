#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>

#include "generalFunctions.h"
void debugPrint(const char *str, ...) {
#ifdef DEBUG
        va_list args;

        va_start(args, str);
        vfprintf(stderr, str, args);
        va_end(args);
#endif
}

void errPrint(const char *str, ...) {
#ifdef ERRPRINT
        va_list args;

        va_start(args, str);
        vfprintf(stderr, str, args);
        va_end(args);
#endif
}

void *allocationCheck(void *val) {
        if (val == NULL) {
                errPrint("A memory allocation (IE malloc, calloc...) Failed. EXITING.\n");
                exit(EXIT_FAILURE);
        }
        return val;
}

void strFree(char *SP) {
        if (SP != NULL) {
                free(SP);
        }
}


