//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//Current File	:			Emulator.cpp


#include "stdafx.h"
#include "Emulator.h"


/**/
/*
		Emulator::InsertMemory() Emulator::InsertMemory()

	NAME
		bool Emulator::InsertMemory - inserts information into VC3600 memory 

	SYNOPSIS
		bool Emulator::InsertMemory(int a_location, int a_contents);

			a_location	--> the location of where the contents need to be stored
			a_contents	--> the contents that are to be stored in memory

	DESCRIPTION
		The function is responsible for inserting information into VC3600
		memory for the Emulator to use. The contents consist of an operation
		code, and where the operand/label is located if it were to have one.

	RETURNS
		Returns true or false, true upon successful memory insertion,
		false if the insertion was attempted outside of the memory scope.

*/
/**/
bool Emulator::InsertMemory(int a_location, int a_contents) {
	if (a_location > MEMSZ) {
		//THROW ERROR: out of memory.
		return false;
	} 
	m_memory[a_location] = a_contents;
	return true;
}
/*bool Emulator::InsertMemory(int a_location, int a_contents);*/


/**/
/*
		Emulator::RunProgram() Emulator::RunProgram()

	NAME
		bool Emulator::RunProgram - runs the instructions stored in memory

	SYNOPSIS
		bool Emulator::RunProgram()

	DESCRIPTION
		The function is responsible for finding the appropriate instructions
		to execute from memory and execute them in the proper order, in
		accordance to what the instruction was.

	RETURNS
		Returns true upon successful execution of the program,
		false upon failure.

*/
/**/
bool Emulator::RunProgram() {
	int currentLoc = 0;
	currentLoc = m_memory[currentLoc];
	for (; currentLoc < MEMSZ; ) {
		string contents = to_string(m_memory[currentLoc]);
		string opCode = "";
		string location = "";
		unsigned it = 0;

		// Get contents of the memory location :: Opcode | Location
		if (stoi(contents) > 99999) {

			//opcode is between 10 and 13
			for (; it < 2; ++it) opCode = opCode + contents[it];
			for (; it < contents.size(); ++it) location = location + contents[it];
		}
		else {
			for (; it < 1; ++it) opCode = opCode + contents[it];
			for (; it < contents.size(); ++it) location = location + contents[it];

		}

		int opCodeVal = stoi(opCode);
		int locationVal = stoi(location);

		// Switch statement determining what instruction was at the location and executing the command
		switch (opCodeVal) {
		case MT_ADD:
			m_acceumulator = m_acceumulator + m_memory[locationVal];
			break;
		case MT_SUB:
			m_acceumulator = m_acceumulator - m_memory[locationVal];
			break;
		case MT_MULT:
			m_acceumulator = m_acceumulator * m_memory[locationVal];
			break;
		case MT_DIV:
			if (m_memory[locationVal] == 0) {
				cout << "ERROR:: DIVISION BY ZERO" << endl;
				exit(1);
			}
			else m_acceumulator = m_acceumulator / m_memory[locationVal];
			break;
		case MT_LOAD:
			m_acceumulator = m_memory[locationVal];
			break;
		case MT_STORE:
			m_memory[locationVal] = m_acceumulator;
			break;
		case MT_READ:
			Read(locationVal);
			break;
		case MT_WRITE:
			cout << m_memory[locationVal] << endl;
			break;
		case MT_B:
			currentLoc = locationVal;
			continue;
		case MT_BM:
			if (m_acceumulator < 0) {
				currentLoc = locationVal;
				continue;
			}
			break;
		case MT_BZ:
			if (m_acceumulator == 0) {
				currentLoc = locationVal;
				continue;
			}
			break;
		case MT_BP:
			if (m_acceumulator > 0) {
				currentLoc = locationVal;
				continue;
			}
			break;
		case MT_HALT:
			return true;
			break;
		default:
			//Err
			cout << "Unknown Operation Code :: Terminating Failure" <<endl;
			return false;
		}
		currentLoc++;
	}
	return false;
}
/*bool Emulator::RunProgram();*/


/**/
/*
		Emulator::Read() Emulator::Read()

	NAME
		void Emulator::Read

	SYNOPSIS
		void Emulator::Read(int a_location);

			a_location,	the integer value of the current location

	DESCRIPTION
		The function is responsible for error checking read input from
		the user, and storing it in the accumulator.

	RETURNS
		((void))

*/
/**/
void Emulator::Read(int a_location) {
	string input;
	bool isAlpha = true;
	while (isAlpha) {
		cout << "? ";
		cin >> input;
		for (auto it = input.begin(); it != input.end(); ++it) {
			if (isdigit(*it)) {
				isAlpha = false;
				continue;
			}
			else if (*it == '-' && it == input.begin()) {
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
/*void Emulator::Read(int a_location);*/