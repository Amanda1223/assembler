//###################################################################
//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//###################################################################
//Current File	:			Emulator.cpp
//###################################################################


#include "stdafx.h"
#include "Emulator.h"


/**/
/*##################################################################
#   bool emulator::insertMemory() bool emulator::insertMemory()
#
#	NAME
#		bool emulator::insertMemory - inserts information into VC3600 memory 
#
#	SYNOPSIS
#		bool emulator::insertMemory(int a_location, int a_contents);
#
#			a_location	--> the location of where the contents need to be stored
#			a_contents	--> the contents that are to be stored in memory
#
#	DESCRIPTION
#		The function is responsible for inserting information into VC3600
#		memory for the emulator to use. The contents consist of an operation
#		code, and where the operand/label is located if it were to have one.
#
#	RETURNS
#		Returns true or false, true upon successful memory insertion,
#		false if the insertion was attempted outside of the memory scope.
#
##################################################################*/
/**/
bool emulator::insertMemory(int a_location, int a_contents) {
	if (a_location > MEMSZ) {
		//THROW ERROR: out of memory.
		return false;
	} 
	m_memory[a_location] = a_contents;
	return true;
}
/*bool emulator::insertMemory(int a_location, int a_contents);*/


/**/
/*##################################################################
#   bool emulator::runProgram() bool emulator::runProgram()
#
#	NAME
#		bool emulator::runProgram - runs the instructions stored in memory
#
#	SYNOPSIS
#		bool emulator::runProgram()
#
#	DESCRIPTION
#		The function is responsible for finding the appropriate instructions
#		to execute from memory and execute them in the proper order, in
#		accordance to what the instruction was.
#
#	RETURNS
#		Returns true upon successful execution of the program,
#		false upon failure.
#
##################################################################*/
/**/
bool emulator::runProgram() {
	int currentLoc = 0;

	// Starting point is at 0, parse through the code to find opcode|address
	// Wrap this in a while / for loop
	for (; currentLoc < MEMSZ; ) {
		string contents = to_string(m_memory[currentLoc]);
		string opCode = "";
		string location = "";
		int it = 0;
		for (; it < 2; ++it) opCode.append(to_string(contents[it]));
		for (; it < contents.size(); ++it) location.append(to_string(contents[it]));

		int opCodeVal = stoi(opCode);
		int locationVal = stoi(location);

		switch (opCodeVal) {
		case emulator::MT_ADD:
			Add(locationVal);
			break;
		case emulator::MT_SUB:
			Sub(locationVal);
			break;
		case emulator::MT_MULT:
			Mult(locationVal);
			break;
		case emulator::MT_DIV:
			Div(locationVal);
			break;
		case emulator::MT_LOAD:
			Load(locationVal);
			break;
		case emulator::MT_STORE:
			Store(locationVal);
			break;
		case emulator::MT_READ:
			Read(locationVal);
			break;
		case emulator::MT_WRITE:
			Write(locationVal);
			break;
		case emulator::MT_B:
			Branch(locationVal, currentLoc);
			continue;
		case emulator::MT_BM:
			BranchNeg(locationVal, currentLoc);
			continue;
		case emulator::MT_BZ:
			BranchZero(locationVal, currentLoc);
			continue;
		case emulator::MT_BP:
			BranchPos(locationVal, currentLoc);
			continue;
		case emulator::MT_HALT:
			cout << "Terminating Success." << endl;
			system("pause");
			exit(0);
			break;
		default:
			// OpCode was 0 :: get start location.
			currentLoc = locationVal;
			break;
		}
		currentLoc++;
	}

}
/*bool emulator::runProgram();*/


/**/
/*##################################################################
#
#
#	NAME
#		
#
#	SYNOPSIS
#		
#
#	DESCRIPTION
#		The function is responsible for 
#
#	RETURNS
#		Returns 
#
##################################################################*/
/**/
void emulator::Add(int a_location) {
	m_acceumulator = m_acceumulator + m_memory[a_location];
}
/**/


/**/
/*##################################################################
#
#
#	NAME
#
#
#	SYNOPSIS
#
#
#	DESCRIPTION
#		The function is responsible for
#
#	RETURNS
#		Returns
#
##################################################################*/
/**/
void emulator::Sub(int a_location) {
	m_acceumulator = m_acceumulator - m_memory[a_location];
}
/**/


/**/
/*##################################################################
#
#
#	NAME
#
#
#	SYNOPSIS
#
#
#	DESCRIPTION
#		The function is responsible for
#
#	RETURNS
#		Returns
#
##################################################################*/
/**/
void emulator::Mult(int a_location) {
	m_acceumulator = m_acceumulator * m_memory[a_location];
}
/**/


/**/
/*##################################################################
#
#
#	NAME
#
#
#	SYNOPSIS
#
#
#	DESCRIPTION
#		The function is responsible for
#
#	RETURNS
#		Returns
#
##################################################################*/
/**/
void emulator::Div(int a_location) {
	if (m_memory[a_location] == 0) {
		cout << "ERROR:: DIVISION BY ZERO" << endl;
		exit(1);
	}
	else m_acceumulator = m_acceumulator / m_memory[a_location];
}
/**/


/**/
/*##################################################################
#
#
#	NAME
#
#
#	SYNOPSIS
#
#
#	DESCRIPTION
#		The function is responsible for
#
#	RETURNS
#		Returns
#
##################################################################*/
/**/
void emulator::Load(int a_location) {
	m_acceumulator = m_memory[a_location];
}
/**/

/**/
/*##################################################################
#
#
#	NAME
#
#
#	SYNOPSIS
#
#
#	DESCRIPTION
#		The function is responsible for
#
#	RETURNS
#		Returns
#
##################################################################*/
/**/
void emulator::Store(int a_location) {
	m_memory[a_location] = m_acceumulator;
}
/**/


/**/
/*##################################################################
#
#
#	NAME
#
#
#	SYNOPSIS
#
#
#	DESCRIPTION
#		The function is responsible for
#
#	RETURNS
#		Returns
#
##################################################################*/
/**/
void emulator::Read(int a_location) {
	string input;
	bool isAlpha = true;
	while (isAlpha) {
		cout << '?';
		cin >> input;
		for (auto it = input.begin(); it != input.end(); ++it) {
			if (isdigit(*it)) {
				isAlpha = false;
				continue;
			}
			else {
				isAlpha = true;
				cout << "Error :: Input is not integer." << endl;
				break;
			}
		}
	}
	int value = stoi(input);
	m_memory[a_location] = value;
}
/**/

void emulator::Write(int a_location) {
	cout << m_memory[a_location];
	return;
}

void emulator::Branch(int a_jumpLocation, int &a_newLocation) {
	a_newLocation = a_jumpLocation;
}

void emulator::BranchNeg(int a_jumpLocation, int &a_newLocation) {
	if (m_acceumulator < 0) {
		a_newLocation = a_jumpLocation;
	}
	else a_newLocation += 1;
	return;
}

void emulator::BranchPos(int a_jumpLocation, int &a_newLocation) {
	if (m_acceumulator > 0) {
		a_newLocation = a_jumpLocation;
	}
	else a_newLocation += 1;
	return;
}

void emulator::BranchZero(int a_jumpLocation, int &a_newLocation) {
	if (m_acceumulator == 0) {
		a_newLocation = a_jumpLocation;
	}
	else a_newLocation += 1;
	return;
}