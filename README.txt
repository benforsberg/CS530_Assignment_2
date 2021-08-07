CS 530 Spring 2020
Program #2 Disassembler dxe
Ben Forsberg, cssc1930
Marina Pestriacheva, cssc1907
Dante Colombo-Sadeghi, cssc1911
FileName: README.txt

Files Included In Project:
	opcode.h
	opcode.cpp
	main.cpp
	README.txt
	data.obj
	data.sym

This project is hosted on Github at https://github.com/benforsberg/CS530_Assignment_2

File Breakdown:
	-opcode.cpp: Contains the hardcoded opcode table, along several helper methods to retreive the opcode or its name. It also includes a few helper methods to convert data between hex, int, and binary.
	-main.cpp: Holds the majority of the program. It is responsible for: getting filenames, managing file creation, parsing the records in the obj file, parsing the sym file, breaking down instructions, reading the flags, extracting the displacement fields, calculating addresses, assigning labels, and producing the output for SIC and LIS files.
	
Compile Instructions:
	run "make" inside program directory
	
Program Operating Instructions:
	The executable produced is named dxe and requires the filename of an obj file as an argument. A sym file of the same filename must also be present in the directory for it to run.
	
	Usage:
	dxe <filename>.obj
	
Program Overview and Structure:
	-Main() starts by opening the obj file and searching for a sym file of the same name
	-It begins looking for the different record types
	-If a H record is found:
		-It saves the program name, start address, and program size
	-If a T record is found:
		-It uses parseInstructions() to read the flags and separate the instructions, and then an array is created to hold them
		-The opcodes are determined and used to fill an array of instruction names
		-The labels and label addresses are are determined and used to fill two arrays
		-The operands array is created and filled by extractDisplacement()
		-The literal table is read by literalsWithLoc() and literals are placed in the operand array
		-Labels are put into the label array, and into operands where necessary
		-printToSic() and printToLis() are called to generate an outprint string for each line
		-Each line is sent to the output file
		-If LDB or literals are placed, the next line prints the directive BASE or LTORG
		-The arrays are reversed to calculate RESB/RESW using labels and label addresses
		-Those lines are generated and outputted
	-If an E record is found:
		-The program prints END then checks if the starting address is equal to the address of the first label.
			-If they are equal, the label is printed after END
			-If they are not equal, the starting address is printed after END
		
Known Deficiencies/Bugs:
	None
	
Lessons Learned:	
	How to program in a group utilizing collaborative software such as Git and communication software such as Discord.
	A better understanding of C++ syntax and libraries.
	A better understanding of how the SIC/XE architecure works.
