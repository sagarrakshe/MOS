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

/*Definations for Error-Checking*/
#define AMJ(x) strstr(x,"$AMJ")
#define H(x) strstr(x,"H")
#define DTA(x) strstr(x,"DTA")
#define END(x) strstr(x,"END")
#define $(x) strstr(x,"$")

/*All global objects imported*/
extern Memory *m;
extern LinePrinter *lp;
extern CardReader *cr;
extern ALU *alu;

/*Buffer and Buffer-Pointer imported*/
extern char Buffer[10][40];
extern int BuffPtr;

/*Buffer Functions imported*/
extern void BuffInitialize();
extern void loadInBuffer(char *);
extern void BuffMap();
//extern CPU *c;

using namespace std;

CPU::CPU() {
	m = new Memory();
	lp = new LinePrinter();
	cr = new CardReader();
	IC=0;
	output=fopen("outputFile","w+");	//output of all jobs is stored in it
}

int CPU::load(){
	
	while(1) 
	{	
		fgets(buffer,42,fp);
		if(!feof(fp))
		{
			/*$AMJ-Syntax checking*/
			if(AMJ(buffer) && strlen(buffer)==17)
			{
				IC=0;
				m->initialize();
				BuffInitialize();

				/*PCB initialization*/
				strcpy(buffer,buffer+4);
				this->initialize_pcb(buffer);
				this->display_pcb();

				fgets(buffer,42,fp);
				/*Program Card in Main Memory*/
				while(!DTA(buffer)){
					m->loadInMemory(buffer);
					fgets(buffer,42,fp);					
				}
				/*$DTA-Syntax Checking*/
				if($(buffer)){
					fgets(buffer,42,fp);
					/*Data Card in Buffer*/
					while(!END(buffer)){
						loadInBuffer(buffer);
						fgets(buffer,42,fp);					
					}
					/*$END-Syntax Checking*/
					if($(buffer) && strlen(buffer)==9) {
						//BuffMap();
						//m->memmap();
						BuffPtr=0;
						this->execute();
					}
					else{
						cout<<"Syntax Error: END"<<endl;
						continue;
					}
				}
				else{
					cout<<"Syntax Error: DTA"<<endl;
					do{
						fgets(buffer,42,fp);
					}while(!END(buffer));
				}
			}
			else		
			{
				cout<<"Syntax Error: AMJ"<<endl;
				do{
					fgets(buffer,42,fp);
				}while(!END(buffer));	
			}	
			//cout<<endl;
		}
		else
			break;
	}

	m->memmap();
	return 0;
}

void CPU::start(char *fileName) {
	
	fp=fopen(fileName,"r");
	
	/*Check for correct file*/
	if(!fp) {
		perror("Error: ");
		exit(1);	
	}
	/*Load in Main Memory and Buffer*/
	this->load();
}

void CPU::execute() {
	
	char temp[40];
	int page,retVal=-1,j;

	m->readline(PCB.PTR, temp);
	for(int i=0; i<m->instrLen();i++) {
		m->readByte(i, IR, PCB.PTR);
		page = atoi(IR+2);
		if(retVal!=-1)
			j=retVal%10;
		else
			j=0;
		for(; j<10; j++) {
			cout <<"IR: "<<IR<<endl;
			m->readByte(j, IR, page);
			retVal = userMode(IR);
			if(retVal!=-1){
				i = (retVal/10)-1;
				break;
			}
		}
	}
}

int CPU::userMode(char *IR) {
	
	if(IR[0]=='G' && IR[1]=='D')
	{
		//cout<<"Exe: GD"<<endl;
		SI=1;	
		this->mos();
	}	
	
	else if(IR[0]=='P' && IR[1]=='D')
	{
		//cout<<"Exe: PD"<<endl;
		SI=2;
		realAddress = alu->addressMap(atoi(IR+2), PCB.PTR);
		this->mos();
	}
	
	else if(IR[0]=='H')
	{
		//cout<<"Exe: H"<<endl;
		SI=3;		
		this->mos();
	}

	else if(IR[0]=='L' && IR[1]=='R') {
		//cout<<"Exe: LR"<<endl;
		if(alu->addressMap(atoi(IR+2), PCB.PTR)!=-1)
		{
			m->readByte(atoi(IR+2), R, alu->addressMap(atoi(IR+2), PCB.PTR));
			//cout<<R<<endl;
		}
		else {
			cout << "Error: LR-address\n";
			exit(2);
		}
	}
	
	else if(IR[0]=='S' && IR[1]=='R') {
		if(alu->addressMap(atoi(IR+2), PCB.PTR)!=-1)
			m->writeByte(atoi(IR+2), R, alu->addressMap(atoi(IR+2), PCB.PTR),0);
		//Dynamically Allocating 
		else{
			m->writeByte(atoi(IR+2), R, alu->addressMap(atoi(IR+2), PCB.PTR),1);
			cout<<"-SR\n";
		}
	}

	else if(IR[0]=='C' && IR[1]=='R')
	{
			char reg[5];
			int flag=0;
			
			if(alu->addressMap(atoi(IR+2), PCB.PTR)!=-1)
			{
				m->readByte(atoi(IR+2), reg, alu->addressMap(atoi(IR+2), PCB.PTR));
	
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
			
			else{
				cout<<"-CR\n";
				exit(2);
			}

		//cout<<"C: "<<C<<endl;								
	}

	else if(IR[0]=='B' && IR[1]=='T') 
	{
		if(C) 
			return atoi(IR+2);
	}
/*
	else 
	{
		cout<<IR<<endl;
		PI=1;
		this->mos();
	}
*/
	return -1;
}

void CPU::mos() {

	switch(SI) {
		
		/*Load data from Buffer to Memory*/
		case 1: cr->buffToMem(atoi(IR+2));
			break;
				
		/*Print data in output-file*/
		case 2: lp->printLine(realAddress,output);
			break;
			
		case 3:
			break;	
	}

	switch(PI) {

		case 1:	cout<<"Operation Error!\n";
			exit(2);
		
		case 2: cout<<"Operand Error!n";
			break;

		case 3:	cout<<"Page Fault!\n";
	}
}

void CPU::initialize_pcb(char *buffer) {
	char temp[5];

	strncpy(temp,buffer,4);
	PCB.jobId = atoi(temp);
	strcpy(buffer,buffer+4);

	strncpy(temp,buffer,4);
	PCB.TTL = atoi(temp);
	strcpy(buffer,buffer+4);

	strncpy(temp,buffer,4);
	PCB.TLL = atoi(temp);
	strcpy(buffer,buffer+4);

	PCB.PTR = m->ptr_initialize();	
}

void CPU::display_pcb() {

	cout<<"ID:-\t"<<PCB.jobId<<endl;
	cout<<"TTL:-\t"<<PCB.TTL<<endl;
	cout<<"TLL:-\t"<<PCB.TLL<<endl;
	cout<<"PTR:-\t"<<PCB.PTR<<endl;
}