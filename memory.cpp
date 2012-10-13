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
		memory[i][j]=' ';
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

void Memory::loadInMemory(char buffer[], int flag) {
	
	int randPage;
	
	do {
		randPage=alu->genRand();
	}while(frame[randPage]!=0);
	frame[randPage] = 1;

	for(int i=0;(unsigned)i<strlen(buffer)-1;i++)
		memory[randPage][i]=buffer[i];
	//cout<<randPage/10<<randPage%10<<endl;
	if(flag){
		memory[ptrPage][((flag/10)*4)+2] = (char)(((int)'0')+randPage/10);
		memory[ptrPage][((flag/10)*4)+3] = (char)(((int)'0')+randPage%10);
	}
	else{
		memory[ptrPage][((memPtr%10)*4)+2] = (char)(((int)'0')+randPage/10);
		memory[ptrPage][((memPtr%10)*4)+3] = (char)(((int)'0')+randPage%10);
		//cout<<"memPtr: "<<memPtr<<endl;
		memPtr++;
	}
}

/*Read Instruction from Memory*/
char *Memory::readByte(int IC, int page) {

	int i,j=0;
	char *ir = new char();

	for(i=(IC%10)*4;i<((IC%10)*4+4);++i,++j)
		ir[j]=memory[page][i];
	
	return ir;
}

void Memory::writeByte(int IC, char *R, int page, int flag) {
	int randPage,j=0;

	if(flag) {
		do {
			randPage=alu->genRand();
		}while(frame[randPage]!=0);
		frame[randPage] = 1;	
		
		memory[ptrPage][((IC/10)*4)+2] = (char)(((int)'0')+randPage/10);
		memory[ptrPage][((IC/10)*4)+3] = (char)(((int)'0')+randPage%10);
		memPtr++;

		page = randPage;
	}

	for(int i=(IC%10)*4;i<(IC%10)*4+4;i++){
		memory[page][i]=R[j++];
	}
}

void Memory::readline(int row, char *content) {	
	for(int i=0;i<40;i++)
		content[i]=memory[row][i];
}	

void Memory::dispFrame() {
	for (int i = 0; i < 30; ++i)
		cout<<frame[i]<<" ";
	cout<<endl;
}