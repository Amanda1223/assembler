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
	Errors::InitErrorReporting();
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
	system("pause");
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

	//Initialize error reporting, will clear out vector if anything in it.
	Errors::InitErrorReporting();
	m_facc.rewind();

	// Parse in each line from the file :: re-examining, except this time around we have the filled Symbol Table.
	int loc = 0;
	int operandLoc = 0;
	cout << "Translation of Program : " << endl << endl;
	cout << "Location\tContents\tOriginal Instruction" <<endl;
	for (; ; ) {
		string buff;
		if (!m_facc.GetNextLine(buff)) {

			// Missing an end statement
			Errors::RecordError(Errors::createError(loc, string(" *FATAL* missing assembly language END statement. Unable to continue...")));
		}

		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);

		// Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) {
			TranslationOutput(loc, operandLoc, m_inst.GetOpCodeNum(), buff, st);
			if (!m_facc.GetNextLine(buff)) return;
			// Else there were still more lines after the END statement
			else Errors::RecordError(Errors::createError(loc, string("statement after assembly language END instruction.")));
		}
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) {
			TranslationOutput(loc, operandLoc, m_inst.GetOpCodeNum(), buff, st);
			continue;
		}

		// If there is (a) an operand, (b) if it isn't numeric (need label/operand name) (c) check if it exists in the symbol table, THEN we can determine if errors occured.
		// operandLoc will be in terms of this function checking, 0 if it was numeric, else operandLoc will be whatever was in the symbol table.
		operandLoc = 0;
		if (!m_inst.GetOperand().empty() && !m_inst.isNumericOperand() && !m_symtab.LookupSymbol(m_inst.GetOperand(), operandLoc)) {

			// If location is -999 it's multiply defined, else it is undefined
			if (operandLoc == -999) {
				Errors::RecordError(Errors::createError(loc, string("multiply defined variable named - " + m_inst.GetOperand())));
			}
			else {
				Errors::RecordError(Errors::createError(loc, string("undefined variable named - " + m_inst.GetOperand())));
				m_symtab.AddSymbol(m_inst.GetOperand(), m_symtab.undefinedSymbol);
			}
		}

		// the location can also indicate out of memory issue. > MEMSZ
		if (loc > emulator::MEMSZ) Errors::RecordError(Errors::createError(loc, "current location exceeds memory."));
		TranslationOutput(loc, operandLoc, m_inst.GetOpCodeNum(), buff, st);
		loc = m_inst.LocationNextInstruction(loc);

		//PRINT ERRORS ON LINE HERE
	}
	system("pause");

	//PRINT ERROR LIST HERE, IF ERRORS, DO NOT RUN THE EMULATOR.
}
/*void Assembler::PassII();*/


/*##################################################################
#	NAME
#		void Assembler::TranslationOutput
#
#	SYNOPSIS
#		void Assembler::TranslationOutput(int a_instLocation, int a_operandLocation, int a_opCodeNum, string a_instruction, Instruction::InstructionType a_type);
#
#			a_instLocation		--> the location of the instruction itself.
#			a_operandLocation	--> the operand location within the symbol table.
#			a_opCodeNum			--> the operation code number if it was a ML instruction (0 otherwise)
#			a_instruction		--> the original instruction in the text file.
#			a_type				--> the type of instruction on the line (ML/AL)
#
#	DESCRIPTION
#		This function's purpose is to display the line information and
#		translation to the user, using the above variables. As well as
#		store the required variables into memory.
#
#	RETURNS
#		((void))
#
##################################################################*/
void Assembler::TranslationOutput(int a_instLocation, int a_operandLocation, int a_opCodeNum, string a_instruction, Instruction::InstructionType a_type) {
	if (Instruction::ST_End == a_type || Instruction::ST_Comment == a_type) {
		cout << "\t\t\t   " << a_instruction << endl;
		return;
	}

	// If it was an assembler instruction, and therefore different statement is required.
	if (Instruction::ST_AssemblerInstr == a_type) {
	
		//ORG and DS instructions do not get "contents" ie operandLocation/opCodNum
		if ("ORG" == m_inst.GetOpCode() || "DS" == m_inst.GetOpCode()) {
			cout << "  " << a_instLocation << "\t\t\t   " << a_instruction << endl;
			return;
		}

		//We will have some numerical value in the operand upon AL instructions
		cout << "  " << a_instLocation << "\t\t" << setfill('0') << setw(2) << a_opCodeNum << setw(4) << m_inst.GetOperandVal() << "\t   " << a_instruction << endl;
		return;
	}
	cout << "  " << a_instLocation << "\t\t" << setfill('0') << setw(2) << a_opCodeNum << setw(4) << a_operandLocation << "\t   " << a_instruction << endl;
	return;
}
/*void Assembler::TranslationOutput(int a_instLocation, int a_operandLocation, int a_opCodeNum, string a_instruction, Instruction::InstructionType a_type);*/


/*##################################################################
#	NAME
#		void Assembler::RunEmulator
#
#	SYNOPSIS
#		void Assembler::RunEmulator()
#
#	DESCRIPTION
#		This function
#
#	RETURNS
#		((void))
#
##################################################################*/
void Assembler::RunEmulator() {

}
/*void Assembler::RunEmulator();*/