#ifndef LINEPRINTER_H_
#define LINEPRINTER_H_

#include <stdio.h>
#include <stdlib.h>

class LinePrinter {

	char content[100];
	FILE *fp;
	
	public:
	
	LinePrinter();
	void printLine(int ,FILE *);

};

#endif /*LINEPRINTER_H_*/
