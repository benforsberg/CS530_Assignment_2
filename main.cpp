//DXE project CS 530 Spring 2020
//Ben Forsberg, cssc1930
//Marina Pestriacheva, cssc1907
//Dante Colombo-Sadeghi, cssc1911

#include "Opcode.h"
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <map>
#include <algorithm>

using namespace std;
using std::string;

/*
std::map<const char, const int> fromHex = //A map used for going from hex to int values
        {{'1', 1},
         {'2', 2},
         {'3', 3},
         {'4', 4},
         {'5', 5},
         {'6', 6},
         {'7', 7},
         {'8', 8},
         {'9', 9},
         {'A', 10},
         {'B', 11},
         {'C', 12},
         {'D', 13},
         {'E', 14},
         {'F', 15}};
*/

std::array<int, 4> extractFlags(string instr) {
    std::array<int, 4> xbpe {{0, 0, 0, 0}};
    int nibble = hexToInt(instr[2]); //Taking 3rd hex digit (aka the 3rd nibble)
    for (int i = 0; i < 4; i++) //Checking each bit in the nibble for 1's or 0's
        xbpe[i] = (nibble & (8 >> i)) ? 1 : 0; //AND'ing the bits (Starting from 8) and shifting right
    //(aka dividing by 2) to see which places have 1's or 0's
    return xbpe;
}

std::array<string, 10> parseInstructions(string textRec) {
    int pos = 9;  //Position in the text string
    int n;   //Length of instruction in nibbles/hex digits
    string instruction; //String to hold individual instruction
    std::array<int, 4> flags; //Array that holds the extracted flags
    std::array<string, 10> parsedInstr; //Array to return, holding the separate instructions

    instruction = textRec.substr(pos, 6); //The first instruction
    flags = extractFlags(instruction); //Taking flags of the instruction to know its length

    for (int i = 0; i < parsedInstr.size() - 1; i++) {
        if (flags[3] == 1) { //Format == 4
            n = 8;
            instruction = textRec.substr(pos, n); //overlapping the instruction w/ correct length
        } else {             //Format == 3
            n = 6;
            instruction = textRec.substr(pos, n);
        }
        parsedInstr[i] = instruction; //Storing the instruction
        pos += n; //Shifting the position counter
        instruction = textRec.substr(pos, n); //Next instruction
        flags = extractFlags(instruction); //Next flags
    }
    return parsedInstr;
}

int main(int argc, char *argv[]) {
	
    //these two functions
	cout << Opcode::getInstruction(Opcode::findOpcode("34"));
    //cout << "Hello World!";
	

    if (argc != 2)
    {
        cout << "Usage: dxe <objFileName>.obj" << endl;
        exit (1);
    }
    string objFileName = argv[1];
    string symFileName;
    string sicFileName;
    string lisFileName;


    //removes obj file's extension and creates the filenames for the sym, sic, and lis files
    string fileNameNoExtension = objFileName.substr(0, objFileName.find_last_of("."));
    string sicExt = fileNameNoExtension;
    string lisExt = fileNameNoExtension;
    symFileName = fileNameNoExtension.append(".sym");
    sicFileName = sicExt.append(".sic");
    lisFileName = lisExt.append(".lis");

    ifstream objInput;
    ifstream symInput;
    ofstream sicOutput((sicFileName).c_str());
    ofstream lisOutput((lisFileName).c_str());

    string record;
    string programName;
    string startingAddress;
    int count = 0;

    //opens obj and sym files to read
    objInput.open(objFileName.c_str());
    symInput.open(symFileName.c_str());

    //opens sic and lis files to write to
    sicOutput.open(sicFileName);
    lisOutput.open(lisFileName);

    //checks if files exist
    if (!objInput) {
        cerr << "Unable to open file: %s", objFileName;
        exit(1);
    }
    if (!symInput) {
        cerr << "Unable to open file: %s", symFileName;
        exit(1);
    }

    //checks for correct file extensions
    if(objFileName.substr(objFileName.find_last_of(".")) != ".obj") {
        cout << "Couldn't find obj file!" << endl;
        exit(1);
    }
    if(symFileName.substr(symFileName.find_last_of(".")) != ".sym") {
        cout << "Couldn't find sym file!" << endl;
        exit(1);
    }

    if(objInput.peek() == istream::traits_type::eof()) {
        cout << "Obj file is empty!" << endl;
        exit(1);
    }
    if(symInput.peek() == istream::traits_type::eof()) {
        cout << "Sym file is empty!" << endl;
        exit(1);
    }

    // extracts records from the file
    //meant to do all steps of disassembly each time a new record is found
    //take each text record, break it into individual instructions, then do process on each instruction from that record
    while (objInput >> record) {
        if (count == 0 ){
            programName = record;
            programName = programName.substr (1);
            cout << "Program Name is: " << programName << endl;
        }

        count++;
        //avoids counting starting address in header as a separate record
        if (count == 2) {
            startingAddress = record;
            continue;
        }

        //reads first char of each line to determine record type
        //need to add logic for each type in its respective if statement
        if (record.at(0) == 'H') {
            cout << "Found a header record" << endl;
        }

        if (record.at(0) == 'T') {
            cout << "Found a text record" << endl;
            //determineflags()
            //the record will be broken down into individual instructions, with size depending on ext flag status
        }

        if (record.at(0) == 'M') {
            cout << "Found a modification record" << endl;
        }

        if (record.at(0) == 'E') {
            cout << "Found an end record" << endl;
        }
        cout << record << endl;

    }

    //debug
    cout <<"Starting address of program " << programName <<" is " << startingAddress << endl;
    //testing access to sym file
    //string symData;
    //cout <<"\nSym file contents:" << endl;
    //while ( !symInput.eof() )
    //{
    //    getline(symInput,symData);
    //    cout << "" << symData << "\n" ;
    //}

    //closes all filestreams
    objInput.close();
    symInput.close();
    sicOutput.close();
    lisOutput.close();
    return 0;
}
