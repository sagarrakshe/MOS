#include <iostream>
#include "lineprinter.h"
#include "memory.h"
#include "cpu.h"

using namespace std;

extern Memory *m;

LinePrinter::LinePrinter() {
}

/*Write output to the outputFile*/
void LinePrinter::printLine(int line, FILE *fp) {
	m->readline(line,content);
	//cout<<"PD content:"<<content<<endl;
	fputs(content,fp);
	fputc('\n',fp);
}
