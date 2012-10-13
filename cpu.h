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
	int TI;
	
	/*Program Control Block*/
	struct ProgramControlBlock PCB;	

	FILE *fp;
	FILE *output;
	char *R;
	char *IR;
	int IC;
	bool C;
	int realAddress;
	int terminate;

	char buffer[100];
	int mode; //0-Master;1-Slave

		
	public:
	
	CPU();
	int load();
	void start(char *);
	void execute();
	int userMode();
	void mos();
	void initialize_pcb(char *);
	void display_pcb();
};

#endif /*CPU_H*/
