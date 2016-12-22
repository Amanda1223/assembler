//Name		:	Amanda Steidl
//Course	:	CMPS361 - Software Design
//Project	:	Assembler
//Instructor:	Professor Victor Miller
//Current File	:			Emulator.h	:: Implementation of the Emulator for the VC300 program


#ifndef _EMULATOR_H
#define _EMULATOR_H

class Emulator {

public:

	const static int MEMSZ = 10000;	// The size of the memory of the VC3600.
	Emulator() {
		m_acceumulator = 0;
		memset(m_memory, 0, MEMSZ * sizeof(int));
	}
	// Records instructions and data into VC3600 memory.
	bool InsertMemory(int a_location, int a_contents);

	// Runs the VC3600 program recorded in memory.
	bool RunProgram();

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
	void Read(int a_location);
	
	//######################## VARIABLES ##############################
	int m_memory[MEMSZ];       // The memory of the VC3600.
	int m_acceumulator;			// The accumulator for the VC3600
};

#endif
