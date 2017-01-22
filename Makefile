all: simplecc

simplecc: main.o lexer.o
	g++ -std=c++11 -o simplecc main.o lexer.o -g

lexer.o: lexer.h lexer.cpp
	g++ -std=c++11 -c lexer.cpp -g

main.o: main.cpp
	g++ -std=c++11 -c main.cpp -g

clean:
	rm -f *.o simplecc *.gch

parse:
	g++ -std=c++11 parser.cpp -c

lexer:
	g++ -std=c++11 lexer.cpp -c

inter:
	g++ -std=c++11 inter.cpp -c

env:
	g++ -std=c++11 env.cpp -c
