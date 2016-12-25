//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//Current File	:			SymbTab.cpp


/*
* Current File	:	Symbtab.cpp
* Description	:	Keeps track of the symbol table which contains a symbol (label/operand)
*					along with the location of where it is located. Functionality to display
*					what is currently in the table as well as look-up.
*/

#include "stdafx.h"
#include "SymTab.h"


/**/
/*
		SymbolTable::AddSymbol() SymbolTable::AddSymbol()

	NAME
		void SymbolTable::AddSymbol

	SYNOPSIS
		void SymbolTable::AddSymbol(string &a_symbol, int a_loc)

			a_symbol	-->	represents the string key for the symbol 
			a_loc		-->	location of the instruction

	DESCRIPTION
		This funciton is responsible for adding a new operand or label along with
		the location to the symbol. Will keep track of arguments defined multiple times.

	RETURN
		((void))

*/
/**/
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


/**/
/*
		SymbolTable::DisplaySymbolTable() SymbolTable::DisplaySymbolTable()

	NAME
		void SymbolTable::DisplaySymbolTable

	SYNOPSIS
		void SymbolTable::DisplaySymbolTable()

	DESCRIPTION
		This function is responsible displaying the Symbol Table
		to the user.

	RETURNS
		((void))

*/
/**/
void SymbolTable::DisplaySymbolTable() {
	int symNum = 0;
	printf("Symbol # \tSymbol\tLocation\n");
	for ( auto it = m_symbolTable.begin(); it != m_symbolTable.end(); ++it, ++symNum) {
		cout << symNum << "\t\t " << it->first << "\t " << it->second << endl;
	}
	return;
}
/*void SymbolTable::DisplaySymbolTable();*/


/**/
/*
		SymbolTable::LookupSymbol() SymbolTable::LookupSymbol()

	NAME
		bool SymbolTable::LookupSymbol

	SYNOPSIS
		bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc)

		a_symbol	-->	represents the string key for the symbol 
		a_loc		-->	location of the instruction

	DESCRIPTION
		This function is responsible for telling us whether the symbol
		was previously defined in the symbol table or not.

	RETURNS
		boolean value, true when the symbol was found, false otherwise.
		Returns value of the location within a_loc if it exists.
		Returns undefined symbol in a_loc otherwise

*/
/**/
bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc) {
	auto it = m_symbolTable.find(a_symbol);
	if (it != m_symbolTable.end()){
		a_loc = it->second;
		return true;
	}
	else {
		a_loc = undefinedSymbol;
		return false;
	}
}
/*bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc);*/