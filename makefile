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