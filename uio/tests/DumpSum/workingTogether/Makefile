#/**********************************************************************
#* Tobias Renwick SN# 1440558                                          *
#* Nahanni Makefile                                                    *
#* Supervisor's Name: Dr. Cam MacDonnel                                *
#* Created: October 9, 2017                                            *
#* Modified: October 9, 2017                                           *
#***********************************************************************/

CC = gcc
CFLAGS= -c -g -std=c99 -D_POSIX_C_SOURCE=200809L -D_BSD_SOURCE -Wall -pedantic
LDFLAGS= -g

.PHONY: all clean test

all:  

wait: wait.o nahanni.o generalFunctions.o 
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

wait.o: waitForFriends.c waitForFriends.h generalFunctions.h
	$(CC) $(CFLAGS) $< generalFunctions.h

nahanni.o: Nahanni.c Nahanni.h generalFunctions.h
	$(CC) $(CFLAGS) $< generalFunctions.h

generalFunctions.o: generalFunctions.c generalFunctions.h
	$(CC) $(CFLAGS) $< 

clean:
	$(RM) wait wait.o nahanni.o generalFunctions.o 

test: tests.c generalFunctions.c
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	valgrind --vgdb=yes ./test
