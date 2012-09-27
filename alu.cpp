#include "alu.h"

using namespace std;
extern Memory *m;

ALU::ALU() {
}

/*Generate a random number*/
int ALU::genRand() {
	
	srand ( time(NULL) );
	return rand()%30;
}

/*Map Virtual address into Real address*/
int ALU::addressMap(int vir, int ptrPage) {
	char temp[5];
	//int real;

	m->readByte(vir/10, temp, ptrPage);
	if(m->frame[atoi(temp+2)]){
		m->readByte(vir/10, temp, ptrPage);
		return atoi(temp+2);
	}
	else {
		cout<<"Invalid Page Fault!! ";
		return -1;
	}
     
}
