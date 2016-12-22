//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//Current File	:			Instruction.cpp


/*
* Current File	:	Instruction.cpp
* Description	:	Has a line from the file to parse through determining the language instruction
*					while also parsing the line to ensure proper use of labels/opcodes/operands.
*					Error reporting occurs line by line, and will accumulate if multiple 'reasonings'
*					for the error to occur happen.
*					Note : Errors are not printed, just checked and created by use of the Error class.
*/


#include "stdafx.h"
#include "Instruction.h"


/**/
/*
		Instruction::Instruction()	Instruction::Instruction()

	NAME
		Instruction::Instruction

	SYNOPSIS
		Instruction::Instruction

	DESCRIPTION
		This constructor sets each variable to an arbitrary value,
		in order to compare later on for opcodes and labels.

	RETURNS
		((void))

*/
/**/
Instruction::Instruction() {
	ClearInfo();
}
/*Instruction::Instruction();*/


/**/
/*
 		Instruction::ClearInfo() Instruction::ClearInfo()	

	NAME
		void Instruction::ClearInfo

	SYNOPSIS
		void Instruction::ClearInfo()

	DESCRIPTION
		Resets the variables to a "blank" value for the current line instruction
		at hand. Therefore, not old values are placed in the wrong line instruction.

	RETURNS
		((none))

*/
/**/
void Instruction::ClearInfo() {
	// All strings can be checked using .empty ::
	m_label = "";
	m_opCode = "";
	m_operand = "";
	m_instruction = "";

	// Numerical value outside the valid opcodes (-1)
	m_opCodeNum = -1;
	m_operandValue = -1;

	// Assume the operand is anything other than a numeric value for now
	m_isNumericOperand = false;

	// Assume blank or commented first line.
	m_type = ST_Comment;
}
/*void Instruction::ClearInfo();*/


/**/
/*
	Instruction::ParseInstruction() Instruction::ParseInstruction()

	NAME
		Instruction::InstructionType Instruction::ParseInstruction

	SYNOPSIS
		Instruction::InstructionType Instruction::ParseInstruction(string &a_buff);

			a_buff	--> a single line within the file

	DESCRIPTION
		This function is responsible for examining the line, and
		determining the type of instruction the line contains.
		OPTIONS : ST_MachineLanguage | ST_AssemblerInstr | ST_Comment
					ST_End

	RETURNS
		Returns the instruction type of the current line (string
		which is passed in).

*/
/**/
Instruction::InstructionType Instruction::ParseInstruction(string &a_buff) {
	ClearInfo();
	
	// Return as comment if nothing was in the buffer
	if (a_buff.empty()) {
		return m_type;
	}
	m_instruction = a_buff;
	string segment = m_instruction;
	int count = 1;
	
	// Storing relevant information [Label | Opcode | Operand] while parsing out comments.
	ParseOutComment(segment);
	std::istringstream singleOut(segment);
	vector <string> lineSegment;
	while (singleOut >> segment) {

		// While parsing out comments, allows possibility of emptiness that must be handled.
		if (!segment.empty()) lineSegment.push_back(segment);
	}
	if (lineSegment.empty()) return m_type;

	// Max size of a VC3600 operation is 3 [Label | Opcode | Operand] 
	if (lineSegment.size() > 3) {
		Errors::RecordError(Errors::CreateError(string("too many arguments on the line.")));
	}
	bool isAssem = false, isMach = false;
	for (unsigned column = 1; column <= lineSegment.size(); column++) {
		segment = lineSegment.at(column - 1);

		// Handling mismatch case in an instruction, or label name
		transform(segment.begin(), segment.end(), segment.begin(), toupper);
		if (column == 1 && lineSegment.size() == 3) {
			m_label = lineSegment.at(column - 1);
			isValidLabel(m_label);
			continue;
		}

		// Determining these later orig INIT to FALSE, changes upon later if-elseif :: therefore the column will be incremented for the operand
		if (isMach) {
			m_operand = lineSegment.at(column - 1);
			isValidLabel(m_operand);

			// Checking for alphanumerical characters
			if (m_isNumericOperand = (!isAlphaNumeric(m_operand))) {
				Errors::RecordError(Errors::CreateError("illegal use of numerical operand \"" + m_operand + "\" on Machine Language operation."));
			}
			continue;
		}
		else if (isAssem) {
			m_operand = lineSegment.at(column - 1);

			// Checking for numerical operand
			if (!(m_isNumericOperand = isNumeric(m_operand))) {
				Errors::RecordError(Errors::CreateError("illegal use of operand \"" + m_operand + "\" on Assembly Language operation"));
			}
			else m_operandValue = stoi(lineSegment.at(column - 1));
			continue;
		}
		if (isMach = isMachineInstruct(segment)) continue;
		else if (isAssem = isAssemInstruct(segment)) continue;
	}

	// Calls to check valid syntax / semantics of a complete line.
	if (isAssem) AssemSyntaxCheck();
	else if (isMach) MachSyntaxCheck();
	else {
		Errors::RecordError(Errors::CreateError("illegal line syntax, no language instruction found."));
		m_opCode = m_MISSING_OPCODE;
		m_operand = m_STR_UNKNOWN_OP;
	}
	return m_type;
}
/*Instruction::InstructionType Instruction::ParseInstruction(string &a_buff);*/


