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

bool emulator::insertMemory(int a_location, int a_opcode, int a_value) {
	m_instruction temp;
	temp.location = a_location;
	temp.opcode = a_opcode;
	temp.value = a_value;	//value is either (a) the location of the label instruction or (b) the value of the symbolic operand
	emulator::m_instructionList.push_back(temp);
}