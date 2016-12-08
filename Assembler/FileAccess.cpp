//###################################################################
//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//###################################################################
//Current File	:			FileAccess.cpp
//###################################################################



//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/*##################################################################
#	NAME
#		FileAccess::FileAccess
#
#	SYNOPSIS
#		FileAccess::FileAccess(int argc, char *argv[]);
#
#			argc	--> number of arguments within the command line
#			argv	-->	file name input by the user
#
#	DESCRIPTION
#		This constructor attempts to open the file from a command
#		line argument. If it could not be opened or if the file does
#		not exist, it will exit the program.
#
#	RETURNS
#		((none))
#
##################################################################*/
FileAccess::FileAccess(int argc, char *argv[])
{
	// Check that there is exactly one run time parameter.
	if (argc != 2) {
		cerr << "Usage: Assem <FileName>" << endl;
		exit(1);
	}
	// Open the file.  One might question if this is the best place to open the file.
	// One might also question whether we need a file access class.
	m_sfile.open(argv[1], ios::in);

	// If the open failed, report the error and terminate.
	if (!m_sfile) {
		cerr << "Source file could not be opened, assembler terminated."
			<< endl;
		exit(1);
	}
}
/*FileAccess::FileAccess(int argc, char *argv[]);*/


/*##################################################################
#	NAME
#		FileAccess::~FileAccess
#
#	SYNOPSIS
#		FileAccess::~FileAccess();
#
#	DESCRIPTION
#		This deconstructor closes the open file
#
#	RETURNS
#		((none))
#
##################################################################*/
FileAccess::~FileAccess()
{
	m_sfile.close();
}
/*FileAccess::~FileAccess()*/


/*##################################################################
#	NAME
#		FileAccess::GetNextLine
#
#	SYNOPSIS
#		bool FileAccess::GetNextLine(string &a_buff);
#
#			a_buff	--> a single line within the file that was opened.
#
#	DESCRIPTION
#		This function attempts to read a line from the open file,
#		if it is at the end of the file then nothing will be read in.
#
#	RETURNS
#		Returns true indicating success, and false if there
#		is no more data in the file.
#
##################################################################*/
bool FileAccess::GetNextLine(string &a_buff)
{
	//Reached end-of-file | there's no more data to be read
	if (m_sfile.eof()) {
		return false;
	}
	getline(m_sfile, a_buff);

	//Return indicating succes; file string read into a_buff.
	return true;
}
/*bool FileAccess::GetNextLine(string &a_buff);*/


/*##################################################################
#	NAME
#		FileAccess::rewind
#
#	SYNOPSIS
#		void FileAccess::rewind();
#
#	DESCRIPTION
#		This function
#
#	RETURNS
#		((void))
#
##################################################################*/
void FileAccess::rewind()
{
	// Clean tall file flags and go back to the beginning of the file.
	m_sfile.clear();
	m_sfile.seekg(0, ios::beg);
}
/*void FileAccess::rewind()*/