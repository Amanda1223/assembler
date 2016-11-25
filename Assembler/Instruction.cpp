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
#		Instruction::Instruction
#
#	SYNOPSIS
#		Instruction::Instruction
#
#	DESCRIPTION
#		This constructor sets each variable to an arbitrary value,
#		in order to compare later on for opcodes and labels.
#
#	RETURNS
#		((none))
#
##################################################################*/
Instruction::Instruction() {
	//all strings can be checked using .empty ::
	m_Label = "";
	m_OpCode = "";
	m_Operand = "";
	m_instruction = "";
	
	//numerical value outside the valid opcodes (-1)
	m_NumOpCode = -1;
	m_OperandValue = -1;

	//assume the operand is anything other than a numeric value for now
	m_IsNumericOperand = false;

	//assume blank or commented first line.
	m_type = ST_Comment;
	
}

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
	if (a_buff.empty()) {
		return m_type;
	}
	m_instruction = a_buff;
	
	//storage for the single phrases/words within the buffer
	string orig_part = "";
	string tmp_copy = "";

	//in order to skip parsing if found correct instruction type
	bool skip;

	//count for each loop iteration [assuming 3 (w/o end comments)]
	int count = 1;
	cout << "Line  :: " << a_buff << endl;
	//examine each word on the line [determine instuction.]
	std::istringstream singleOut(a_buff);
	while (singleOut >> orig_part) {
		skip = false;
		
		//determine IF comment begin [should immediately return the previous value]
		if (orig_part[0] == ';') {
			cout << "AYYY dis be a comment!" << endl;
			//check if it's only the first "word" in the file, if it is then the WHOLE line is a comment
			//else it would be at the end, and we can "ignore it"
			if (count == 1) {
				m_type = ST_Comment;
			}
		}
	
		//convert the orig_part to uppercase in a separate string, then compare
		tmp_copy = orig_part;
		transform(tmp_copy.begin(), tmp_copy.end(), tmp_copy.begin(), toupper);

		//determine IF machine language
		skip = isMachineInstruct(tmp_copy, count);
		if (skip) continue;

		//determine ELSE IF assembly language
		skip = isAssemInstruct(tmp_copy, count);
		if (skip) continue;

		//determine ELSE IF label [count is equivalent to "column" labels are within first column]
		//if (count == 1) {
			//"first column" therefore label.. we hope.
			//m_Label = orig_part;
		//}
		count++;
	}
	
	return m_type;

}
/*Instruction::InstructionType Instruction::ParseInstruction(string &a_buff);*/


/*##################################################################
#	NAME
#		bool Instruction::isAssemInstruct
#
#	SYNOPSIS
#		bool Instruction::isAssemInstruct(string &a_section, int &a_count);
#
#			a_section	--> a piece of the line (parsed out by spaces)
#			a_count		--> "column" counter / word counter.
#
#	DESCRIPTION
#		This function is responsible for determining whether this is an
#		assembler instruction by comparing it to the strings of assembler list
#
#	RETURNS
#		Returns true upon matching the section with an assembler instruction,
#		false if not found (not an assembler instruction).
#
##################################################################*/
bool Instruction::isAssemInstruct(string &a_section, int &a_count) {
	for (int i = 0; i < m_AssemList.size(); i++) {
		if (m_AssemList[i].second != a_section) continue;
		else {

			//Conclusion: is an assembler instruction
			//reached the matching assembly string set necessary values
			m_OpCode = m_AssemList[i].second;
			cout << "[string] opcode: "<< m_OpCode << endl;
			m_NumOpCode = 0;
			if (m_AssemList[i].first == AT_END) m_type = ST_End;
			else m_type = ST_AssemblerInstr;
			a_count++;
			return true;
		}
	}
	return false;
}
/*bool Instruction::isAssemInstruct(string &a_section, int &a_count);*/


/*##################################################################
#	NAME
#		bool Instruction::isMachineInstruct
#
#	SYNOPSIS
#		bool Instruction::isMachineInstruct(string &a_section, int &a_count);
#
#			a_section	--> a piece of the line (parsed out by spaces)
#			a_count		--> "column" counter / word counter.
#
#	DESCRIPTION
#		This function is responsible for determining whether this is an
#		machine instruction by comparing it to the strings of machine list
#
#	RETURNS
#		Returns true upon matching the section with a machine instruction,
#		false if not found (not a machine instruction).
#
##################################################################*/
bool Instruction::isMachineInstruct(string &a_section, int &a_count) {
	for (int i = 0; i < m_NUM_OPCODE; i++) {
		if (m_MachList[i].second != a_section) continue;
		else {

			//reached the matching mach string set the opcode string & integer value
			m_OpCode = m_MachList[i].second;
			cout << "[string] Op-code : " << m_OpCode << endl;
			m_NumOpCode = m_MachList[i].first;
			cout << "[int] OpCode : " << m_NumOpCode << endl;
			m_type = ST_MachineLanguage;
			a_count++;
			return true;
		}
	}
	return false;
}
/*bool isMachineInstruct(string &a_section, int &a_count);*/