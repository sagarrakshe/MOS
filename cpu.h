#ifndef CPU_H_
#define CPU_H_

#include <iostream>
#include "memory.h"
#include "cardreader.h"

using namespace std;

struct ProgramControlBlock {
	int jobId;
	
	int TTL;
	int TLL;

	int TTC;
	int TLC;

	int PTR;
};

class CPU {

	/*Interrupts*/
	int SI;
	int PI;

	/*Program Control Block*/
	struct ProgramControlBlock PCB;	

	FILE *fp;
	FILE *output;
	char IR[4];
	char R[5];
	int IC;
	bool C;

	char buffer[100];
	int mode; //0-Master;1-Slave

		
	public:
	
	CPU();
	int load();
	void start(char *);
	void execute();
	void mos();
	void initialize_pcb(char *);
	void display_pcb();
};

#endif /*CPU_H*/
