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
	IC=0;
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
				IC=0;
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
				//cout<<"Data Card\n";
				while(!END(buffer)){
					//cout<<buffer;
					loadInBuffer(buffer);
					fgets(buffer,42,fp);					
				}
				cout<<endl;
				//BuffMap();
				BuffPtr=0;
				this->execute();
				//m->memmap();
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
	
	while(1) {
		m->readByte(IR,IC);
			
		if(IR[0]=='G' && IR[1]=='D')
		{
			SI=1;	
			this->mos();
		}
			
		else if(IR[0]=='P' && IR[1]=='D')
		{
			SI=2;
			this->mos();
		}
		
		else if(IR[0]=='H')
		{
			SI=3;		
			this->mos();
			break;
		}
		
		else if(IR[0]=='L' && IR[1]=='R')
		{
			m->readByte(R,atoi(IR+2));
		}
		
		else if(IR[0]=='S' && IR[1]=='R')
		{		
			//m->memmap();
			m->writeByte(R,atoi(IR+2));
			//m->memmap();
		}
		
		else if(IR[0]=='C' && IR[1]=='R')
		{
			char reg[4];
			int flag=0;
			m->readByte(reg,atoi(IR+2));
			//cout<<"REG:-"<<reg<<endl;
			for(int i=0;i<4;i++)
			{
				if(R[i]!=reg[i]) {
					flag=1;
					break;
				}
			}
				if(!flag)
					C=1;
		}
		
		else if(IR[0]=='B' && IR[1]=='T')
		{
			if(C)
			IC=atoi(IR+2)-1;
		}
		
		IC++;
	}
}

void CPU::mos() {

	switch(SI) {
		case 1: cr->buffToMem(atoi(IR+2));
			break;
			
		case 2: lp->printLine(atoi(IR+2));
			break;
			
		case 3: 
			break;	
	}
}
