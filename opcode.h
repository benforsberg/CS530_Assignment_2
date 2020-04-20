/*
Names: Marina Pestriacheva, Ben Forsberg, Dante Colombo-Sadeghi
Account usernames: 
    Marina Pestriacheva, cssc1907 
    Ben Forsberg, cssc1930
    Dante Colombo-Sadeghi, cssc1911
Class information: CS 530, Spring 2020
Assignment #2, DXE Disassembler for XE computer
Filename of this file: Opcode.h
*/
#ifndef OPCODE_H
#define    OPCODE_H

#include <stdio.h>
#include <string>
#include "math.h"

using namespace std;

class Opcode {
public:
    static int findOpcode(string value); //finds index of an opcode in the opcode table
    static string getOpcode(string opcodes); //takes a string of 2 hex charecters that is the opcode part from the machine instruction, removed flags and returns back the string with 2 hex charecters that represent the opcode from the opcodeTable
    static string getInstruction(int optabIndex); //takes the index of an opcode table and returns the instruction name as a string 
    static string getFormats(string opcode);
    static int hexToInt(string hexValue);//takes a string of 2 (hex)charecters and returns int
    struct flags;
    struct instructionList;
};


#endif
