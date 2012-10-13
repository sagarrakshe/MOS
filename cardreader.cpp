#include <iostream>
#include <stdio.h>
#include <string.h>
#include "cardreader.h"
#include "memory.h"

using namespace std;

extern Memory *m;
extern void BuffInitialize();
extern void loadInBuffer(char *);
extern void BuffMap();
extern void readBuffer(char *);

CardReader::CardReader() {
}

/*Buffer-to-Memory*/
void CardReader::buffToMem(int loc) {	
	readBuffer(str);
	//m->memPtr=loc/10;
	m->loadInMemory(str,loc);
}

