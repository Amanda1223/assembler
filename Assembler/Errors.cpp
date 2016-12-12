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

// Initialize the space for the static variables.
vector<string> Errors::m_ErrorMsgs;
int Errors::m_TotalErrors;

/*##################################################################
#	NAME
#		void Errors::InitErrorReporting
#
#	SYNOPSIS
#		void Errors::InitErrorReporting()
#
#	DESCRIPTION
#		This function is responsible for clearing out the error message
#		vector, and initialize error counting.
#
#	RETURNS
#		((void))
#
##################################################################*/
void Errors::InitErrorReporting() {
	m_TotalErrors = 0;

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
	m_TotalErrors ++;
	return;
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

	cout << m_TotalErrors << " ERROR REPORTS::" << endl;
	for (auto it = m_ErrorMsgs.begin(); it != m_ErrorMsgs.end(); ++it) {
		cout << *it << endl;
	}
}
/*void Errors::DisplayErrors();*/


/*##################################################################
#	NAME
#		string Errors::createError
#
#	SYNOPSIS
#		string Errors::createError(int a_loc, const string &a_errMsg)
#
#			a_loc		-> the location where the error occurred.
#			a_errMsg	-> the message describing the error.
#
#	DESCRIPTION
#		This function is responsible for creating an error message string
#		and returning it.
#
#	RETURNS
#		Returns a string with the error message specified.
#
##################################################################*/
string Errors::createError(int a_loc, const string &a_errMsg) {
	string location = to_string(a_loc);

	return ("Error at location " + location + " :: " + a_errMsg + '\n');
}
/*string Errors::createError(int a_loc, const string &a_errMsg);*/


/*##################################################################
#	NAME
#		string Errors::createError
#
#	SYNOPSIS
#		string Errors::createError(const string &a_errMsg)
#
#			a_errMsg	-> the message describing the error.
#
#	DESCRIPTION
#		This function is responsible for creating an error message string
#		and returning it.
#
#	RETURNS
#		Returns a string with the error message specified.
#
##################################################################*/
string Errors::createError(const string &a_errMsg) {
	return ("Error ::" + a_errMsg + '\n');
}
/*string Errors::createError(const string &a_errMsg);*/


/*##################################################################
#	NAME
#		string Errors::reportCurrentError
#
#	SYNOPSIS
#		string Errors::reportCurrentError()
#
#	DESCRIPTION
#		This function is responsible for returning the most recent
#		error message produced.
#
#	RETURNS
#		Returns a string with the latest error message.
#
##################################################################*/
string Errors::reportCurrentError() {
	if( !m_ErrorMsgs.empty() ) return m_ErrorMsgs.back();
	else return string("No Errors To Report");
}
/*string Errors::reportCurrentError();*/