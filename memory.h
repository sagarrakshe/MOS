#ifndef MEMORY_H_
#define MEMORY_H_

#include <iostream>
#include "alu.h"

class Memory {
	
	char memory[30][40];	
		
	public:
	int memPtr;
	int ptrPage;	//PageTableRegisterPage
	int frame[30];

	Memory();		
	void initialize();
	void loadInMemory(char *buffer);
	void readByte(int , char *, int );
	void readline(int ,char *);
	void memmap();
	void writeByte(int , char *, int, int );
	int ptr_initialize();
	int instrLen();
	void dispFrame();

};
#endif /*MEMORY_H_*/
