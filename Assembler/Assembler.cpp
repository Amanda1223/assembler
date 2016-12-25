//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller


/*
* Current File	:	Assembler.cpp
* Description	:	Contains the implementation of the steps it takes to assemble the VC3600 programming languages, including
*					opening the file to read the lines of programming language from. These will parsed into the Assembler to
*					be parsed and utilized through Pass I, Pass II and the Emulator.
*/

#include "stdafx.h"
#include "Assembler.h"


/**/
/*
		Assembler::Assembler() Assembler::Assembler
	NAME
		Assembler::Assembler

	SYNOPSIS
		Assembler::Assembler(int argc, char *argv[]);

			argc	--> number of arguments passed to the progam. Should only be 2
			argv	--> the array of arguments passed to the program
						[0] should be the program name
						[1] should be the file we are reading from

	DESCRIPTION
		Opening the file in the command line arguments for program use
		using the FileAccess class.

	RETURNS
		((void))

*/
/**/
Assembler::Assembler(int argc, char *argv[])
	: m_facc(argc, argv)
{
}
/*Assembler::Assembler(int argc, char *argv[]);*/


/**/
/*
		Assembler::PassI() Assembler::PassI()

	NAME
		void Assembler::PassI

	SYNOPSIS
		void Assembler::PassI();

	DESCRIPTION
		This function is responsible for:
				(1) Identifying the instruction type of a line.
				(2) Filling in the symbol table for the whole text file. 
				(3) Error checks, however, no errors are saved in this.
			
	RETURNS
		((void))

*/
/**/
void Assembler::PassI() {
	Errors::InitErrorReporting(); //Required statement for error checking in the parsing the instruciton
	int loc = 0;

	// Loop is for gathering each line in a file to the end.
	for ( ; ; ) {

		// Read in a line from the file to be parsed.
		string buff;
		if (!m_facc.GetNextLine(buff)) {
			return;
		}

		// Parse the line and get the instruction type :: done through the Instruction class.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);
		cout << buff << endl;
		
		// Reached an end statement :: on to Pass II
		if (st == Instruction::ST_End) return;

		// Labels only on Machine Language  & Assembler, else continue to next line.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) continue;
		if (m_inst.isLabel()) {
			m_symtab.AddSymbol(m_inst.GetLabel(), loc);
		}
		loc = m_inst.LocationNextInstruction(loc);
	}
	system("pause");
}
/*void Assembler::PassI();*/


/**/
/*
		Assembler::PassII() void Assembler::PassII()

	NAME
		void Assembler::PassII

	SYNOPSIS
		void Assembler::PassII();

	DESCRIPTION
		This function is responsible for:
				(1) Translating each line into instruction code.
				(2) Finding each error, and reporting them accordingly.

	RETURNS
		((void))

*/
/**/
void Assembler::PassII() {

	// Initialize the error reporting, clears out any unwanted information from Pass I
	Errors::InitErrorReporting();
	m_facc.Rewind();

	// Parse in each line from the file :: re-examining, except this time around we have the filled Symbol Table.
	int loc = 0;
	int operandLoc = 0;
	cout << "____________________________________________________________" << endl;
	cout << "Translation of Program : " << endl << endl;
	cout << "Location\tContents\tOriginal Instruction" <<endl;
	for (; ; ) {
		string buff;
		if (!m_facc.GetNextLine(buff)) {

			// If we were missing an END statement, there should be an error & return here since no more lines.
			Errors::RecordError(Errors::CreateError(loc, string(" *FATAL* missing assembly language END statement. Unable to continue...")));
			return;
		}

		// Parse through the instructions, determining what to do based on Instruction type
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);
		if (st == Instruction::ST_End) {
			if (m_facc.GetNextLine(buff)) {
				Errors::RecordError(Errors::CreateError(loc, string("statement after assembly language END instruction.")));
				Errors::ReportCurrentError();
			}
			return;
		}
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) {
			TranslationOutput(loc, operandLoc, m_inst.GetOpCodeNum(), buff, st);
			continue;
		}
		operandLoc = 0;

		// Read this next statement with caution :: checking for a alphanumeric operand that is within a symbol table.
		if (!m_inst.GetOperand().empty() && !m_inst.isNumericOperand() && !m_symtab.LookupSymbol(m_inst.GetOperand(), operandLoc)) {

			// If it does not satisfy the above condition, then there is probably a garbage value in the symbol table.
			if (operandLoc == -999) {
				Errors::RecordError(Errors::CreateError(loc, string("multiply defined variable named - " + m_inst.GetOperand())));
				Errors::ReportCurrentError();
			}
			else {
				Errors::RecordError(Errors::CreateError(loc, string("undefined variable named - " + m_inst.GetOperand())));
				m_symtab.AddSymbol(m_inst.GetOperand(), m_symtab.undefinedSymbol);
				Errors::ReportCurrentError();
			}
		}

		// Output the line according to the translated values
		TranslationOutput(loc, operandLoc, m_inst.GetOpCodeNum(), buff, st);
		if (loc > Emulator::MEMSZ) {
			Errors::RecordError(Errors::CreateError(loc, "current location exceeds memory."));
			Errors::ReportCurrentError();
		}
		loc = m_inst.LocationNextInstruction(loc);
	}
	system("pause");
}
/*void Assembler::PassII();*/


