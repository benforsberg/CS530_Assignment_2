#Names: Marina Pestriacheva, Ben Forsberg, Dante Colombo-Sadeghi
#Account usernames: 
#    Marina Pestriacheva, cssc1907 
#    Ben Forsberg, cssc1930
#   Dante Colombo-Sadeghi, cssc1911
#Class information: CS 530, Spring 2020
#Assignment #2, DXE Disassembler for XE computer
#Filename of this file: makefile

CC = g++
CFLAGS = -c

dxe : main.o opcode.o
	${CC} opcode.o main.o -o dxe

opcode.o : opcode.cpp opcode.h
	${CC} ${CFLAGS} -c opcode.cpp
	
main.o : main.cpp
	${CC} ${CFLAGS} -c main.cpp

clean:
	rm -f dxe *.o