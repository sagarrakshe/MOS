#include <iostream>
#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "cpu.h"
#include "lineprinter.h"
#include "cardreader.h"
#include "alu.h"

using namespace std;

/*Created all objects global*/
Memory *m;
LinePrinter *lp;
CardReader *cr;
CPU *c;
ALU *alu;

char Buffer[30][40];
int BuffPtr;

void BuffInitialize() {
	BuffPtr=0;
	for(int i=0;i<30;i++)
		for(int j=0;j<40;j++)
          Buffer[i][j]=' ';
}

void loadInBuffer(char buffer[]) {

	for(int i=0;(unsigned)i<strlen(buffer)-1;i++)
		Buffer[BuffPtr][i]=buffer[i];
	BuffPtr++;
}
/*Print Buffer Map*/
void BuffMap() {
	for(int i=0;i<30;i++) {
		cout<<endl;
		for(int j=0;j<40;j++)
			cout<<' '<<Buffer[i][j]<<' ';
	}
	cout<<endl;
}

void readBuffer(char *temp) {
	int j=0;

	for(int i=0;i<40;i++)
	temp[j++]=Buffer[BuffPtr%10][i];
	temp[j]='\n';
	//cout<<temp<<endl;
	BuffPtr++;	
}

//@main()
int main(int argc, char *argv[])
{
        c = new CPU();
        c->start(argv[1]);

        return 0;
}
