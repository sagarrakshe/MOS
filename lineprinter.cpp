#include <iostream>
#include "lineprinter.h"
#include "memory.h"
#include "cpu.h"

using namespace std;

extern Memory *m;
extern CPU *c;

using namespace std;

LinePrinter::LinePrinter() {
	output.open("outputFile");
}

void LinePrinter::printJobId(int job_id){
	output<<"Job Id: "<<job_id<<endl;
}

/*Write output to the outputFile*/
void LinePrinter::printLine(int line) {
	m->readline(line,content);
	output<<content<<"\n";
}

void LinePrinter::printError(char *err_msg){
	output<<"[ "<<err_msg<<"\t";
}

void LinePrinter::printInterrupt(int si, int ti, int pi){
	output<<"SI: "<<si<<"\tTI: "<<ti<<"\tPI: "<<pi<<"\t";
}

void LinePrinter::printCounter(int ttc, int tlc){
	output<<"TTC: "<<ttc<<"\tTLC: "<<tlc<<"\t";
}

void LinePrinter::printIns(int ic, char *ir){
	output<<"IC: "<<ic<<"\tIR: "<<ir<<" ]"<<"\n\n"<<endl;
}

void LinePrinter::close(){
	output.close();
}