/**/
/*
	Instruction::isAssemInstruct() Instruction::isAssemInstruct()

	NAME
		bool Instruction::isAssemInstruct

	SYNOPSIS
		bool Instruction::isAssemInstruct(const string &a_segment);

			a_segment	--> a piece of the line (parsed out by spaces)

	DESCRIPTION
		This function is responsible for determining whether this is an
		assembler instruction by comparing it to the strings of assembler list.

	RETURNS
		Returns TRUE upon matching the section with an assembler instruction
				and will assign values to the corresponding member variables:
					m_opCode, m_opCodeNum = 0, m_type
		FALSE if not found (not an assembler instruction).

*/
/**/
bool Instruction::isAssemInstruct(const string &a_segment) {
	auto it = m_AssemList.find(a_segment);
	if (it != m_AssemList.end()) {
		m_opCode = it->first;
		m_opCodeNum = 0;
		if (it->second == AT_END) m_type = ST_End;
		else m_type = ST_AssemblerInstr;
		return true;
	}
	return false;
}
/*bool Instruction::isAssemInstruct(const string &a_segment);*/


/**/
/*
	Instruction::isMachineInstruct() Instruction::isMachineInstruct()

	NAME
		bool Instruction::isMachineInstruct

	SYNOPSIS
		bool Instruction::isMachineInstruct(const string &a_segment);

			a_segment	--> a piece of the line (parsed out by spaces)

	DESCRIPTION
		This function is responsible for determining whether this is an
		machine instruction by comparing it to the strings of machine list

	RETURNS
		Returns TRUE upon matching the section with a machine instruction,
				and will assign values to corresponding member variables:
					m_opCode, m_opCodeNum, m_type

		Returns FALSE if not found (not a machine instruction).

*/
/**/
bool Instruction::isMachineInstruct(const string &a_segment) {
	auto it = m_MachList.find(a_segment);
	if (it != m_MachList.end()) {
		m_opCode = it->first;
		m_opCodeNum = it->second;
		m_type = ST_MachineLanguage;
		return true;
	}
	else return false;
}
/*bool Instruction::isMachineInstruct(const string &a_segment);*/


/**/
/*
	Instruction::ParseOutComment() Instruction::ParseOutComment()

	NAME
		void Instruction::ParseOutComment

	SYNOPSIS
		void Instruction::ParseOutComment(string &a_section);

			a_section	--> a piece of the line (parsed out by spaces)

	DESCRIPTION
		This function is responsible for checking if there is a comment
		in the middle of a "phrase" ie if the user placed a comment next to
		an operand or a language operation.

	RETURNS
		((void))

*/
/**/
void Instruction::ParseOutComment(string &a_section) {

	//check if the "single word" is attached to a comment!
	auto commentSplit = a_section.find(";");
	if (commentSplit != string::npos) {

		//there was a comment found, parse the string
		a_section = a_section.substr(0, commentSplit);
		return;
	}
	return;
}
/*void Instruction::ParseOutComment(string &a_section);*/


