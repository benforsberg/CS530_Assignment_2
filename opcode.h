#ifndef OPCODE_H
#define    OPCODE_H

#include <stdio.h>
#include <string>

using namespace std;
string getOpcode(std::string opcodes);
class Opcode {
public:
    static int findOpcode(string value); //finds index of a opcode in opcode table
    string getOpcode(string opcodes);
    // static string getRegister(string registerHex);
    // static string getFormat(int optabIndex);
    static string getInstruction(int optabIndex);

    static string getFormats(string opcode);
};


#endif
