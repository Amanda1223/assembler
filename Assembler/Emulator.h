//###################################################################
//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//###################################################################
//Current File	:			Emulator.h
//###################################################################



//
//		Emulator class - supports the emulation of VC3600 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class emulator {

public:

	const static int MEMSZ = 10000;	// The size of the memory of the VC3600.
	emulator() {
		m_acceumulator = 0;
		memset(m_memory, 0, MEMSZ * sizeof(int));
	}
	// Records instructions and data into VC3600 memory.
	bool insertMemory(int a_location, int a_contents);

	// Runs the VC3600 program recorded in memory.
	bool runProgram();

	//Machine Operation Code enum, for readability
	enum MachineOpCode {
		MT_ADD = 1,
		MT_SUB,
		MT_MULT,
		MT_DIV,
		MT_LOAD,
		MT_STORE,
		MT_READ,
		MT_WRITE,
		MT_B,
		MT_BM,
		MT_BZ,
		MT_BP,
		MT_HALT
	};

private:
	
	//######################## FUNCTIONS ##############################
	void Add(int a_location);
	void Sub(int a_location);
	void Mult(int a_location);
	void Div(int a_location);
	void Load(int a_location);
	void Store(int a_location);
	void Read(int a_location);
	void Write(int a_location);
	void Branch(int a_jumpLocation, int &a_newLocation);
	void BranchNeg(int a_jumpLocation, int &a_newLocation);
	void BranchZero(int a_jumpLocation, int &a_newLocation);
	void BranchPos(int a_jumpLocation, int &a_newLocation);
	void Halt();
	
	//######################## VARIABLES ##############################
	int m_memory[MEMSZ];       // The memory of the VC3600.
	int m_acceumulator;			// The accumulator for the VC3600
};

#endif
