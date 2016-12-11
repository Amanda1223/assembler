//###################################################################
//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//###################################################################
//Current File	:			Errors.h
//###################################################################



//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include <vector>

class Errors {

public:

	// Initializes error reports.
	static void InitErrorReporting();

	// Records an error message.
	static void RecordError(string &a_emsg);

	// Displays the collected error message.
	static void DisplayErrors();

	// Detailing the messages
	static string createError( int a_loc, const string &a_errMsg );
	static string createError(const string &a_errMsg);

	// Display the most recent error recorded
	static string reportCurrentError();

private:
	static int m_TotalErrors;
	static vector <string> m_ErrorMsgs;
};
#endif