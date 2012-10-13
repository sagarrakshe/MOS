#include "lineprinter.h"
#include "cardreader.h"
#include "cpu.h"
#include "memory.h"
#include "alu.h"
#include "terminate.cpp"
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
	IR = new char();
	R = new char();
	IC=0;
	output=fopen("outputFile","w+");	//output of all jobs is stored in it
	SI = 0;
	PI = 0;
	TI = 0;
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
					m->loadInMemory(buffer,0);
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
	
	char *temp = new char();
	int tempPage,j,branch=-1, page;

	IC=0;
	terminate = 0;
	for (int i=0; i<m->instrLen();++i) {
		
		if(branch==-1)
			j=0;

		else{
			cout<<"Branched!\n";
			i=branch/10;
			j=(branch%10);
		}

		temp = m->readByte(i, PCB.PTR);
		tempPage = atoi(temp+2);

		for(; j<10; ++j) {
			branch = -1;

			IR = m->readByte(j, tempPage);
			cout<<"IC:"<<IC<<"\t";
			cout<<"Executing: "<<IR<<endl;
			PCB.TTC++;
			
			if(IR[0]=='G' && IR[1]=='D')
				SI=1;

			else if(IR[0]=='P' && IR[1]=='D'){
				PCB.TLC++;
				SI=2;
				page = alu->addressMap(atoi(IR+2), PCB.PTR);
				if(page==-1){
					cout<<"Invalid Page!\n";
					PI=3;
				}
				else
					realAddress = page;
				
			}

			else if(IR[0]=='H')
				SI=3;

			else if(IR[0]=='L' && IR[1]=='R')
			{
				page = alu->addressMap(atoi(IR+2),PCB.PTR);
				if(page==-1){
					//cout<<"Invalid Page!\n";
					PI=3;
				}
				else
					R = m->readByte(atoi(IR+3), page);
					cout<<"LR: "<<R<<endl;
			}

			
			else if(IR[0]=='S' && IR[1]=='R'){
				page = alu->addressMap(atoi(IR+2),PCB.PTR);
				
				if(page==-1){
					cout<<"Allocating!\n";
					m->writeByte(atoi(IR+2), R, page, 1);
				}
				
				else{
					cout<<"Available!\n";
					m->writeByte(atoi(IR+2), R, page, 0);
				}

			}

			else if(IR[0]=='C' && IR[1]=='R'){
				char *byte = new char();

				page = alu->addressMap(atoi(IR+2), PCB.PTR);
				if(page ==-1)
					PI=3;

				else{
					byte = m->readByte(atoi(IR+3), page);
					if(!strcmp(byte,R))
						C=1;
				}
			}

			else if(IR[0]=='B' && IR[1]=='T'){
				if(C){
					branch = atoi(IR+2);
					//cout<<"branch: "<<branch<<endl;
					j=10;
				}
				else
					continue;
			}
			
			if(PCB.TTC == PCB.TTL)
				TI=2;
			
			cout<<"TTC: "<<PCB.TTC<<"TTL: "<<PCB.TTL<<endl;
			cout<<"TLC: "<<PCB.TLC<<"TLL: "<<PCB.TLL<<endl;
			//cout<<"SI: "<<SI<<"\tPI: "<<PI<<"\tTI: "<<TI<<endl;
			if(SI || PI || TI){
				this->mos();
				SI=0;
				PI=0;
				TI=0;
			}

			//End of job
			if(IR[0]=='H' || SI==3){
				m->memmap();
				terminate =1;
				break;
			}
			IC++;
		}
		if(terminate)
			break;
		
	}
}

void CPU::mos() {

	switch(SI) {
		
		case 1: if(!TI){
					cr->buffToMem(atoi(IR+2));
					//cout<<"Write to memory"<<endl;
				}
				else{
					cout<<error_message(3);
					terminate = 1;
				}
				break;
				
		case 2: if(!TI){

					lp->printLine(realAddress,output);
					//cout<<"Print to File"<<endl;
				}
				else{
					//cout<<"Wrote"<<endl;
					cout<<error_message(3);
					terminate = 1;
				}
				break;
			
		case 3:
				//cout << error_message(0);
				fputc('\n',output);
	}
	
	switch(PI) {

		case 1:	if(!TI){
					//cout<<error_message(4);
				}
				else{
					//cout<<error_message(3);
					//cout<<error_message(4);
				}
				break;
		
		case 2: if(!TI){
					//cout<<error_message(5);
				}
				else{
					//cout<<error_message(3);
					//cout<<error_message(5);
				}
				break;

		case 3:	if(!TI){
					cout<<"Yet to be handled!!\n";
				}
				else{
					//cout<<error_message(3);
				}
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