#ifndef MEMORY_H_
#define MEMORY_H_

#include <iostream>

class Memory {
	
	char memory[10][40];	
		
	public:
	int memPtr;

	Memory();		
	void initialize();
	void loadInMemory(char *buffer);
	void readByte(char *,int );
	void readline(int);
	void memmap();
	void writeByte(char *, int);

};
#endif /**/
