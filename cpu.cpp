#include "lineprinter.h"
#include "cardreader.h"
#include "cpu.h"
#include "memory.h"
#include "alu.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern Memory *m;
extern LinePrinter *lp;
extern CardReader *cr;
extern ALU *alu;
extern char Buffer[10][40];
extern int BuffPtr;

extern void BuffInitialize();
extern void loadInBuffer(char *);
extern void BuffMap();
//extern CPU *c;

#define AMJ(x) strstr(x,"$AMJ")
#define H(x) strstr(x,"H")
#define DTA(x) strstr(x,"DTA")
#define END(x) strstr(x,"END")
#define $(x) strstr(x,"$")

using namespace std;

CPU::CPU() {
	m = new Memory();
	lp = new LinePrinter();
	cr = new CardReader();
	IC=0;
	output=fopen("outputFile","w+");
}

int CPU::load(){
	
	int jobNum;
	while(1) 
	{	
		fgets(buffer,42,fp);
		if(!feof(fp))
		{
			cout<<"JOB:-"<<++jobNum<<endl;
			if(AMJ(buffer) && strlen(buffer)==17)
			{
				strcpy(buffer,buffer+4);
				this->initialize_pcb(buffer);

				IC=0;
				m->initialize();
				BuffInitialize();
				
				fgets(buffer,42,fp);
				//cout<<"Program Card\n";
				while(!DTA(buffer)){
					m->loadInMemory(buffer);
					fgets(buffer,42,fp);					
				}
				
				if($(buffer)){
					fgets(buffer,42,fp);
					//cout<<"Data Card\n";				
					while(!END(buffer)){
					//cout<<buffer;
					loadInBuffer(buffer);
					fgets(buffer,42,fp);					
					}
				
					if($(buffer) && strlen(buffer)==9) {
						cout<<endl;
						//BuffMap();
						BuffPtr=0;
						this->execute();
						//m->memmap();
					}
					else{
						cout<<"Syntax Error"<<endl;
						continue;
					}
				}
				else{
					cout<<"Syntax Error"<<endl;
					do{
						fgets(buffer,42,fp);
					}while(!END(buffer));
				}
			}
			else		
			{
				cout<<"Syntax Error"<<endl;
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
	
	//char operand[2];

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
			m->readByte(R,atoi(IR+2));
		
		else if(IR[0]=='S' && IR[1]=='R')
			m->writeByte(R,atoi(IR+2));
		
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

		else {
			PI=1;
			this->mos();
		}
		
		IC++;
	}
}

void CPU::mos() {

	switch(SI) {
		case 1: cr->buffToMem(atoi(IR+2));
			break;
			
		case 2: lp->printLine(atoi(IR+2),output);
			break;
			
		case 3:
			fputc('\n',output);
			fputc('\n',output);
			break;	
	}

	switch(PI) {
		case 1:	cout<<"Operation Error!\n";
			//msg and exit and same for all
			exit(2);
		
		case 2: cout<<"Operand Error!n";
			break;

		case 3:	cout<<"Page Fault!\n";
	}
}

void CPU::initialize_pcb(char *buffer) {
	char temp[5];

	strncpy(temp,buffer,4);
	PCB.jobId=atoi(temp);
	//cout<<PCB.jobId<<endl;
	strcpy(buffer,buffer+4);

	strncpy(temp,buffer,4);
	PCB.TTL=atoi(temp);
	//cout<<PCB.TTL<<endl;
	strcpy(buffer,buffer+4);

	strncpy(temp,buffer,4);
	PCB.TLL=atoi(temp);
	//cout<<PCB.TLL<<endl;
	strcpy(buffer,buffer+4);

	cout<<"IN PCB initialization"<<alu->genRand();
	
}