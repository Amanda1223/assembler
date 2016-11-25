//###################################################################
//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//###################################################################
//Current File	:			SymbTab.cpp
//###################################################################



//
//		Implementation of the symbol table class.
//
#include "stdafx.h"
#include "SymTab.h"

/*##################################################################
#	NAME
#		void SymbolTable::AddSymbol
#
#	SYNOPSIS
#		void SymbolTable::AddSymbol(string &a_symbol, int a_loc)
#
#			a_symbol	-->	represents the string key for the symbol 
#			a_loc		-->	location of the instruction
#
#	DESCRIPTION
#		This function is responsible for adding, and keeping track
#		of the symbol table
#
##################################################################*/
void SymbolTable::AddSymbol(string &a_symbol, int a_loc)
{
	// If the symbol is already in the symbol table, record it as multiply defined.
	map<string, int>::iterator st;
	st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end()) {
		st->second = multiplyDefinedSymbol;
		return;
	}
	// Record a the  location in the symbol table.
	m_symbolTable[a_symbol] = a_loc;
	return;
}

/*##################################################################
#	NAME
#		void SymbolTable::DisplaySymbolTable
#
#	SYNOPSIS
#		void SymbolTable::DisplaySymbolTable()
#
#	DESCRIPTION
#		This function is responsible displaying the Symbol Table
#		to the user.
#
##################################################################*/
void SymbolTable::DisplaySymbolTable() {

	return;
}