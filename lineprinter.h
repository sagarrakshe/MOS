#ifndef LINEPRINTER_H_
#define LINEPRINTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

using namespace std;

class LinePrinter {

	char content[100];
	ofstream output;
	
	public:
	
	LinePrinter();
	void printJobId(int );
	void printLine(int );
	void printError(char * );
	void printInterrupt(int, int, int);
	void printCounter(int, int);
	void printIns(int , char *);
	void close();

};

#endif /*LINEPRINTER_H_*/
