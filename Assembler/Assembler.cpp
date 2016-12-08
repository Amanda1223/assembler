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

/*##################################################################
#	NAME
#		void Assembler::Assembler
#
#	SYNOPSIS
#		Assembler::Assembler(int argc, char *argv[]);
#
#			argc	--> number of arguments passed to the progam. Should only be 2
#			argv	--> the array of arguments passed to the program
#						[0] should be the program name
#						[1] should be the file we are reading from
#
#	DESCRIPTION
#		Opening the file in the command line arguments for program use
#		using the FileAccess class.
#
#	RETURNS
#		((void))
#
##################################################################*/
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
#		This function is responsible for:
#				(1) Identifying the instruction type of a line.
#				(2) Filling in the symbol table for the whole text file. 
#			
#	RETURNS
#		((void))
#
##################################################################*/
void Assembler::PassI()
{
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
		cout << buff << endl;
		
		// If this is an end statement, there is nothing left to do in pass I.
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


/*##################################################################
#	NAME
#		void Assembler::PassII
#
#	SYNOPSIS
#		void Assembler::PassII();
#
#	DESCRIPTION
#		This function is responsible for:
#				(1) Translating each line into instruction code.
#				(2) Finding each error, and reporting them accordingly.
#
#	RETURNS
#		((void))
#
##################################################################*/
void Assembler::PassII() {

	//Initialize Error class.
	Errors err;
	err.InitErrorReporting();

	//Parse in each line from the file :: re-examining, except this time around we have the filled Symbol Table.
	int loc = 0;
	cout << "Translation of Program : " << endl << endl;
	for (; ; ) {
		string buff;
		if (!m_facc.GetNextLine(buff)) {
			// If there are no more lines, we are missing an end statement.
			// We will let this error be reported by Pass II.

			return;
		}

		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);

		// Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) return;

		// Labels can only be on machine language and assembler language
		// instructions.  So, skip other instruction types.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) continue;

		loc = m_inst.LocationNextInstruction(loc);

		//output the translation
		//LOCATION	| CONTENTS 000 1234	| ORIGINAL
		//loc		| m_NumOpCode && location + value of operand | m_inst.m_instruction 
		cout << loc << "/t" << setfill('0') << setw(2) << m_inst.GetOpCode() << setw(4) << "/t" << buff << endl;
	}

	//Use the instruction class to figure out the type, location and Label/Operand if any -- Translating

	//If there was a Operand, find the value that was stored if any [ERROR if no value was stored.]

	//Each instruction may have its own Error depending on how it is used.

	//DC/DS instructions need to have LABEL | DS/DC | NUMERICAL_VALUE [ERROR if there was no value within the "third column"]

	//ORG needs to have a numerical value after it. [ERROR if no value after]



	//If any errors were found, we should NOT emulate the program, and report all the errors

}
/*void Assembler::PassII();*/