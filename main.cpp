//DXE project CS 530 Spring 2020
//Ben Forsberg, cssc1930
//Marina Pestriacheva, cssc1907
//Dante Colombo-Sadeghi, cssc1911

#include "opcode.h"
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <map>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream> 


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
    //strings that keep track of each instruction's format
    string form0;
    string form1;
    string form2;
    string form3;
    string form4;
    string form5;
    string form6;
    string form7;
    string form8;
    string form9;
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

    //debug seeing what flags should be
    //cout <<"Flags for " << instr << ": "<< xbpe.x << xbpe.b<< xbpe.p<< xbpe.e << endl;

    return xbpe;
}

void printInsrcList(instructionList l) {
    std::cout << l.s0 + " " + l.s1 + " " + l.s2 + " " + l.s3 + " " + l.s4 + " " + l.s5 + " " + l.s6 + " " + l.s7 + " " + l.s8 + " " + l.s9 << endl;
}

instructionList parseInstructions(string textRec) {
    int pos = 9;  //Position in the text string
    int n;   //Length of instruction in nibbles/hex digits
    string instruction; //String to hold individual instruction
    flags xbpe;  //Struct that holds the extracted flags
    string parsedInstr[10]; //Array holding the separate instructions
    string form[10]; //Array holding the formats
    instructionList inList; //Struct that will hold instructions and be returned

    instruction = textRec.substr(pos, 6); //The first instruction
    xbpe = extractFlags(instruction);  //Taking flags of the instruction to know its format

    form[0] = "none";
    form[1] = "none";
    form[2] = "none";
    form[3] = "none";
    form[4] = "none";
    form[5] = "none";
    form[6] = "none";
    form[7] = "none";
    form[8] = "none";
    form[9] = "none";

    parsedInstr[0] = "none";
    parsedInstr[1] = "none";
    parsedInstr[2] = "none";
    parsedInstr[3] = "none";
    parsedInstr[4] = "none";
    parsedInstr[5] = "none";
    parsedInstr[6] = "none";
    parsedInstr[7] = "none";
    parsedInstr[8] = "none";
    parsedInstr[9] = "none";

    for (int i = 0; i < sizeof(parsedInstr); i++) {
        if (Opcode::getFormats(Opcode::getOpcode(instruction.substr(0,2))) == "1") { //Format == 1
            form[i] = "1";
            n = 2;
        } else if (Opcode::getFormats(Opcode::getOpcode(instruction.substr(0,2))) == "2") { //Format == 2
            form[i] = "2";
            n = 4;
        } else if (xbpe.e == 1) { //Format == 4
            form[i] = "4";
            n = 8;
        } else { //Format == 3
            form[i] = "3";
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

    inList.form0 = form[0];
    inList.form1 = form[1];
    inList.form2 = form[2];
    inList.form3 = form[3];
    inList.form4 = form[4];
    inList.form5 = form[5];
    inList.form6 = form[6];
    inList.form7 = form[7];
    inList.form8 = form[8];
    inList.form9 = form[9];

    return inList;
}

string int_to_hex( int b )
{
  std::stringstream stream;
  stream << setfill ('0') << std::setw(sizeof(int)) 
         << hex << b;
  return stream.str();
}

//function that returns the array of Loc addresses (lis files on the left)
//startAddr has 6 characters!
vector<string> addressesLoc(string startAddr, instructionList instructions) {
    string form[10];
    form[0] = instructions.form0;
    form[1] = instructions.form1;
    form[2] = instructions.form2;
    form[3] = instructions.form3;
    form[4] = instructions.form4;
    form[5] = instructions.form5;
    form[6] = instructions.form6;
    form[7] = instructions.form7;
    form[8] = instructions.form8;
    form[9] = instructions.form9;
    vector<string> addrLoc; //this will hold Loc addresses
    int intAddr = Opcode::hexToInt(startAddr);
    addrLoc.push_back(startAddr.substr(2, 6)); //loads first address 

    for (int i = 0; i<10; i++) {
    if (form[i] != "none") {
        if (form[i] == "1"){
            intAddr += 1;
            string newAddr = int_to_hex(intAddr);
            addrLoc.push_back(newAddr);
        }
        else if (form[i] == "2"){
            intAddr += 2;
            string newAddr = int_to_hex(intAddr);
            addrLoc.push_back(newAddr);
        }
        else if (form[i] == "3"){
            intAddr += 3;
            string newAddr = int_to_hex(intAddr);
            addrLoc.push_back(newAddr);
        }
        else if (form[i] == "4"){
            intAddr += 4;
            string newAddr = int_to_hex(intAddr);
            addrLoc.push_back(newAddr);
        }
    }
    }
    
    /*//printing out the loc addresses for testing purposes
    std::cout << "The contents of addrLoc:" << endl;
    for (std::vector<string>::iterator it = addrLoc.begin(); it != addrLoc.end(); ++it)
        std::cout << ' ' << *it << endl;
    std::cout << '\n';
*/
    return addrLoc;
}

//Takes in a single instruction
//Returns its displacement/address field
string extractDisplacement(string instr) {
    string disp;

    if (instr.length() == 2) //format 1
        disp = "No Displacement Field. Format 1.";
    else if (instr.length() == 4) //format 2
        disp = "No Displacement Field. Format 2.";
    else if (instr.length() == 6) //format 3
        disp = instr.substr(3,3);
    else if (instr.length() == 8) //format 4 (extended)
        disp = instr.substr(3,5);
    else
        disp = "Not a valid instruction.";

    return disp;
}

//Takes in a single instruction
//Returns the addressing mode(s)
string addressingMode(string instr) {
    string mode; //String which will be returned
    flags f = extractFlags(instr); //To see x, b, p, and e
    string opcode = instr.substr(0,2); //Storing opcode characters into a variable

    if (Opcode::getFormats(Opcode::getOpcode(opcode)) != "3/4") {//Getting formats from opcode table to see if Format == 3/4
        mode = "No addressing mode associated. Format == 1 or 2";
    } else { //Must be format 3 or 4 by this point
        int difference = Opcode::hexToInt(opcode) - Opcode::hexToInt(Opcode::getOpcode(opcode)); //Difference between "given" and "true" opcode
        if (f.e == 1) { //FORMAT == 4; Always Direct Addressing (Never Base- nor PC-relative)
            if (difference == 3) //Simple Addressing
                if (f.x == 1) //Indexing
                    mode = "Simple & Direct Addressing (Indexing).";
                else //No Indexing
                    mode = "Simple & Direct Addressing.";
            else if (difference == 2) //iNdirect Addressing
                mode = "iNdirect & Direct Addressing.";
            else  //difference == 1, Immediate Addressing
                mode = "Immediate and Direct Addressing.";
        } else { //FORMAT == 3
            if (difference == 3) { //Simple Addressing
                if (f.x == 1) { //Indexing
                    if (f.b == 1) //Base-relative
                        mode = "Simple Addressing (Indexing & Base-relative).";
                    else if (f.p == 1)  //PC-relative
                        mode = "Simple Addressing (Indexing & PC-relative).";
                    else  //Neither Base- nor PC-Relative
                        mode = "Simple & Direct Addressing (Indexing).";
                } else { //No indexing
                    if (f.b == 1)  //Base-relative
                        mode = "Simple Addressing (Base-relative).";
                    else if (f.p == 1)  //PC-relative
                        mode = "Simple Addressing (PC-relative).";
                    else  //Neither Base- nor PC-Relative
                        mode = "Simple & Direct Addressing.";
                }
            } else if (difference == 2) { //iNdirect Addressing, there is no Indexing
                if (f.b == 1) //Base-relative
                    mode = "iNdirect Addressing (Base-relative).";
                else if (f.p == 1) //PC-relative
                    mode = "iNdirect Addressing (PC-relative).";
                else //neither Base- nor PC-relative
                    mode = "iNdirect & Direct Adressing.";
            } else { //difference == 1, Immediate Addressing, there is no Indexing
                if (f.b == 1) //Base-relative
                    mode = "Immediate Addressing (Base-relative).";
                else if (f.p == 1) //PC-relative
                    mode = "Immediate Addressing (PC-relative).";
                else //Neither Base- nor PC-relative
                    mode = "Immediate & Direct Addressing.";
            }
        }
    }
    return mode;
}


string printToSICFile(string sicLabel, string opCode, string sicOperand, string comment,string objCode){
    //label col 1-6
    string label = sicLabel;
    //col 7-8
    string spaceChars1 = "  ";
    //col 9
    string isExtended = " ";
    flags flag = extractFlags(objCode);
    int extValue = flag.e;
    int indirectValue = 0;
    int immediateValue = 0;

    //debug
    //cout << "\nAddressing type: " <<addressingMode(objCode) << endl;

    //checks if start of string returned by addressingMode() has immediate or indirect in it to apply special character
    if (addressingMode(objCode).substr(0,9) =="iNdirect"){
        indirectValue = 1;
    }
    if (addressingMode(objCode).substr(0,9) =="Immediate"){
        immediateValue = 1;
    }

    if (extValue == 1)
        isExtended = "+";
    //default is space, gets set as other if found
    string operandSymbol = " ";
    if (indirectValue == 1)
        operandSymbol = "@";
    if (immediateValue == 1)
        operandSymbol = "#";

    //op code col 10-15
    string opCodeString = opCode;
    //space col 16 don't change
    string space = " ";
    //# @ = or space char col 17
    //string operandSymbol = operandSym;
    //operand col 18-35 size 18
    string operand = sicOperand;
    //col 36-68 comments size 33
    string comments = comment;

    //This section pads each section with spaces for correct formatting
    while (label.length() < 6){
        label.append(" ");
    }
    while (spaceChars1.length() < 2){
        spaceChars1.append(" ");
    }
    while (opCodeString.length() < 6){
        opCodeString.append(" ");
    }
    while (operandSymbol.length() < 1){
        operandSymbol.append(" ");
    }
    while (operand.length() < 18){
        operand.append(" ");
    }
    while (comments.length() < 33) {
        comments.append(" ");
    }

    //full SIC output line to be written to file
    string sicOutString = label + spaceChars1 + isExtended + opCodeString + space + operandSymbol + operand + comments;

    //Debug
    //cout << sicOutString << endl;
    return sicOutString;

}
//need to account for special char columns!
string printToLISFile(string lineAddr, string symName, string opCode, string operand, string objCode){
    //writing output for LIS file

    //label col 1-4
    string location = lineAddr;
    //col 5-8
    string blank = "    ";
    //col 9-14
    string symbolNameLis = symName;
    //col 15-16 blank
    string doublespace = "  ";

    //col 17 special + symbol
    string isExtended = " ";
    flags flag = extractFlags(objCode);
    int extValue = flag.e;
    int indirectValue = 0;
    int immediateValue = 0;

    //debug
    //cout << "\nAddressing type: " <<addressingMode(objCode) << endl;

    if (addressingMode(objCode).substr(0,8) =="iNdirect"){
        indirectValue = 1;
    }
    if (addressingMode(objCode).substr(0,9) =="Immediate"){
        immediateValue = 1;
    }

    if (extValue == 1)
        isExtended = "+";
    string operandSymbol = " ";
    if (indirectValue == 1)
        operandSymbol = "@";
    if (immediateValue == 1)
        operandSymbol = "#";

    //opcode col 18-23
    string opcodeLis = opCode;
    //col 24-25 blank
    //use doublespace again
    //26 special addressingSymbol
    string addressSymbol = "";
    //27-49 operand/value size 23
    string operandLis = operand;
    //col 50-51 doublespace again
    //col 52-59+ assembled instruction get from record
    string instructionLis =  objCode;


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
    while (opcodeLis.length() < 6){
        opcodeLis.append(" ");
    }
    while (operandLis.length() < 23){
        operandLis.append(" ");
    }
    while (instructionLis.length() < 8){
        instructionLis.append(" ");
    }

    //full LIS output line to be written to file //+ isExtended
    string lisOutString = location + blank + symbolNameLis + doublespace + isExtended + opcodeLis + doublespace + operandSymbol +  operandLis + doublespace + instructionLis;
    //cout << "LIS Outstring" << lisOutString << endl;
    return lisOutString;
}

//reads Symtable and returns vector of strings (each string represents label and its Loc address). Loc address (first 4 charecters of string) and the labels names (last 6 charecters a string) 
vector<string> labelsWithLoc(string fileName) {
    ifstream symtab;
    symtab.open(fileName);
    vector<string> answer;
    string str;
    while (std::getline(symtab, str)) { //reading symtable line by line
        if (str.length() > 15) {
            if (str.length() > 15 && str.substr(8, 2) == "00" && str.substr(16, 1) == "R") { //checking that there is value and an indication of realtive addressing in the lin we are reading
                string labelLoc = str.substr(10,4) + str.substr(0, 6);
                answer.push_back(labelLoc);
            }
        }
    }
    symtab.close();

//    Symbol  Value   Flags:
//  -----------------------
//  FIRST   000000  R
//  LOOP    00000B  R
//  COUNT   00001E  R
//  TABLE   000021  R
//  TABLE2  001791  R
//  TOTAL   002F01  R

//  Name    Literal  Length Address:
//  ------------------------------
//          =X'3F'     2    000003

    //printing out the loc addresses for testing purposes
    std::cout << "The contents of string vector that labelsWith Loc function returns is :" << endl;
    for (std::vector<string>::iterator it = answer.begin(); it != answer.end(); ++it)
        std::cout << *it << endl;
    std::cout << '\n';

  return answer;
}


int main(int argc, char *argv[]) {


	//Checks for correct arguments
    if (argc != 2)
    {
        cout << "The format of the command should be: dxe <objFileName>.obj" << endl;
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

    //reading symtable and returning a vector of strings with labels and Loc addresses here!!
    labelsWithLoc(symFileName);

    ifstream objInput;
    ifstream symInput;
    ofstream sicOutput((sicFileName).c_str());
    ofstream lisOutput((lisFileName).c_str());

    string record;
    string programName;
    string strtAdrsAndLngth;
    string startingAddress;
    string actualstartAddress;
    string programLength;
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

    //used to store address throughout different text records
    //string currentAddress;

    while (objInput >> record) {
        if (count == 0) {
            programName = record;
            programName = programName.substr(1);
            cout << "\nProgram Name is: " << programName << endl;
        }

        count++;
        //avoids counting starting address in header as a separate record
        if (count == 2) {
            strtAdrsAndLngth = record;
            startingAddress = strtAdrsAndLngth.substr(0, 6);

            cout << "Starting address: " << startingAddress << endl;
            programLength = strtAdrsAndLngth.substr(6, 6);
            string zeroAddr = startingAddress;
            actualstartAddress= startingAddress;

            cout << "Program length: " << programLength << endl;

            //makes title lines for SIC and LIS files
            string sicTitleLine;
            string lisTitleLine;
            while (programName.size() < 9) {
                programName = programName + " ";
            }
            //for correct formatting
            string lisStart = programName;
            lisStart = lisStart.substr(0, lisStart.size()-1);
            //lisStart.pop_back();

            if (zeroAddr == "000000") {
                sicTitleLine = programName + "START  " + " 0";
                lisTitleLine = "0000    " + programName + "START   " + " 0";
            } else {
                sicTitleLine = programName + "START   " + startingAddress;
                lisTitleLine = lisStart + "START    " + startingAddress;
            }

            //cout << sicTitleLine << endl;
            //cout << lisTitleLine << endl;

            sicOutput << sicTitleLine << endl;
            lisOutput << lisTitleLine << endl;
            continue;
        }

        //if statements read first char of each line to determine record type
        if (record.at(0) == 'H') {
            //cout << "Found a header record" << endl;
            //cout << record << endl;
        }

        if (record.at(0) == 'T') {
            //cout << "Found a text record" << endl;

            //Makes array of instructions returned from the struct
            instructionList instrList = parseInstructions(record);

            int numInstructionsInRecord = 0;

            //control size of objList array: 2 parts, have counter of how many instructions present in record
            //part 1 if s# != "none" then increase counter

            if (instrList.s0 != "none")
                numInstructionsInRecord++;
            if (instrList.s1 != "none")
                numInstructionsInRecord++;
            if (instrList.s2 != "none")
                numInstructionsInRecord++;
            if (instrList.s3 != "none")
                numInstructionsInRecord++;
            if (instrList.s4 != "none")
                numInstructionsInRecord++;
            if (instrList.s5 != "none")
                numInstructionsInRecord++;
            if (instrList.s6 != "none")
                numInstructionsInRecord++;
            if (instrList.s7 != "none")
                numInstructionsInRecord++;
            if (instrList.s8 != "none")
                numInstructionsInRecord++;
            if (instrList.s9 != "none")
                numInstructionsInRecord++;

            //makes array of instructions of correct size;
            string objList[numInstructionsInRecord];

            //part 2 if that instruction exists add to objCode[i]
            if (instrList.s0 != "none")
                objList[0] = instrList.s0;
            if (instrList.s1 != "none")
                objList[1] = instrList.s1;
            if (instrList.s2 != "none")
                objList[2] = instrList.s2;
            if (instrList.s3 != "none")
                objList[3] = instrList.s3;
            if (instrList.s4 != "none")
                objList[4] = instrList.s4;
            if (instrList.s5 != "none")
                objList[5] = instrList.s5;
            if (instrList.s6 != "none")
                objList[6] = instrList.s6;
            if (instrList.s7 != "none")
                objList[7] = instrList.s7;
            if (instrList.s8 != "none")
                objList[8] = instrList.s8;
            if (instrList.s9 != "none")
                objList[9] = instrList.s9;

            //hardcoded for now, these all must be gone before we're finished
            string labels[] = {"", "LOOP", "", "", "LOOP2", "", "WAIT", "", "", ""};
            string operands[] = {"LISTA", "LISTB", "LISTC", "MAXLEN", "MIN", "TOTAL", "LISTA-LISTB",
                                 "LISTB-LISTC+LISTA", "FIRST", "STORE"};

            string instructions [numInstructionsInRecord];


            //loop to populate the instruction name array
            for (int i = 0; i < numInstructionsInRecord; i++) {
                //gets correct opcode for instruction
                string codeResult = Opcode::getOpcode(objList[i]);
                //gets index of code in table
                int index = Opcode::findOpcode(codeResult);
                //returns opcode name to put in array
                string instrName = Opcode::getInstruction(index);
                instructions[i] = instrName;
            }

            //program not able to handle starting address larger than FFFF due to 4 digit limit
            //Generates addresses for location column for instructions in this text record
            vector<string> addressesLIS = addressesLoc(startingAddress, instrList);

            string addresses[addressesLIS.size()];
            string buffer;

            string finalAddress;

            //puts addresses from vector into array after capitalizing them
            for (int i = 0; i < addressesLIS.size(); i++) {
                buffer = addressesLIS[i];

                //fixes issue where first address in each record would be only 2 digits
                if (buffer.size() != 4){
                    buffer = "00" + buffer;
                }
                //holds each char in address to have toupper() run on it
                string cap[] = { "0","0","0","0"};
                int ctr = 0;

                //need to rewrite as a for loop since edoras doesn't like it
                for (char x: buffer) {
                    x = toupper(x);
                    cap[ctr] = x;
                    ctr++;
                }

                for (int i = 0; i < buffer.size(); i++){

                }




                buffer = cap[0] + cap[1] + cap[2] + cap[3];
                addresses[i] = buffer;

                if (i == (addressesLIS.size() - 1)) {
                    finalAddress = buffer;
                }
            }



            //TODO check if need to be in hex or decimal for label replacement
            //fills in correct operand and converts from hex to decimal to display in file
            for (int i = 0; i < numInstructionsInRecord; i++) {
                //gets displacement field
                operands[i] = extractDisplacement(objList[i]);
                //retrieves decimal form of operand to display
                //int num = Opcode::hexToInt(operands[i]);
                //cout <<"Hex to int for: " << operands[i] << " is " << to_string(num) <<endl;
                //operands[i] = to_string(num);
            }



            //TODO
            // need to read symtable, fill in symbol column, compare operands with symtable and replace them.
            // need to include literal detection and replacement as well,
            // including adding lines using correct EQU and LTORG instructions.
            // would need to check if each operand contains a literal before calling print statements and
            // call helper function to do this.





            //Does this as many times are there are instructions in each record
            cout << endl;
            for (int i = 0; i < numInstructionsInRecord; i++) {



                //This statement generates a string with SIC format
                string sicOutString = printToSICFile(labels[i], instructions[i], operands[i], "This is a comment.",
                                                     objList[i]);

                //This statement generates a string with LIS format
                string lisOutString = printToLISFile(addresses[i], labels[i], instructions[i], operands[i], objList[i]);


                //Statements below show SIC and LIS output in console (Having both on at once not recommended for readability's sake)
                //cout << sicOutString << endl;
                cout << lisOutString << endl;

                //Writes both strings to their respective files
                sicOutput << sicOutString << endl;
                lisOutput << lisOutString << endl;

                //dante stuff here
                // base stuff here by parsing objList[] via adressingmode()
                //if an objlist[i] has base in it, cout "            BASE"


                //.substr(0,2)
                //PRINT "BASE" ASSEMBLER DIRECTIVE (parse objList)
                if (Opcode::getOpcode(objList[i]) == "68") { //If instruction == "LDB"
                    //cout << "Inside dantes's loop" <<Opcode::getOpcode(objList[i])<<  endl;
                    string basestring = "         BASE                 ";
                    string lisbasestring = addresses[i] + "             BASE";


                    sicOutput << basestring << endl;
                    lisOutput << lisbasestring << endl;

                    //debug
                    cout << lisbasestring << endl;
                }
            }

            //debug testing known immediate and indirect instructions to test function
            /*cout << "\nTesting addressing modes" << endl;
            cout << addressingMode("022030") << endl;
            cout << addressingMode("010030") <<  "\n"<< endl;

            cout << "\nTesting displacement extraction" << endl;
            cout << extractDisplacement("022030") << endl;
            cout << extractDisplacement("010030") <<  "\n"<< endl;*/

            //saves last address to know where to start for next text record
            startingAddress = finalAddress;

            //TODO BEN TASK

            //read in stuff from symtable in vector format ("4 chars addr + 6 chars label name"
            // then work back from the end taking difference to get size of
            // that declaration then add label
            //finalAddress will be where first RESB/RESW declaration starts
            //need to save finalAddress again at end to end of prog/record


        }

        //Can access the mod record in this loop
        if (record.at(0) == 'M') {
            //cout << "Found a modification record" << endl;
            //cout << "\n" << record << endl;
        }

        //Can access the end record in this loop
        if (record.at(0) == 'E') {
            //cout << "Found an end record" << endl;
            cout << "                 END      " + actualstartAddress << endl;
            sicOutput << "         END     " + actualstartAddress<< endl;
            lisOutput << "                 END      " + actualstartAddress<< endl;
        }
    }

    cout << "\n\nTODO:\n" << endl;
    cout << "- Need to read symtable, fill in symbol column, compare operands with symtable and replace them." << endl;
    cout << "- Need to do literal detection and replacement, including adding lines using correct EQU and LTORG instructions." << endl;
    cout << "- Would need to check if each operand contains a literal before calling print statements." << endl;




    //closes all filestreams
    objInput.close();
    symInput.close();
    sicOutput.close();
    lisOutput.close();
    return 0;
}
