all: simplecc

simplecc: main.cpp
	g++ -std=c++11 -o simplecc main.cpp

clean:
	rm -f *.o simplecc *.gch

lexer:
	g++ -std=c++11 lexer.h
