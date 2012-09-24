#ifndef MEMORY_H_
#define MEMORY_H_

#include <iostream>
#include "alu.h"

class Memory {
	
	char memory[30][40];	
	int frame[30];
		
	public:
	int memPtr;
	int ptrPage;	//PageTableRegisterPage

	Memory();		
	void initialize();
	void loadInMemory(char *buffer);
	void readByte(int , char *, int );
	void readline(int ,char *);
	void memmap();
	void writeByte(char *, int);
	int ptr_initialize();
	int instrLen();

};
#endif /*MEMORY_H_*/
