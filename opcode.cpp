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

#include "opcode.h"

/*structure to store opcodes*/
struct opcode {
    std::string hex;
    std::string name;
    std::string formats;
};

/*structure to store registers*/
struct registers{
    std::string letter;
    std::string number;
};

const struct registers registers_map[] {
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

const struct opcode opcode_table [] = {
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

// Take the first 2 hex digits of the instruction as string
// Return the true opcode as string.
std::string opcode::getOpcode(std::string opcode) {
    int    secondInt;
    std::string first2Bin;
    std::string firstHex;
    std::string mod2HexBin;
    std::string secondBin;
    std::string secondHex;
    std::string translated_opcode;
    std::string translatedSecondHex;

    firstHex = opcode.substr(0,1);   // grab the first digit of the opcode
    secondHex = opcode.substr(1,1);    // grab the secondd digit of the opcode

    // secondInt = Converter::hexToInt(secondHex);             // convert the last string digit to integer
    // secondBin = Converter::hexToStringBin(secondInt);       // convert the last integer digit to 4 a binary number

    // first2Bin = (char *)secondBin.substr(0,2).c_str();   // grab the first 2 binary digits
    // mod2HexBin = strcat((char *)first2Bin.c_str(), "00"); // combine the first 2 binary digit with "00"
    // translatedSecondHex = Converter::binToHex(mod2HexBin);// convert the combined 4 binary digits to hex

    translated_opcode = strcat((char *)firstHex.c_str(),(char *)translatedSecondHex.c_str()); // combime both opcode digits after conversion

    return translated_opcode;
};