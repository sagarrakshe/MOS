#include "lineprinter.h"
#include "cardreader.h"
#include "cpu.h"
#include "memory.h"
#include "alu.h"
#include "terminate.cpp"
#include <iostream>
#include <fstream>
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

	lp->close();

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
	int tempPage,j,branch=-1, page,n;

	lp->printJobId(PCB.jobId);
	this->initialize_var();
	n=m->instrLen();

	for (int i=0; i<n;++i) {	
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
			cout<<"\nIC:"<<IC<<"\t";
			cout<<"Executing: "<<IR<<endl;
			PCB.TTC++;

			if(IR[0]=='G' && IR[1]=='D'){
				SI=1;
				skip_ins=1;
			}

			else if(IR[0]=='P' && IR[1]=='D'){
				PCB.TLC++;
				SI=2;
				skip_ins=1;
				page = alu->addressMap(atoi(IR+2), PCB.PTR);
				if(page==-1){
					cout<<"Invalid Page!\n";
					PI=3;
				}
				else
					realAddress = page;
				
			}

			else if(IR[0]=='H'){
				SI=3;
				skip_ins=1;
			}

			else if(IR[0]=='L' && IR[1]=='R'){
				skip_ins=0;
				page = alu->addressMap(atoi(IR+2),PCB.PTR);
				if(page==-1){
					//cout<<"Invalid Page!\n";
					PI=3;
				}
				else
					R = m->readByte(atoi(IR+3), page);
					//cout<<"LR: "<<R<<endl;
			}

			
			else if(IR[0]=='S' && IR[1]=='R'){
				skip_ins=0;
				page = alu->addressMap(atoi(IR+2),PCB.PTR);
				
				if(page==-1){
					//cout<<"Allocating!\n";
					m->writeByte(atoi(IR+2), R, page, 1);
				}
				
				else{
					//cout<<"Available!\n";
					m->writeByte(atoi(IR+2), R, page, 0);
				}
				//m->memmap();
				//exit(2);
			}

			else if(IR[0]=='C' && IR[1]=='R'){

				char *byte = new char();

				skip_ins=0;
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
				skip_ins=0;
				if(C){
					branch = atoi(IR+2);
					//cout<<"branch: "<<branch<<endl;
					j=10;
				}
				else
					continue;
			}

			//Opcode-Error
			else
				PI = 1;

			//Operand-Error
			if(!(((int)IR[2]>=48 && (int)IR[2]<58) && ((int)IR[3]>=48 && (int)IR[3]<58)))
			{
				if(IR[0]!='H')
					PI = 2;
			}

			//Line-Limit-Error
			if(PCB.TLC > PCB.TLL)
				TI=2;
			
			//Time-Limit-Error
			if(PCB.TTC == PCB.TTL )
				TI=2;
			
			cout<<"TTC: "<<PCB.TTC<<"\tTTL: "<<PCB.TTL<<"\t";
			cout<<"TLC: "<<PCB.TLC<<"\tTLL: "<<PCB.TLL<<endl;
			//cout<<"SI: "<<SI<<"\tPI: "<<PI<<"\tTI: "<<TI<<endl;
			if(SI || PI || TI)
				this->mos();

			//End of job
			if(terminate)
				break;

			IC++;
		}

		if(terminate){
			lp->printInterrupt(SI, TI, PI);
			lp->printCounter(PCB.TTC, PCB.TLC);
			lp->printIns(IC, IR);
			m->memmap();
			break;
		}
	}
}

void CPU::mos() {

	switch(SI) {
		
		case 1: if(!TI){
					if(skip_ins)
					cr->buffToMem(atoi(IR+2));
					//cout<<"Write to memory"<<endl;
				}
				else{
					cout<<error_message(3)<<endl;
					lp->printError(error_message(3));
					terminate = 1;
				}
				break;
				
		case 2: if(!TI){
					if(skip_ins)
						lp->printLine(realAddress);
					
				}
				else{
					//cout<<"Wrote"<<endl;
					lp->printLine(realAddress);
					cout<<error_message(2)<<endl;
					lp->printError(error_message(2));
					terminate=1;
				}
				break;
			
		case 3:	if(skip_ins){
					cout << error_message(0)<<endl;
					lp->printError(error_message(0));
					terminate = 1;
				}
	}
	
	switch(PI) {

		case 1:	if(!TI){
					cout<<error_message(4)<<endl;
					lp->printError(error_message(4));
					terminate = 1;
				}
				else{
					cout<<error_message(3)<<endl;
					cout<<error_message(4)<<endl;
					lp->printError(error_message(3));
					lp->printError(error_message(4));
					terminate = 1;
				}
				break;
		
		case 2: if(!TI){
					cout<<error_message(5)<<endl;
					lp->printError(error_message(5));
					terminate = 1;
				}
				else{
					cout<<error_message(3)<<endl;
					cout<<error_message(5)<<endl;
					lp->printError(error_message(3));
					lp->printError(error_message(5));
					terminate = 1;
				}
				break;

		case 3:	if(!TI){
					cout<<error_message(6)<<endl;
					lp->printError(error_message(6));
					terminate = 1;
				}
				else{

					cout<<error_message(3)<<endl;
					lp->printError(error_message(3));
					terminate = 1;
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

	cout<<"ID:-\t"<<PCB.jobId<<"\t";
	cout<<"TTL:-\t"<<PCB.TTL<<"\t";
	cout<<"TLL:-\t"<<PCB.TLL<<"\t";
	cout<<"PTR:-\t"<<PCB.PTR<<endl;
}

void CPU::initialize_var() {
	IC=0;

	SI=0;
	PI=0;
	if(BuffPtr)
		TI=0;
	else
		TI=2;

	PCB.TTC=0;
	PCB.TLC=0;

	terminate =0;
	skip_ins=0;
	BuffPtr=0;
}