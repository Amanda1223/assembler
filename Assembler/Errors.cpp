//###################################################################
//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//###################################################################
//Current File	:			Errors.cpp
//###################################################################

#include "stdafx.h"
#include "Errors.h"

//initialize space for the error messages?
vector<string> Errors::m_ErrorMsgs;

/*##################################################################
#	NAME
#		void Errors::InitErrorReporting
#
#	SYNOPSIS
#		void Errors::InitErrorReporting()
#
#	DESCRIPTION
#		This function is responsible for clearing out the error message
#		vector.
#
#	RETURNS
#		((void))
#
##################################################################*/
void Errors::InitErrorReporting() {

	//Clearing out any errors that would possibly be within the Error Class already
	if (m_ErrorMsgs.empty()) {
		return;
	}
	m_ErrorMsgs.clear();
	return;
}
/*void Errors::InitErrorReporting()*/


/*##################################################################
#	NAME
#		void Errors::RecordError
#
#	SYNOPSIS
#		void Errors::RecordError(string &a_emsg)
#
#	DESCRIPTION
#		This function is responsible for adding Error messages (strings) to
#		the Error message vector.
#
#	RETURNS
#		((void))
#
##################################################################*/
void Errors::RecordError(string &a_emsg) {
	if (a_emsg.empty()) return;

	m_ErrorMsgs.push_back(a_emsg);
}
/*void Errors::RecordError(string &a_emsg);*/


/*##################################################################
#	NAME
#		void Errors::DisplayErrors
#
#	SYNOPSIS
#		void Errors::DisplayErrors()
#
#	DESCRIPTION
#		This function is responsible for displaying the error messages,
#		if any, acummulated to the user.
#
#	RETURNS
#		((void))
#
##################################################################*/
void Errors::DisplayErrors() {
	if (m_ErrorMsgs.empty()) {
		cout << "Translated Successfully!" << endl;
		return;
	}

	cout << "ERROR REPORTS::" << endl;
	for (auto it = m_ErrorMsgs.begin(); it != m_ErrorMsgs.end(); ++it) {
		cout << *it << endl;
	}
}
/*void Errors::DisplayErrors();*/

//Create Error? Line + Location + Num + Error + Reason
string Errors::createError(int a_line, int a_loc, string &a_errMsg) {
	string line = to_string(a_line);
	string location = to_string(a_loc);

	return ("Error on line " + line + " location " + location + " :: " + a_errMsg + '\n');
}

string Errors::reportCurrentError() {
	if( !m_ErrorMsgs.empty() ) return m_ErrorMsgs.back();
	else return string("No Errors To Report");
}