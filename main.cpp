#include <iostream>
#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "cpu.h"
#include "lineprinter.h"
#include "cardreader.h"

void BuffInitialize();
void loadInBuffer(char *);
void BuffMap();

Memory *m;
LinePrinter *lp;
CardReader *cr;
CPU *c;

char Buffer[10][40];
int BuffPtr;

using namespace std;

int main(int argc, char *argv[])
{
	c = new CPU();
	c->start(argv[1]);

	return 0;
}

void BuffInitialize() {
	cout<<"Entered\n";
	BuffPtr=0;
	for(int i=0;i<10;i++)
		for(int j=0;j<40;j++)
          Buffer[i][j]='-';
}

void loadInBuffer(char buffer[]) {

	for(int i=0;i<strlen(buffer)-1;i++)
		Buffer[BuffPtr][i]=buffer[i];
	BuffPtr++;
	
}

void BuffMap() {
	for(int i=0;i<10;i++) {
		cout<<endl;
		for(int j=0;j<40;j++)
			cout<<Buffer[i][j]<<" ";
	}
	cout<<endl;

}
