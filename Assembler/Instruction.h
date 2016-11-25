//###################################################################
//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//###################################################################
//Current File	:			Instruction.h
//###################################################################



#pragma once
#include <sstream>
#include <algorithm>
#include <vector>
//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//

// The elements of an instruction.
class Instruction {

public:

	Instruction();

	~Instruction() { };

	// Codes to indicate the type of instruction we are processing.  Why is this inside the
	// class?
	enum InstructionType {
		ST_MachineLanguage, // A machine language instruction.
		ST_AssemblerInstr,  // Assembler Language instruction.
		ST_Comment,          // Comment or blank line
		ST_End                    // end instruction.
	};

	enum MachineOpCode {
		MT_ADD = 1,
		MT_SUB,
		MT_MULT,
		MT_DIV,
		MT_LOAD,
		MT_STORE,
		MT_READ,
		MT_WRITE,
		MT_B,
		MT_BM,
		MT_BZ,
		MT_BP,
		MT_HALT
	};

	enum AssemCode {
		AT_ORG = 1,
		AT_DC,
		AT_DS,
		AT_END
	};

	// Parse the Instruction.
	InstructionType ParseInstruction(string &a_buff);

	// Compute the location of the next instruction.
	int LocationNextInstruction(int a_loc);

	// To access the label
	inline string &GetLabel() {

		return m_Label;
	};

	// To determine if a label is blank.
	inline bool isLabel() {

		return !m_Label.empty();
	};


private:

	//##############################  (helpful) FUNCTION DECLARATIONS  ##############################
	
	//Determine the type of instruction Assembly | Machine
	bool isAssemInstruct(string &a_section, int &a_count);
	bool isMachineInstruct(string &a_section, int &a_count);

	//##############################  MEMBER VARIABLES  ##############################
	const int m_NUM_OPCODE = 13;
	const vector <pair <MachineOpCode, string>> m_MachList = { //list of machine codes / string equiv
		{ MT_ADD, "ADD" },
		{ MT_SUB, "SUB" },
		{ MT_MULT, "MULT" },
		{ MT_DIV, "DIV" },
		{ MT_LOAD, "LOAD" },
		{ MT_STORE, "STORE" },
		{ MT_READ, "READ" },
		{ MT_WRITE, "WRITE" },
		{ MT_B, "B" },
		{ MT_BM, "BM" },
		{ MT_BZ, "BZ" },
		{ MT_BP, "BP" },
		{ MT_HALT, "HALT" }
	};

	const vector <pair <AssemCode, string>> m_AssemList = { //list of possible assembly strings
		{AT_ORG, "ORG"},
		{AT_DC, "DC"},
		{AT_DS, "DS"},
		{AT_END, "END"}		
	}; 


	// The elemements of a instruction
	string m_Label;        // The label.
	string m_OpCode;       // The symbolic op code.
	string m_Operand;      // The operand.


	string m_instruction;    // The original instruction.

							 // Derived values.
	int m_NumOpCode;     // The numerical value of the op code.
	InstructionType m_type; // The type of instruction.

	bool m_IsNumericOperand;// == true if the operand is numeric.
	int m_OperandValue;   // The value of the operand if it is numeric.

};

