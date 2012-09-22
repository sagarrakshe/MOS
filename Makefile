MOS: main.o cpu.o memory.o lineprinter.o cardreader.o alu.o
	g++ -Wall -o MOS main.o cpu.o memory.o lineprinter.o cardreader.o alu.o

alu.o: alu.cpp alu.h
	g++ -Wall -g -c alu.cpp

cardreader.o: cardreader.cpp cardreader.h
	g++ -Wall -g -c cardreader.cpp

lineprinter.o: lineprinter.cpp memory.h
	g++ -Wall -g -c lineprinter.cpp

cpu.o: cpu.cpp cpu.h
	g++ -Wall -g -c cpu.cpp

memory.o: memory.cpp memory.h
	g++ -Wall -g -c memory.cpp

main.o: main.cpp cpu.h memory.h 
	g++ -Wall -g -c main.cpp 

clean: 
	rm -rf *.o 
