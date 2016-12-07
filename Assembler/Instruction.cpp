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
	clearInfo();
}
/*Instruction::Instruction();*/

void Instruction::clearInfo() {
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
	clearInfo();
	if (a_buff.empty()) {
		return m_type;
	}
	m_instruction = a_buff;
	string orig_part = ""; //storage for the single phrases/words within the buffer
	string tmp_copy = ""; //copy for comparison purposes. (will use this only for uppercase copies)
	int count = 1; //count for each loop iteration [assuming 3 (w/o end comments)]

	cout << a_buff << endl;

	//store each relevant segment on the line (parse out comments)
	std::istringstream singleOut(a_buff);
	vector <string> lineSegment;
	while (singleOut >> orig_part) {
		checkComment(orig_part);

		//parsed out comments, therefore possibility of "emptiness" :: comments at end will no longer be within the line
		if (!orig_part.empty()) lineSegment.push_back(orig_part);
	}

	//IF the lineSegment vector is empty, we had a comment on the beginning of the line
	if (lineSegment.empty()) return m_type;
	bool isAssem = false, isMach = false;
	for (int column = 1; column <= lineSegment.size(); column++) {
		
		//Comments already done.
		tmp_copy = lineSegment.at(column - 1);
		transform(tmp_copy.begin(), tmp_copy.end(), tmp_copy.begin(), toupper);
		if (column == 1 && lineSegment.size() == 3) {

			//assume label is first? "3 columns" normally consist of LABEL | INST | OPERAND
			//this could be a potential issue in Error checking.
			m_Label = lineSegment.at(column - 1);
			continue;
		}

		//determining these later orig INIT to FALSE, changes upon later if-elseif :: therefore the column will be incremented for the operand
		if (isMach) {
			m_Operand = lineSegment.at(column - 1);
			m_IsNumericOperand = false;
			isMach = false;
			continue;
		}
		else if (isAssem) {
			m_Operand = lineSegment.at(column - 1);

			//assuming numeric will always be true in an assembly language
			m_IsNumericOperand = true;
			m_OperandValue = stoi(lineSegment.at(column - 1));
			isAssem = false;
			continue;
		}
		if (isMach = isMachineInstruct(tmp_copy)) {
			continue;
		}
		else if (isAssem = isAssemInstruct(tmp_copy)) {
			continue;
		}
	}
	return m_type;
}
/*Instruction::InstructionType Instruction::ParseInstruction(string &a_buff);*/


/*##################################################################
#	NAME
#		bool Instruction::isAssemInstruct
#
#	SYNOPSIS
#		bool Instruction::isAssemInstruct(const string &a_section, int &a_count);
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
bool Instruction::isAssemInstruct(const string &a_section, int &a_count) {
	auto it = m_AssemList.find(a_section);
	if (it != m_AssemList.end()) {
		
		//found the opcode string
		m_OpCode = it->first;
		m_NumOpCode = 0; //AL does not have opcodes
		if (it->second == AT_END) m_type = ST_End;
		else m_type = ST_AssemblerInstr;
		if (a_count = 1) {

			//"no label" case
			a_count += 1;
		}
		return true;
	}
	return false;
}
/*bool Instruction::isAssemInstruct(const string &a_section, int &a_count);*/

bool Instruction::isAssemInstruct(const string &a_segment) {
	auto it = m_AssemList.find(a_segment);
	if (it != m_AssemList.end()) {

		//found the opcode string
		m_OpCode = it->first;
		m_NumOpCode = 0; //AL does not have opcodes
		if (it->second == AT_END) m_type = ST_End;
		else m_type = ST_AssemblerInstr;
		return true;
	}
	return false;
}

/*##################################################################
#	NAME
#		bool Instruction::isMachineInstruct
#
#	SYNOPSIS
#		bool Instruction::isMachineInstruct(const string &a_section, int &a_count);
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
bool Instruction::isMachineInstruct(const string &a_section, int &a_count) {
	auto it = m_MachList.find(a_section);
	if (it != m_MachList.end()) {
		m_OpCode = it->first;
		m_NumOpCode = it->second; //second value stored is the ML value
		m_type = ST_MachineLanguage;
		if (a_count == 1) {
			a_count++;
		}
		return true;
	}
	else return false;
}
/*bool Instruction::isMachineInstruct(const string &a_section, int &a_count);*/

bool Instruction::isMachineInstruct(const string &a_segment) {
	auto it = m_MachList.find(a_segment);
	if (it != m_MachList.end()) {
		m_OpCode = it->first;
		m_NumOpCode = it->second; //second value stored is the ML value
		m_type = ST_MachineLanguage;
		return true;
	}
	else return false;
}


/*##################################################################
#	NAME
#		void Instruction::checkComment
#
#	SYNOPSIS
#		void Instruction::checkComment(string &a_section);
#
#			a_section	--> a piece of the line (parsed out by spaces)
#
#	DESCRIPTION
#		This function is responsible for checking if there is a comment
#		in the middle of a "phrase" ie if the user placed a comment next to
#		an operand or a language operation.
#
#	RETURNS
#		((void))
#
##################################################################*/
void Instruction::checkComment(string &a_section) {

	//check if the "single word" is attached to a comment!
	auto commentSplit = a_section.find(";");
	if (commentSplit != string::npos) {

		//there was a comment found, parse the string
		a_section = a_section.substr(0, commentSplit);

		//empty string if ; was at the beginning.
		return;
	}
	return;
}
/*void Instruction::checkComment(string &a_section);*/

/*##################################################################
#	NAME
#		int Instruction::LocationNextInstruction
#
#	SYNOPSIS
#		int Instruction::LocationNextInstruction(int a_loc);
#
#			a_loc	--> location of instruction
#
#	DESCRIPTION
#		This function is responsible for determining the upcoming instruction
#		location, whether it was marked by an instruction, or 
#		just a line incrementation.
#
#	RETURNS
#		Returns the INTEGER value of the next instruction.
#
##################################################################*/
int Instruction::LocationNextInstruction(int a_loc) {
	if (m_type == ST_AssemblerInstr && !m_Operand.empty()) {
		//should look for the location @m_OPCode
		if (m_AssemList.at(m_OpCode) == AT_ORG) {

			//ORG tells us WHERE to go.
			if (m_IsNumericOperand) return m_OperandValue;
		}
		else if (m_AssemList.at(m_OpCode) == AT_DS) {

			//adding "space" therefore INCREASE the location by the operand value
			if (m_IsNumericOperand) return a_loc + m_OperandValue;
		}
	}
	return a_loc + 1;
}
/*int Instruction::LocationNextInstruction(int a_loc);*/