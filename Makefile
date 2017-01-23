all: simplecc

simplecc: main.o lexer.o parser.o inter.o env.o
	g++ -std=c++11 -o simplecc main.o lexer.o parser.o inter.o env.o -g

lexer.o: lexer.h lexer.cpp
	g++ -std=c++11 -c lexer.cpp -g

clean:
	rm -f *.o simplecc *.gch

parser.o: parser.cpp parser.h
	g++ -std=c++11 parser.cpp -c

inter.o: inter.h inter.cpp
	g++ -std=c++11 inter.cpp -c

env.o: env.h env.cpp
	g++ -std=c++11 env.cpp -c

main.o: main.cpp
	g++ -std=c++11 -c main.cpp -g

