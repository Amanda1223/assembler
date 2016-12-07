//###################################################################
//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//###################################################################
//Current File	:			Assembler.cpp
//###################################################################



//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.
Assembler::Assembler(int argc, char *argv[])
	: m_facc(argc, argv)
{
}
/*Assembler::Assembler(int argc, char *argv[]);*/

// Destructor currently does nothing.  You might need to add something as you develop this project.
Assembler::~Assembler()
{
}
/*Assembler::~Assembler();*/

/*##################################################################
#	NAME
#		void Assembler::PassI
#
#	SYNOPSIS
#		void Assembler::PassI();
#
#	DESCRIPTION
#		This function is responsible for identifying the instruction
#		type as well as beginning the symbol table to prep Pass II. 
#			
#	RETURNS
#		((void))
#
##################################################################*/
void Assembler::PassI()
{
		//hello
	int loc = 0;        // Tracks the location of the instructions to be generated.

	// Successively process each line of source code.
	for (; ; ) {

		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {
			// If there are no more lines, we are missing an end statement.
			// We will let this error be reported by Pass II.
			return;
		}
		
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);

		
		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) return;

		// Labels can only be on machine language and assembler language
		// instructions.  So, skip other instruction types.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) continue;

		// If the instruction has a label, record it and its location in the
		// symbol table.
		if (m_inst.isLabel()) {
			m_symtab.AddSymbol(m_inst.GetLabel(), loc);
		}
		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
}
/*void Assembler::PassI();*/