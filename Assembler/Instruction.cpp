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
#		((void))
#
##################################################################*/
Instruction::Instruction() {
	clearInfo();
}
/*Instruction::Instruction();*/


/*##################################################################
#	NAME
#		void Instruction::clearInfo
#
#	SYNOPSIS
#		void Instruction::clearInfo()
#
#	DESCRIPTION
#		Resets the variables to a "blank" value for the current line instruction
#		at hand. Therefore, not old values are placed in the wrong line instruction.
#
#	RETURNS
#		((none))
#
##################################################################*/
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
/*void Instruction::clearInfo();*/

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
	string segment = m_instruction; //storage for the single phrases/words within the buffer
	int count = 1; //count for each loop iteration [assuming 3 (w/o end comments)]
	
	//store each relevant segment on the line (parse out comments)
	parseOutComment(segment);
	std::istringstream singleOut(segment);
	vector <string> lineSegment;
	while (singleOut >> segment) {

		//parsed out comments, therefore possibility of "emptiness" :: comments at end will no longer be within the line
		if (!segment.empty()) lineSegment.push_back(segment);
	}

	//IF the lineSegment vector is empty, we had a comment on the beginning of the line
	if (lineSegment.empty()) return m_type;

	//If the lineSegment vector has MORE than 3 values within it, there are too many arguments.
	if (lineSegment.size() > 3) {
		Errors::RecordError(Errors::createError(string("too many arguments on the line.")));
		//PLACEHOLDER cout << "ERROR :: Too many arguments on a single line." << endl;
	}
	bool isAssem = false, isMach = false;
	for (unsigned column = 1; column <= lineSegment.size(); column++) {
		segment = lineSegment.at(column - 1);
		transform(segment.begin(), segment.end(), segment.begin(), toupper);
		if (column == 1 && lineSegment.size() == 3) {

			//assume label is first? "3 columns" normally consist of LABEL | INST | OPERAND
			//this could be a potential issue in Error checking.
			m_Label = lineSegment.at(column - 1);
			isValidLabel(m_Label);
			continue;
		}

		//determining these later orig INIT to FALSE, changes upon later if-elseif :: therefore the column will be incremented for the operand
		if (isMach) {
			m_Operand = lineSegment.at(column - 1);
			isValidLabel(m_Operand);

			// PARSE to check if the value is only ALPHABETICAL for ML
			if (m_IsNumericOperand = isNumeric(m_Operand)) {

				// isNumeric == true, means error for ML
				Errors::RecordError(Errors::createError("illegal use of numerical operand \"" + m_Operand + "\" on Machine Language operation."));
			}
			continue;
		}
		else if (isAssem) {
			m_Operand = lineSegment.at(column - 1);

			// PARSE to check if the value is only NUMERICAL for AL
			if (!(m_IsNumericOperand = isNumeric(m_Operand))) {

				// isNumeric == false, means error for AL instructions.
				Errors::RecordError(Errors::createError("illegal use of operand \"" + m_Operand + "\" on Assembly Language operation"));
			}

			// If numerical, can give the operand value correctly.
			else m_OperandValue = stoi(lineSegment.at(column - 1));
			continue;
		}
		if (isMach = isMachineInstruct(segment)) continue;
		else if (isAssem = isAssemInstruct(segment)) continue;
	}

	//call to check valid syntax of the entire line :: LABEL | OPCODE | OPERAND, per instruction specifications.
	if (isAssem) assemSyntaxCheck();
	else if (isMach) machSyntaxCheck();
	else {
		Errors::RecordError(Errors::createError("illegal line syntax, no language instruction found."));
		m_OpCode = unknownOpcodeNum;
		m_Operand = unknownOpcode;
	}
	return m_type;
}
/*Instruction::InstructionType Instruction::ParseInstruction(string &a_buff);*/


