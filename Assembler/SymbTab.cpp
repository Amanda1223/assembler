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
#	RETURN
#		((void))
#
##################################################################*/
void SymbolTable::AddSymbol(string &a_symbol, int a_loc) {
	
	// If the symbol is already in the symbol table, record it as multiply defined.
	map<string, int>::iterator st;
	st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end() && !m_symbolTable.empty()) {
		st->second = multiplyDefinedSymbol;
		return;
	}

	// Record a the  location in the symbol table.
	m_symbolTable[a_symbol] = a_loc;
	return;
}
/*void SymbolTable::AddSymbol(string &a_symbol, int a_loc);*/

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
#	RETURNS
#		((void))
#
##################################################################*/
void SymbolTable::DisplaySymbolTable() {
	int symNum = 0;
	printf("Symbol #\tSymbol\tLocation\n");
	for ( auto it = m_symbolTable.begin(); it != m_symbolTable.end(); ++it, ++symNum) {
		cout << symNum << "\t\t " << it->first << "\t " << it->second << endl;
		//printf("  %d\t  %s\t  %d\n", symNum, it->first, it->second);
	}
	return;
}
/*void SymbolTable::DisplaySymbolTable();*/

/*##################################################################
#	NAME
#		bool SymbolTable::LookupSymbol
#
#	SYNOPSIS
#		bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc)
#
#		a_symbol	-->	represents the string key for the symbol 
#		a_loc		-->	location of the instruction
#
#	DESCRIPTION
#		This function is responsible for telling us whether the symbol
#		was previously defined in the symbol table or not.
#
#	RETURNS
#		boolean value, true when the symbol was found, false otherwise.
#
##################################################################*/
bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc) {

	auto st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end()) return false;
	else return true;
}
/*bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc);*/