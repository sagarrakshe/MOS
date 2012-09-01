#ifndef MEMORY_H_
#define MEMORY_H_

#include <iostream>

class Memory {
	
	char memory[10][40];	
	int memPtr;
		
	public:

	Memory();		
	void initialize();
	void loadInMemory(char *buffer);
	void read(char *,int );
	void readline(int);
	void memmap();
	void store(char *, int);

};
#endif /**/
