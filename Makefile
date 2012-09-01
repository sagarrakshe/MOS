MOS: main.o cpu.o memory.o lineprinter.o
	g++ -Wall -o MOS main.o cpu.o memory.o lineprinter.o

lineprinter.o: lineprinter.cpp memory.h
	g++ -Wall -c lineprinter.cpp

cpu.o: cpu.cpp cpu.h
	g++ -Wall -c cpu.cpp

memory.o: memory.cpp memory.h
	g++ -Wall -c memory.cpp

main.o: main.cpp cpu.h memory.h 
	g++ -Wall -c main.cpp 

clean: 
	rm -rf *.o 
