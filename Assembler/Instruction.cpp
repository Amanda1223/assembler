//###################################################################
//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//###################################################################
//Current File	:			Instruction.cpp
//###################################################################

#include "stdafx.h"
#include "Instruction.h"


/*##################################################################
#	NAME
#		Instruction::InstructionType Instruction::ParseInstruction
#
#	SYNOPSIS
#		Instruction::InstructionType Instruction::ParseInstruction(string &a_buff);
#
#			a_buff	--> a single line within the file
#
#	DESCRIPTION
#		This function is responsible for examining the line, and
#		determining the type of instruction the line contains.
#		OPTIONS : ST_MachineLanguage | ST_AssemblerInstr | ST_Comment
#					ST_End
#
#	RETURNS
#		Returns the instruction type of the current line (string
#		which is passed in).
#
##################################################################*/
Instruction::InstructionType Instruction::ParseInstruction(string &a_buff) {


	//determining if line is a comment
	if (a_buff[0] == ';') {
		m_type = InstructionType::ST_Comment;
		return m_type;
	}

	//determining if line is a machine language instruction
}
/*Instruction::InstructionType Instruction::ParseInstruction(string &a_buff);*/