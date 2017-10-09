#ifndef GENERAL
#define GENERAL
#define DEBUG
#define ERRPRINT

void debugPrint(const char *str, ...);

void errPrint(const char *str, ...);

void *allocationCheck(void *val);

void strFree(char *SP);

#endif