/**/
/*
		Assembler::TranslationOutput() Assembler::TranslationOutput()

	NAME
		void Assembler::TranslationOutput

	SYNOPSIS
		void Assembler::TranslationOutput(int a_instLocation, int a_operandLocation, int a_opCodeNum, string a_instruction, Instruction::InstructionType a_type);

			a_instLocation		--> the location of the instruction itself.
			a_operandLocation	--> the operand location within the symbol table.
			a_opCodeNum			--> the operation code number if it was a ML instruction (0 otherwise)
			a_instruction		--> the original instruction in the text file.
			a_type				--> the type of instruction on the line (ML/AL)

	DESCRIPTION
		This function's purpose is to display the line information and
		translation to the user, using the above variables. As well as
		store the required variables into memory.

	RETURNS
		((void))

*/
/**/
void Assembler::TranslationOutput(int a_instLocation, int a_operandLocation, int a_opCodeNum, string a_instruction, Instruction::InstructionType a_type) {
	string contents = "";
	Assembler::InstructionInfo temp;
	stringstream stream;
	int location= a_operandLocation;

	
	// If it is just a Comment or an END statement we can output the contents right away.
	if (Instruction::ST_Comment == a_type) {
		cout << "\t\t\t   " << a_instruction << endl;
		return;
	}
	if (Instruction::ST_End == a_type) {
		cout << "\t\t\t   " << a_instruction << endl;
		return;
	}

	// If it was an assembler instruction, and therefore different statement is required.
	if (Instruction::ST_AssemblerInstr == a_type) {
	
		// ORG and DS instructions do not get "contents" ie operandLocation/opCodNum
		if ("ORG" == m_inst.GetOpCode() ||"DS" == m_inst.GetOpCode()) {
			cout << "  " << a_instLocation << "\t\t\t   " << a_instruction << endl;
			if ("ORG" == m_inst.GetOpCode()) {
				stream.str(string());
				stream << setfill('0') << setw(6) << m_inst.GetOperandVal();
				contents = stream.str();
				temp.contents = stoi(contents);
				temp.location = int(a_instLocation);
				Assembler::m_instructions.push_back(temp);
			}
			return;
		}

		// If it was AL :: numerical value expected at this point, replace the location remark with the value.
		location =  m_inst.GetOperandVal();
	}
	stream.str(string());
	stream << setfill('0') << setw(2) << a_opCodeNum << setw(4) << location;
	contents = stream.str();


	// Fill the information for the line into our instruction vector to be passed to the Emulator if no errors.
	temp.contents = stoi(contents);
	temp.location = int(a_instLocation);
	cout << "  " << a_instLocation << "\t\t" << contents << "\t   " << a_instruction << endl;
	Assembler::m_instructions.push_back(temp);
	return;
}
/*void Assembler::TranslationOutput(int a_instLocation, int a_operandLocation, int a_opCodeNum, string a_instruction, Instruction::InstructionType a_type);*/


/**/
/*
		Assembler::RunEmulator() Assembler::RunEmulator()
	NAME
		void Assembler::RunEmulator

	SYNOPSIS
		void Assembler::RunEmulator()

	DESCRIPTION
		This function will run the Emulator and output the running
		program to the user upon no errors.

	RETURNS
		((void))

*/
/**/
void Assembler::RunEmulator() {
	if (Errors::isError()) {
		Errors::DisplayErrors();
		return;
	}
	else {
	
		// If there are no errors, pass all the required information to the Emulator class.
		for (auto it = Assembler::m_instructions.begin(); it != Assembler::m_instructions.end(); ++it) {
			if (Assembler::m_emul.InsertMemory(it->location, it->contents)) continue;
			else {
				Errors::RecordError(string("out of memory."));
				cout << Errors::ReportCurrentError();
				exit(1);
			}
		}
	}

	// Remember RunProgram returns a bool, allow the user to know the emulation was success/no.
	if (m_emul.RunProgram()) {
		cout << "Termination successful" << endl;
	}
	else cout << "Termination failure, unable to complete program." << endl;
	return;
}
/*void Assembler::RunEmulator();*/