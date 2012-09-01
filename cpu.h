#ifndef CPU_H_
#define CPU_H_

#include <iostream>
#include "memory.h"

using namespace std;

class CPU {

	//Interrupts
	int SI;
	
	FILE *fp;
	char IR[4];
	char R[4];
	int IC;
	bool C;

	char buffer[100];
	int mode; //0-Master;1-Slave

		
	public:
	
	CPU();
	int load();
	void start(char *);
	void execute();
	void master();
};

#endif /*CPU_H*/
