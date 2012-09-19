#include "alu.h"

using namespace std;

ALU::ALU() {
}

int ALU::genRand() {
	
	srand ( time(NULL) );
	return rand()%30;
}

int ALU::addressMap() {
	return 0;
}