/**/
/*
	Instruction::LocationNextInstruction() Instruction::LocationNextInstruction()

	NAME
		int Instruction::LocationNextInstruction

	SYNOPSIS
		int Instruction::LocationNextInstruction(int a_loc);

			a_loc	--> location of instruction

	DESCRIPTION
		This function is responsible for determining the upcoming instruction
		location, whether it was marked by an instruction, or 
		just a line incrementation.

	RETURNS
		Returns the INTEGER value of the next instruction.

*/
/**/
int Instruction::LocationNextInstruction(int a_loc) {
	if (m_type == ST_AssemblerInstr && !m_operand.empty()) {
		//should look for the location @m_OPCode
		if (m_AssemList.at(m_opCode) == AT_ORG) {

			//ORG tells us WHERE to go.
			if (m_isNumericOperand) return m_operandValue;
		}
		else if (m_AssemList.at(m_opCode) == AT_DS) {

			//adding "space" therefore INCREASE the location by the operand value
			if (m_isNumericOperand) return a_loc + m_operandValue;
		}
	}
	return a_loc + 1;
}
/*int Instruction::LocationNextInstruction(int a_loc);*/


/**/
/*
	Instruction::isNumeric() Instruction::isNumeric()

	NAME
		bool Instruction::isNumeric

	SYNOPSIS
		bool Instruction::isNumeric(const string &a_segment);

			a_segment	--> the string being checked for numerical characters.

	DESCRIPTION
		This function is responsible for determining whether a string
		contains all digits.

	RETURNS
		Returns FALSE if empty string
				FALSE if a character other than digit is found
				TRUE if all characters within the string are digits

*/
/**/
bool Instruction::isNumeric(const string &a_segment) {

	//checks each digit from string.begin() to string.end() by using the isdigit built-in function 
	return !a_segment.empty() && find_if(a_segment.begin(), a_segment.end(),
		[](char check) { return !isdigit(check); }) == a_segment.end();
}
/*bool Instruction::isNumeric(const string &a_segment);*/


/**/
/*
	Instruction::isAlphaNumeric() Instruction::isAlphaNumeric()

	NAME
		bool Instruction::isAlphaNumeric

	SYNOPSIS
		bool Instruction::isAlphaNumeric(const string &a_segment);

			a_segment	--> the string being checked for numerical characters.

	DESCRIPTION
		This function is responsible for determining whether a string
		contains all digits.

	RETURNS
		Returns FALSE if empty string
				FALSE if a character other than digit is found
				TRUE if all characters within the string are digits
*/
/**/
bool Instruction::isAlphaNumeric(const string &a_segment) {
	if (!isalpha(a_segment.at(0))) {
		return false;
	}

	//checks each digit from string.begin() to string.end() by using the isalnum built-in function 
	return !a_segment.empty() && find_if(a_segment.begin(), a_segment.end(),
		[](char check) { return !isalnum(check); }) == a_segment.end();
}
/*bool Instruction::isAlphaNumeric(const string &a_segment);*/


/**/
/*
	Instruction::isValidLabel() Instruction::isValidLabel()

	NAME
		bool Instruction::isValidLabel

	SYNOPSIS
		bool Instruction::isValidLabel(const string &a_segment);

			a_segment	--> the label/operand being checked

	DESCRIPTION
		This function is responsible for checking the label/operand :
			(a) if it begins with a letter.
			(b) if the rest of the string is only numbers/digits
			(c) if it is between 1 -> 10 characters
		If it the label/operand broke any of the specifications, we report to
		the Error Class here.

	RETURNS
		Returns FALSE if empty string (should never happen.)
				FALSE if string is > 10 characters
				FALSE if string's first character is something other than a letter
				FALSE if the string contains special characters
				TRUE if the string meets the standards specified

*/
/**/
bool Instruction::isValidLabel(const string &a_segment) {
	
	//isValid will help reporting multiple errors on one label/operand (if it is too long & has invalid char)
	bool isValid = true;
	if (a_segment.empty()) return isValid;

	if (!isalpha(a_segment[0])) {
		Errors::RecordError(Errors::CreateError(a_segment + " has invalid label/operand syntax, cannot start with character : " + a_segment[0]));
		isValid = false;
	}

	//check if size > 10
	if (a_segment.size() > 10) {
		Errors::RecordError(Errors::CreateError(a_segment + " has invalid label/operand size, must be between 1 -> 10 characters."));
		isValid = false;
	}

	//checking each character from string.being -> string.end() if it is alphanumeric
	for (auto it = a_segment.begin(); it != a_segment.end(); ++it) {
		if (!isalnum(*it)) {
			Errors::RecordError(Errors::CreateError(a_segment + " has invalid character : \"" + *it +"\""));
			isValid = false;
		}
	}
	return isValid;
}
/*bool Instruction::isValidLabel(const string &a_segment);*/


