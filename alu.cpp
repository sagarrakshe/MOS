#include "alu.h"

using namespace std;

ALU::ALU() {
}

/*Generate a random number*/
int ALU::genRand() {
	
	srand ( time(NULL) );
	return rand()%10;
}

/*Map Virtual address into Real address*/
int ALU::addressMap() {
	return 0;
}
