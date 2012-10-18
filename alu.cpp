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
	char *temp = new char();

	temp = m->readByte(vir/10, ptrPage);
	if(temp[0]=='1')
		return atoi(temp+2);
	
	else 
		return -1;
}
