#ifndef OPCODE_H
#define    OPCODE_H

#include <stdio.h>
#include <string>
#include "math.h"

using namespace std;

class Opcode {
public:
    static int findOpcode(string value); //finds index of a opcode in opcode table
    static string getOpcode(string opcodes);
    // static string getRegister(string registerHex);
    static string getInstruction(int optabIndex);
    static string getFormats(string opcode);
    static int hexToInt(string value);//takes a string of 2 (hex)charecters and returns int
    struct flags;
    struct instructionList;

};


#endif
