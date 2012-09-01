#include "lineprinter.h"
#include "cardreader.h"
#include "cpu.h"
#include "memory.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern Memory *m;
extern LinePrinter *lp;
extern CardReader *cr;
extern char Buffer[10][40];
extern int BuffPtr;

extern void BuffInitialize();
extern void loadInBuffer(char *);
extern void BuffMap();
//extern CPU *c;

#define AMJ(x) strstr(x,"$AMJ")
#define H(x) strstr(x,"H")
#define DTA(x) strstr(x,"$DTA")
#define END(x) strstr(x,"$END")

using namespace std;

CPU::CPU() {
	m = new Memory();
	lp = new LinePrinter();
	cr = new CardReader();
}

int CPU::load(){
	
	int i=0;
	while(1) 
	{	
		fgets(buffer,42,fp);
		if(!feof(fp))
		{
			if(AMJ(buffer) && strlen(buffer)==17)
			{
				m->initialize();
				BuffInitialize();
				
				cout<<"JOB:-"<<++i<<endl;
				fgets(buffer,42,fp);
				//cout<<"Program Card\n";
				while(!DTA(buffer)){
					m->loadInMemory(buffer);
					fgets(buffer,42,fp);					
				}
				fgets(buffer,42,fp);				
				cout<<"Data Card\n";
				while(!END(buffer)){
					//cout<<buffer;
					loadInBuffer(buffer);
					fgets(buffer,42,fp);					
				}
				cout<<endl;
				//m->memmap();
				BuffMap();
			}
			else			
			{
				cout<<"Error in JOB:-"<<++i<<endl;
				do{
					fgets(buffer,42,fp);
				}while(!END(buffer));
				
			}
			
			cout<<endl;
		}
		
		else
			break;
	}

	return 0;
}

void CPU::start(char *fileName) {
	
	fp=fopen(fileName,"r");
	
	/*check for file opening*/
	if(!fp) {
		perror("Error: ");
		exit(1);	
	}
	
	/*after succesfully opening call load function to load in mem.*/
	this->load();
}


void CPU::execute() {
	
	char reg[4]; 

	while(1)
	{
		m->read(IR,IC);
		IC++;
	
		if(IR[0]=='G' && IR[1]=='D')
			SI=1;	

		else if(IR[0]=='P' && IR[1]=='D')
			SI=2;	

		else if(IR[0]=='H')
			SI=3;	
	
		else if(IR[0]=='L' && IR[1]=='R')
		{
			m->read(R,atoi(IR+2));
		}
	
		else if(IR[0]=='S' && IR[1]=='R')
		{
			m->store(R,atoi(IR+2));
		}

		else if(IR[0]=='C' && IR[1]=='R')
		{
			m->read(reg,atoi(IR+2));
		}
			
		this->master();
	}

	//m->check();	
}

void CPU::master() {


	if(SI==1)
	{
		m->read(R,atoi(IR+2));
		cout<<R<<endl;
	}
	
	else if(SI==2)
	{
		m->readline(atoi(IR+2));
	}

	else if(SI==3)
	{
		cout<<"Program terminated\n";	
		exit(1);
	}
}
