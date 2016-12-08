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

	//initialize all member variables
	Instruction();

	~Instruction() { };

	// Codes to indicate the type of instruction we are processing.  Why is this inside the
	// class?
	enum InstructionType {
		ST_MachineLanguage,
		ST_AssemblerInstr,
		ST_Comment,
		ST_End
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

	inline int GetOpCode() {
		return m_NumOpCode;
	};

private:

	//############################## FUNCTION DECLARATIONS  ##############################
	
	//Determine the type of instruction Assembly | Machine
	bool isAssemInstruct(const string &a_segment);
	bool isMachineInstruct(const string &a_segment);

	//checking if there is a comment on the line and parsing it out.
	void checkComment(string &a_section);

	//clearing the member variables of the class, useful for parsing line by line.
	void clearInfo();

	//##############################  MEMBER VARIABLES  ##############################
	const unordered_map <string, MachineOpCode> m_MachList = { //list of machine codes / string equiv
		{ "ADD", MT_ADD },
		{ "SUB", MT_SUB },
		{ "MULT", MT_MULT },
		{ "DIV", MT_DIV },
		{ "LOAD", MT_LOAD },
		{ "STORE", MT_STORE },
		{ "READ", MT_READ },
		{ "WRITE", MT_WRITE },
		{ "B", MT_B },
		{ "BM", MT_BM },
		{ "BZ", MT_BZ },
		{ "BP", MT_BP },
		{ "HALT", MT_HALT }
	};

	const unordered_map <string, AssemCode> m_AssemList = { //list of possible assembly strings
		{ "ORG", AT_ORG },
		{ "DC", AT_DC },
		{ "DS", AT_DS },
		{ "END", AT_END }
	}; 


	// The elemements of an instruction
	string m_Label;        // The label.
	string m_OpCode;       // The symbolic op code.
	string m_Operand;      // The operand.

	string m_instruction;    // The original instruction.

	// Derived values.
	int m_NumOpCode;     // The numerical value of the op code.
	InstructionType m_type; // The type of instruction.

	bool m_IsNumericOperand;// == true if the operand is numeric. :: will have to check this specifically in pass II for errors
	int m_OperandValue;   // The value of the operand if it is numeric.
};