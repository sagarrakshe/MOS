#ifndef MEMORY_H_
#define MEMORY_H_

#include <iostream>
#include "alu.h"

class Memory {
	
	char memory[10][40];	
	int frame[30];
		
	public:
	int memPtr;

	Memory();		
	void initialize();
	void loadInMemory(char *buffer);
	void readByte(char *,int );
	void readline(int ,char *);
	void memmap();
	void writeByte(char *, int);

};
#endif /**/
