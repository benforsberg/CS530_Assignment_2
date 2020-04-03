/*
Names: Marina Pestriacheva, Ben Forsberg, Dante Colombo-Sadeghi
Account usernames: Marina - cssc1907, Ben - cssc1930, Dante - cssc1911
Class information: CS 530, Spring 2020
Assignment #2, DXE Disassembler for XE computer
Filename of this file: README
*/

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "Opcode.h"
using namespace std;

/*structure to store opcodes*/
struct opcodes{
    std::string hex;
    std::string name;
    std::string formats;
};

/*structure to store registers*/
struct registers{
    std::string letter;
    std::string number;
};

const struct registers registers_map[] = {
    { "A", "0" },
    { "X", "1" },
    { "L", "2" },
    { "B", "3" },
    { "S", "4" },
    { "T", "5" },
    { "F", "6" },
    { "PC", "8" },
    { "SW", "9" }
};

const struct opcodes opcode_table[] = {
  {"18", "ADD", "3/4" },
  {"58", "ADDF", "3/4"},
  {"90", "ADDR", "2"},
  {"40", "AND", "3/4"},
  {"B4", "CLEAR", "2"},
  {"28", "COMP", "3/4"},
  {"88", "COMPF", "3/4"},
  {"A0", "COMPR", "2"},
  {"24", "DIV", "3/4"},
  {"64", "DIVF", "3/4"},
  {"9C", "DIVR", "2"},
  {"C4", "FIX", "1"},
  {"C0", "FLOAT", "1"},
  {"F4", "HIO", "1"},
  {"3C", "J", "3/4"},
  {"30", "JEQ", "3/4"},
  {"34", "JGT", "3/4"},
  {"38", "JLT", "3/4"},
  {"48", "JSUB", "3/4"},
  {"00", "LDA", "3/4"},
  {"68", "LDB", "3/4"},
  {"50", "LDCH", "3/4"},
  {"70", "LDF", "3/4"},
  {"08", "LDL", "3/4"},
  {"6C", "LDS", "3/4"},
  {"74", "LDT", "3/4"},
  {"04", "LDX", "3/4"},
  {"D0", "LPS", "3/4"},
  { "20", "MUL", "3/4" }, 
  {"60", "MULF", "3/4"},
  {"98", "MULR", "2"},
  {"C8", "NORM", "1"},
  {"44", "OR", "3/4"},
  {"D8", "RD", "3/4"},
  {"AC", "RMO", "2"},
  {"4C", "RSUB", "3/4"},
  {"A4", "SHIFTL", "2"},
  {"A8", "SHIFTR", "2"},
  {"F0", "SIO", "1"},
  {"EC", "SSK", "3/4"},
  {"0C", "STA", "3/4"},
  {"78", "STB", "3/4"},
  {"54", "STCH", "3/4"},
  {"80", "STF", "3/4"},
  {"D4", "STI", "3/4"},
  {"14", "STL", "3/4"},
  {"7C", "STS", "3/4"},
  {"E8", "STSW", "3/4"},
  {"84", "STT", "3/4"},
  {"10", "STX", "3/4"},
  {"1C", "SUB", "3/4"},
  {"5C", "SUBF", "3/4"},
  {"94", "SUBR", "2"},
  {"B0", "SVC", "2"},
  {"E0", "TD", "3/4"},
  {"F8", "TIO", "1"},
  {"2C", "TIX", "3/4"},
  {"B8", "TIXR", "2"},
  {"DC", "WD", "3/4"}
};

int hexToInt(string value){
    int num = 0;
    int pow16 = 1;

    string alpha = "0123456789ABCDEF";
    for(int i = value.length() - 1; i >= 0; --i)
    {
        num += alpha.find(toupper(value[i])) * pow16;
        pow16 *= 16;
    }
    return num;
};

// Take in 1 int and return Binary number as a string.
string hexToStringBin(unsigned long value) {
    if(value == 0)
        return "0000";
    else if(value == 1)
        return "0001";
    else if(value == 2)
        return "0010";
    else if(value == 3)
        return "0011";
    else if(value == 4)
        return "0100";
    else if(value == 5)
        return "0101";
    else if(value == 6)
        return "0110";
    else if(value == 7)
        return "0111";
    else if(value == 8)
        return "1000";
    else if(value == 9)
        return "1001";
    else if(value == 0x0A)
        return "1010";
    else if(value == 0x0B)
        return "1011";
    else if(value == 0x0C)
        return "1100";
    else if(value == 0x0D)
        return "1101";
    else if(value == 0x0E)
        return "1110";
    else if (value == 0x0F)
        return "1111";
    printf("Opcode Error!\n");
    exit(EXIT_FAILURE);
};

//Take in a 4 binary number as a string and return an integer (decimal)
string binToHex(string value){
    if(value.compare("0000")==0)
        return "0";
    else if(value.compare("0001")==0)
        return "1";
    else if(value.compare("0010")==0)
        return "2";
    else if(value.compare("0011")==0)
        return "3";
    else if(value.compare("0100")==0)
        return "4";
    else if(value.compare("0101")==0)
        return "5";
    else if(value.compare("0110")==0)
        return "6";
    else if(value.compare("0111")==0)
        return "7";
    else if(value.compare("1000")==0)
        return "8";
    else if(value.compare("1001")==0)
        return "9";
    else if(value.compare("1010")==0)
        return "A";
    else if(value.compare("1011")==0)
        return "B";
    else if(value.compare("1100")==0)
        return "C";
    else if(value.compare("1101")==0)
        return "D";
    else if(value.compare("1110")==0)
        return "E";
    else if(value.compare("1111")==0)
        return "F";
    printf("Invalided Input!\n");
    exit(EXIT_FAILURE);
};

// Take the first 2 hex digits of the instruction as string
// Return the true opcode as string.
string getOpcode(string opcodes) {
    int    secondInt;
    string first2Bin;
    string firstHex;
    string mod2HexBin;
    string secondBin;
    string secondHex;
    string translated_opcode;
    string translatedSecondHex;

    firstHex = opcodes.substr(0,1);   // grab the first digit of the opcode
    secondHex = opcodes.substr(1,1);    // grab the secondd digit of the opcode

    secondInt = hexToInt(secondHex);             // convert the last string digit to integer
    secondBin = hexToStringBin(secondInt);       // convert the last integer digit to 4 a binary number

    first2Bin = (char *)secondBin.substr(0,2).c_str();   // grab the first 2 binary digits
    mod2HexBin = strcat((char *)first2Bin.c_str(), "00"); // combine the first 2 binary digit with "00"
    translatedSecondHex = binToHex(mod2HexBin);// convert the combined 4 binary digits to hex

    translated_opcode = strcat((char *)firstHex.c_str(),(char *)translatedSecondHex.c_str()); // combime both opcode digits after conversion

    return translated_opcode;
   
    // return opcodes;
};

// Return index of the opcode if it is in the opcode_table. -1 if its not
int Opcode::findOpcode(string value){
    for (int i = 0; i <= 57; i++) {
        if (opcode_table[i].hex.compare(value) == 0) {
            return i;
        }
    }
    return -1;
}

// Returns the Instruction name from the opcode_table given the index from the opcode_table 
string Opcode::getInstruction(int optabIndex){
    string instructionName;
    instructionName = opcode_table[optabIndex].name;
    return instructionName;
}