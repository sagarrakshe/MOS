#include <iostream>
#include "memory.h"
#include <string.h>
#include <time.h>
#include <math.h>

using namespace std;

extern ALU *alu;

Memory::Memory() {
	/*Memory pointer set to 0*/
	this->initialize();
	bzero(frame,sizeof(frame));
}

void Memory::initialize() {

	memPtr=0;
	/*Memory initialized to 0*/
	for(int i=0;i<30;i++)
	for(int j=0;j<40;j++)
		memory[i][j]='~';
}

void Memory::memmap() {
	for(int i=0;i<30;i++) {
		cout<<i<<"  ";
		for(int j=0;j<40;j++) {
			if(!(j%4))	cout<<"|";
			cout<<" "<<memory[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}

/*PageTableRegister allocated and initialized*/
int Memory::ptr_initialize() {

	int page;
	page = alu->genRand();

	for(int i=0;i<40;i++)
		memory[page][i]='@';
	frame[page]=1;
	ptrPage = page;

	return page;
}

int Memory::instrLen() {
	return memPtr;
}

void Memory::loadInMemory(char buffer[]) {
	
	int randPage;
	
	do {
		randPage=alu->genRand();
	}while(frame[randPage]!=0);
	frame[randPage] = 1;

	for(int i=0;(unsigned)i<strlen(buffer)-1;i++)
		memory[randPage][i]=buffer[i];
	//cout<<randPage/10<<randPage%10<<endl;
	memory[ptrPage][((memPtr%4)*4)+2] = (char)(((int)'0')+randPage/10);
	memory[ptrPage][((memPtr%4)*4)+3] = (char)(((int)'0')+randPage%10);
	memPtr++;
}

/*Read Instruction from Memory*/
void Memory::readByte(int IC, char *IR, int page) {

	int i,j=0;
	for(i=(IC%10)*4;i<((IC%10)*4+4);i++,j++) {
		IR[j]=memory[page][i];
	}
}

void Memory::readline(int row, char *content) {	
	for(int i=0;i<40;i++)
		content[i]=memory[row][i];
}	

void Memory::writeByte(char *R,int row) {
	int j=0;
	for(int i=(row%10)*4;i<(row%10)*4+4;i++){
		memory[row/10][i]=R[j++];
	}
}
