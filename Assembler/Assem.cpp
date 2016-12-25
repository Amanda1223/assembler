//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//Current File	:			Assem.cpp




/* Assembler main program.*/


#include "stdafx.h"
#include <stdio.h>

#include "Assembler.h"

int main(int argc, char *argv[])
{
	Assembler assem(argc, argv);

	// Establish the location of the labels:
	assem.PassI();
	system("pause");

	// Display the symbol table.
	assem.DisplaySymbolTable();
	system("pause");

	// Output the symbol table and the translation.
	assem.PassII();
	system("pause");

	// Run the emulator on the VC3600 program that was generated in Pass II.
	assem.RunEmulator();
	system("pause");
	
	return 0;
}