/*##################################################################
#	NAME
#		bool Instruction::isAssemInstruct
#
#	SYNOPSIS
#		bool Instruction::isAssemInstruct(const string &a_segment);
#
#			a_segment	--> a piece of the line (parsed out by spaces)
#
#	DESCRIPTION
#		This function is responsible for determining whether this is an
#		assembler instruction by comparing it to the strings of assembler list.
#
#	RETURNS
#		Returns TRUE upon matching the section with an assembler instruction
#				and will assign values to the corresponding member variables:
#					m_OpCode, m_NumOpCode = 0, m_type
#		FALSE if not found (not an assembler instruction).
#
##################################################################*/
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
/*bool Instruction::isAssemInstruct(const string &a_segment);*/


/*##################################################################
#	NAME
#		bool Instruction::isMachineInstruct
#
#	SYNOPSIS
#		bool Instruction::isMachineInstruct(const string &a_segment);
#
#			a_segment	--> a piece of the line (parsed out by spaces)
#
#	DESCRIPTION
#		This function is responsible for determining whether this is an
#		machine instruction by comparing it to the strings of machine list
#
#	RETURNS
#		Returns TRUE upon matching the section with a machine instruction,
#				and will assign values to corresponding member variables:
#					m_OpCode, m_NumOpCode, m_type
#
#		Returns FALSE if not found (not a machine instruction).
#
##################################################################*/
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
/*bool Instruction::isMachineInstruct(const string &a_segment);*/


/*##################################################################
#	NAME
#		void Instruction::parseOutComment
#
#	SYNOPSIS
#		void Instruction::parseOutComment(string &a_section);
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
void Instruction::parseOutComment(string &a_section) {

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
/*void Instruction::parseOutComment(string &a_section);*/

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


//Add a function for ERROR DETECTION based upon LANGUAGE INSTRUCTION

/*##################################################################
#	NAME
#		bool Instruction::isNumeric
#
#	SYNOPSIS
#		bool Instruction::isNumeric(const string &a_segment);
#
#			a_segment	--> the string being checked for numerical characters.
#
#	DESCRIPTION
#		This function is responsible for determining whether a string
#		contains all digits.
#
#	RETURNS
#		Returns FALSE if empty string
#				FALSE if a character other than digit is found
#				TRUE if all characters within the string are digits
#
##################################################################*/
bool Instruction::isNumeric(const string &a_segment) {

	//checks each digit from string.begin() to string.end() by using the isdigit built-in function 
	return !a_segment.empty() && find_if(a_segment.begin(), a_segment.end(),
		[](char check) { return !isdigit(check); }) == a_segment.end();
}
/*bool Instruction::isNumeric(const string &a_segment);*/


/*##################################################################
#	NAME
#		bool Instruction::isValidLabel
#
#	SYNOPSIS
#		bool Instruction::isValidLabel(const string &a_segment);
#
#			a_segment	--> the label/operand being checked
#
#	DESCRIPTION
#		This function is responsible for checking the label/operand :
#			(a) if it begins with a letter.
#			(b) if the rest of the string is only numbers/digits
#			(c) if it is between 1 -> 10 characters
#		If it the label/operand broke any of the specifications, we report to
#		the Error Class here.
#
#	RETURNS
#		Returns FALSE if empty string (should never happen.)
#				FALSE if string is > 10 characters
#				FALSE if string's first character is something other than a letter
#				FALSE if the string contains special characters
#				TRUE if the string meets the standards specified
#
##################################################################*/
bool Instruction::isValidLabel(const string &a_segment) {
	
	//isValid will help reporting multiple errors on one label/operand (if it is too long & has invalid char)
	bool isValid = true;
	if (a_segment.empty()) return isValid;

	if (!isalpha(a_segment[0])) {
		Errors::RecordError(Errors::createError(a_segment + " has invalid label/operand syntax, cannot start with character : " + a_segment[0]));
		isValid = false;
	}

	//check if size > 10
	if (a_segment.size() > 10) {
		Errors::RecordError(Errors::createError(a_segment + " has invalid label/operand size, must be between 1 -> 10 characters."));
		isValid = false;
	}

	//checking each character from string.being -> string.end() if it is alphanumeric
	for (auto it = a_segment.begin(); it != a_segment.end(); ++it) {
		if (!isalnum(*it)) {
			Errors::RecordError(Errors::createError(a_segment + " has invalid character : \"" + *it +"\""));
			isValid = false;
		}
	}
	return isValid;
}
/*bool Instruction::isValidLabel(const string &a_segment);*/


