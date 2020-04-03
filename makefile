CC = g++
CFLAGS = -c

dxe : main.o Opcode.o
	${CC} Opcode.o main.o -o dxe

Opcode.o : Opcode.cpp Opcode.h
	${CC} ${CFLAGS} -c opcode.cpp
	
main.o : main.cpp
	${CC} ${CFLAGS} -c main.cpp

clean:
	rm -f dxe *.o