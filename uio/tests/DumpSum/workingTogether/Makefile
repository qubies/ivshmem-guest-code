#/**********************************************************************
#* Tobias Renwick SN# 1440558                                          *
#* Nahanni Makefile                                                    *
#* Supervisor's Name: Dr. Cam MacDonnel                                *
#* Created: October 9, 2017                                            *
#* Modified: October 9, 2017                                           *
#***********************************************************************/

CC = gcc
CFLAGS= -g -c -std=c99 -D_POSIX_C_SOURCE=200809L -D_BSD_SOURCE 
LDFLAGS= -g
LDLIBS=-pthread
.PHONY: all clean test

wait: waitForFriends.o Nahanni.o generalFunctions.o 
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

waitForFriends.o: waitForFriends.c waitForFriends.h generalFunctions.h Nahanni.h
	$(CC) $(CFLAGS) $<

nahanni.o: Nahanni.c Nahanni.h generalFunctions.h
	$(CC) $(CFLAGS) $< generalFunctions.h

generalFunctions.o: generalFunctions.c generalFunctions.h
	$(CC) $(CFLAGS) $< 

clean:
	$(RM) wait waitForFriends.o nahanni.o generalFunctions.o 

test: tests.c generalFunctions.c
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