/*##################################################################
#	NAME
#		void Instruction::assemSyntaxCheck
#
#	SYNOPSIS
#		void Instruction::assemSyntaxCheck()
#
#	DESCRIPTION
#		This function is responsible for determining if:
#			(1) there are labels on DS/DC
#			(2) NO labels on ORG
#			(3) END contains no Label OR Operand
#			(4) If the operand exists *important
#			(5) Constant value too large?
#
#	RETURNS
#		Returns a string with the latest error message.
#
##################################################################*/
void Instruction::assemSyntaxCheck() {
	if (!m_OpCode.empty()) {
		string segment = m_OpCode;
		transform(segment.begin(), segment.end(), segment.begin(), toupper);
		auto it = m_AssemList.find(segment);
		if (it != m_AssemList.end()) {
			switch (it->second) {

				// ORG instructions cannot have a Label, *must* have NUMERICAL OPERAND
			case Instruction::AT_ORG:
				if (isLabel()) Errors::RecordError(Errors::createError("cannot have a label on the Assembly Instruction \"" + m_OpCode + "\""));
				break;

				// Require labels on DC, DS as well as NUMERICAL OPERANDS
			case Instruction::AT_DC:
			case Instruction::AT_DS:
				if (isLabel()) Errors::RecordError(Errors::createError("Assembly Instructions \"" + m_OpCode + "\"require labels."));
				break;
			default:

				//AT_END
				if (isLabel() || isOperand()) Errors::RecordError(Errors::createError("cannot have a label or operand on the END instruction."));
				return;
			}

			//check if there is in fact an operand
			if(!isOperand()){
				Errors::RecordError(Errors::createError("missing operand on assembly instruction \"" + m_OpCode + "\" "));
			}

			if (m_IsNumericOperand && isOperand() && m_OperandValue > maxValue) {
				Errors::RecordError(Errors::createError("operand value too large for memory is greater than the max size of " + maxValue));
			}
		}
		else {
			Errors::RecordError(Errors::createError("FATAL ERROR :: read assembly instruction now missing in program memory."));
			Errors::reportCurrentError();
			system("pause");
			exit(1);
		}
	}
}
/*void Instruction::assemSyntaxCheck()*/


/*##################################################################
#	NAME
#		void Instruction::machSyntaxCheck
#
#	SYNOPSIS
#		void Instruction::machSyntaxCheck()
#
#	DESCRIPTION
#		This function is responsible for determining if:
#			(1) there are operands on the instruction
#			(2) HALT does not take an operand
#
#	RETURNS
#		Returns a string with the latest error message.
#
##################################################################*/
void Instruction::machSyntaxCheck() {
	if (!m_OpCode.empty()) {
		string segment = m_OpCode;
		transform(segment.begin(), segment.end(), segment.begin(), toupper);
		auto it = m_MachList.find(segment);
		if (it != m_MachList.end()) {
			switch (it->second) {
			case Instruction::MT_HALT:
				if (isOperand()) Errors::RecordError(Errors::createError("operand not accepted on HALT machine instructions."));
				return;
			default:
				if (!isOperand()) Errors::RecordError(Errors::createError("operand required after \"" + m_OpCode +"\" machine instruction."));
				break;
			}
		}
	}
	else {
		Errors::RecordError(Errors::createError("FATAL ERROR :: read machine instruction now missing in program memory."));
		Errors::reportCurrentError();
		system("pause");
		exit(1);
	}
}
/*void Instruction::machSyntaxCheck();*/