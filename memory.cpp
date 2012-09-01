#include <iostream>
#include "memory.h"
#include <string.h>
	
using namespace std;

Memory::Memory() {
	/*Memory pointer set to 0*/
	memPtr=0;
	this->initialize();
}

void Memory::loadInMemory(char buffer[]) {

	for(int i=0;i<strlen(buffer)-1;i++)
		memory[memPtr][i]=buffer[i];
	memPtr++;
}

void Memory::read(char *IR, int IC) {

	int i,j=0;

	for(i=(IC%10)*4;i<((IC%10)*4+4);i++)
	IR[j++]=memory[IC/10][i];

}

void Memory::initialize() {
	
	memPtr=0;
	/*Memory initialized to 0*/
	for(int i=0;i<10;i++)
	for(int j=0;j<40;j++)
		memory[i][j]='*';
}

void Memory::memmap() {
	for(int i=0;i<10;i++) {
		cout<<endl<<"\t";
		for(int j=0;j<40;j++)
			cout<<memory[i][j]<<"  ";
	}
	cout<<endl;
}

void Memory::readline(int row) {
	
	for(int i=0;i<40;i++)
	{
		//bf[i]=memory[line/10][i];
		cout<<memory[row/10][i];
	}

}	

void Memory::store(char *R,int row) {
	
	int j=0;
	
	for(int i=(row%10)*4;i<(row%10)*4+4;i++){
		memory[row/10][i]=R[j++];
	}
	

}
