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

struct flags {
    int x;
    int b;
    int p;
    int e;
};

struct instructionList {
    string s0;
    string s1;
    string s2;
    string s3;
    string s4;
    string s5;
    string s6;
    string s7;
    string s8;
    string s9;
};

flags extractFlags(string instr) {
    flags xbpe;
    int nibble = Opcode::hexToInt(instr.substr(2,1)); //Taking 3rd hex digit (aka the 3rd nibble)

    //Checking each bit in the nibble for 1's or 0's
    //AND'ing the bits (Starting from 8) and shifting right
    //(aka dividing by 2) to see which places have 1's or 0's
    xbpe.x = (nibble & 8) ? 1 : 0;
    xbpe.b = (nibble & 4) ? 1 : 0;
    xbpe.p = (nibble & 2) ? 1 : 0;
    xbpe.e = (nibble & 1) ? 1 : 0;
    return xbpe;
}

void printInsrcList(instructionList l) {
    std::cout << l.s0 + " " + l.s1 + " " + l.s2 + " " + l.s3 + " " + l.s4 + " " + l.s5 + " " + l.s6 + " " + l.s7 + " " + l.s8 + " " + l.s9;
}

instructionList parseInstructions(string textRec) {
    int pos = 9;  //Position in the text string
    int n;   //Length of instruction in nibbles/hex digits
    string instruction; //String to hold individual instruction
    flags xbpe;  //Struct that holds the extracted flags
    string parsedInstr[10]; //Array holding the separate instructions
    instructionList inList; //Struct that will hold instructions and be returned

    instruction = textRec.substr(pos, 6); //The first instruction
    xbpe = extractFlags(instruction);  //Taking flags of the instruction to know its format

    for (int i = 0; i < sizeof(parsedInstr); i++) {
        if (Opcode::getFormats(Opcode::getOpcode(instruction.substr(0,2))) == "1") { //Format == 1
            n = 2;
        } else if (Opcode::getFormats(Opcode::getOpcode(instruction.substr(0,2))) == "2") { //Format == 2
            n = 4;
        } else if (xbpe.e == 1) { //Format == 4
            n = 8;
        } else { //Format == 3
            n = 6;
        }
        instruction = textRec.substr(pos, n); //overlapping the instruction w/ correct length
        parsedInstr[i] = instruction; //Storing the instruction
        pos += n; //Shifting the position counter
        if (pos < textRec.length())
            instruction = textRec.substr(pos, n); //Next instruction
        else
            break;
        xbpe = extractFlags(instruction);  //Next flags
    }

    inList.s0 = parsedInstr[0];
    inList.s1 = parsedInstr[1];
    inList.s2 = parsedInstr[2];
    inList.s3 = parsedInstr[3];
    inList.s4 = parsedInstr[4];
    inList.s5 = parsedInstr[5];
    inList.s6 = parsedInstr[6];
    inList.s7 = parsedInstr[7];
    inList.s8 = parsedInstr[8];
    inList.s9 = parsedInstr[9];

    return inList;
}

int main(int argc, char *argv[]) {

    //testing getOpcode function
    cout << "getOpcode for 04: " + Opcode::getOpcode("04") + "\n";

    //testing parseInstructions
    printInsrcList(parseInstructions("T0000001E0500000320033F691017911BA0131BC0002F200A3B2FF40F102F014F0000640111"));
	
    //these two functions combined give you back the instruction given the opcode
	cout << Opcode::getInstruction(Opcode::findOpcode("34"));

	//this function Opcode::getFormats(sting: opcode) gives back formats: 1, 2, 3/4
    cout << " " + Opcode::getFormats("98");

    //testing stringToInt function that takes hex number as a string and returns int
    cout << "\n80 in hex is ";
    cout << Opcode::hexToInt("80");
	cout << " in decimal\n";

    if (argc != 2)
    {
        cout << " The format of the command should be: dxe <objFileName>.obj" << endl;
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
            //Will call Dante's function here
            //parseInstructions(record);
			
			
			 //For testing purposes these strings have data in them, they should be blank in final version
            //The following section fills in and formats the sic and lis files.
            //It will make individual strings for each part then make into one final string.

            //SIC File
            //label col 1-6
            string label = "LABEL";
            //col 7-8
            string spaceChars1 = "  ";
            //col 9
            string specialChar1 = "+";
            //op code col 10-15
            string opCodeString = "ADD";
            //space col 16 don't change
            string space = " ";
            //# @ = or space char col 17
            string addressSymbol = "@";
            //operand col 18-35 size 18
            string operand = "LISTA";
            //col 36-68 comments size 33
            string comments = "This is a comment";

            //This section pads each part with spaces
            while (label.length() < 6){
                label.append(" ");
            }
            while (spaceChars1.length() < 2){
                spaceChars1.append(" ");
            }
            while (specialChar1.length() < 1){
                specialChar1.append(" ");
            }
            while (opCodeString.length() < 6){
                opCodeString.append(" ");
            }
            while (addressSymbol.length() < 1){
                addressSymbol.append(" ");
            }
            while (operand.length() < 18){
                operand.append(" ");
            }
            while (comments.length() < 33) {
                comments.append(" ");
            }



            //writing output for LIS file

            //label col 1-4
            string location = "0000";
            //col 5-8
            string blank = "    ";
            //col 9-14
            string symbolNameLis = "LOOP";
            //col 15-16 blank
            string doublespace = "  ";
            //opcode col 17-23
            string opcodeLis = "CLEAR";
            //col 24-25 blank
            //use doublespace again
            //26-49 operand/value size 24 1st for @# or =
            string operandLis = "LISTA + LISTB - LISTC";
            //col 50-51 doublespace again
            //col 52-59+ assembled instruction get from record
            string instructionLis = "050000";

            //This section pads each part with spaces
            while (location.length() < 4){
                location.append(" ");
            }
            while (blank.length() < 4){
                blank.append(" ");
            }
            while (symbolNameLis.length() < 6){
                symbolNameLis.append(" ");
            }
            while (doublespace.length() < 2){
                doublespace.append(" ");
            }
            while (opcodeLis.length() < 7){
                opcodeLis.append(" ");
            }
            while (operandLis.length() < 24){
                operandLis.append(" ");
            }
            while (instructionLis.length() < 8){
                instructionLis.append(" ");
            }

            //full SIC output line to be written to file
            string sicOutString = label + spaceChars1 + specialChar1 + opCodeString + space + addressSymbol + operand + comments;
            //full LIS output line to be written to file
            string lisOutString = location + blank + symbolNameLis + doublespace + opcodeLis + doublespace + operandLis + doublespace + instructionLis;

            //writes to file
            sicOutput << "Testing SIC file output" << sicOutString << endl;
            lisOutput << "Testing LIS file output" << lisOutString << endl;

            //Debug
            cout << sicOutString << endl;
            cout << lisOutString << endl;

			
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


    //closes all filestreams
    objInput.close();
    symInput.close();
    sicOutput.close();
    lisOutput.close();
    return 0;
}
