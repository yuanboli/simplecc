all: simplecc

simplecc: main.cpp
	g++ -o simplecc main.cpp

clean:
	rm -f *.o simplecc
