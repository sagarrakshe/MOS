#include <iostream>
#include "lineprinter.h"
#include "memory.h"
#include "cpu.h"
using namespace std;

extern Memory *m;

LinePrinter::LinePrinter() {

}

void LinePrinter::printLine(int line) {
	m->readline(line);
	cout<<endl;
}