/**/
/*
	Instruction::AssemSyntaxCheck() Instruction::AssemSyntaxCheck()

	NAME
		void Instruction::AssemSyntaxCheck

	SYNOPSIS
		void Instruction::AssemSyntaxCheck()

	DESCRIPTION
		This function is responsible for determining if:
			(1) there are labels on DS/DC
			(2) NO labels on ORG
			(3) END contains no Label OR Operand
			(4) If the operand exists *important
			(5) Constant value too large?

	RETURNS
		Returns a string with the latest error message.

*/
/**/
void Instruction::AssemSyntaxCheck() {
	if (!m_opCode.empty()) {
		string segment = m_opCode;
		transform(segment.begin(), segment.end(), segment.begin(), toupper);
		auto it = m_AssemList.find(segment);
		if (it != m_AssemList.end()) {
			switch (it->second) {

				// ORG instructions cannot have a Label, *must* have NUMERICAL OPERAND
			case Instruction::AT_ORG:
				if (isLabel()) Errors::RecordError(Errors::CreateError("cannot have a label on the Assembly Instruction \"" + m_opCode + "\""));
				break;

				// Require labels on DC, DS as well as NUMERICAL OPERANDS
			case Instruction::AT_DC:
			case Instruction::AT_DS:
				if (!isLabel()) Errors::RecordError(Errors::CreateError("Assembly Instructions \"" + m_opCode + "\"require a label."));
				break;
			default:

				//AT_END
				if (isLabel() || isOperand()) Errors::RecordError(Errors::CreateError("cannot have a label or operand on the END instruction."));
				return;
			}

			//check if there is in fact an operand
			if(!isOperand()){
				Errors::RecordError(Errors::CreateError("missing operand on assembly instruction \"" + m_opCode + "\" "));
			}

			if (m_isNumericOperand && isOperand() && m_operandValue > m_MAX_VALUE) {
				Errors::RecordError(Errors::CreateError("operand value too large for memory is greater than the max size of " + m_MAX_VALUE));
			}
		}
		else {
			Errors::RecordError(Errors::CreateError("FATAL ERROR :: read assembly instruction now missing in program memory."));
			Errors::ReportCurrentError();
			system("pause");
			exit(1);
		}
	}
}
/*void Instruction::AssemSyntaxCheck()*/


/**/
/*
	Instruction::MachSyntaxCheck() Instruction::MachSyntaxCheck()

	NAME
		void Instruction::MachSyntaxCheck

	SYNOPSIS
		void Instruction::MachSyntaxCheck()

	DESCRIPTION
		This function is responsible for determining if:
			(1) there are operands on the instruction
			(2) HALT does not take an operand

	RETURNS
		Returns a string with the latest error message.

*/
/**/
void Instruction::MachSyntaxCheck() {

	// Check the machine operation code for valid codes, and check operands / labels for certain operations
	if (!m_opCode.empty()) {
		string segment = m_opCode;
		transform(segment.begin(), segment.end(), segment.begin(), toupper);
		auto it = m_MachList.find(segment);
		if (it != m_MachList.end()) {
			switch (it->second) {
			case Instruction::MT_HALT:
				if (isOperand()) Errors::RecordError(Errors::CreateError("operand not accepted on HALT machine instructions."));
				return;
			default:
				if (!isOperand()) Errors::RecordError(Errors::CreateError("operand required after \"" + m_opCode +"\" machine instruction."));
				break;
			}
		}
	}

	// There was no operation code on a line, report a fatal error.
	else {
		Errors::RecordError(Errors::CreateError("FATAL ERROR :: read machine instruction now missing in program memory."));
		Errors::ReportCurrentError();
		system("pause");
		exit(1);
	}
}
/*void Instruction::MachSyntaxCheck();*/