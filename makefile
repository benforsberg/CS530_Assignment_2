CC = g++
CFLAGS = -g
LDFLAGS = -lm

dxe : main.o opcode.o
	${CC} ${CFLAGS} OpGen.o Converter.o SicDisassembler.o main.o ${LDFLAGS} -o xed

opcode.o : OpGen.cpp
	${CC} ${CFLAGS} -c opcode.cpp
	
main.o : main.cpp
	${CC} ${CFLAGS} -c main.cpp

clean: 
rm -f dxe *.o