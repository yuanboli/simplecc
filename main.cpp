#include "lexer.h"
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		cout << "Usage: simplecc FILENAME\n";
		return 0;
	}
	//for test
	Lexer lexer;
	Token* t;
	t = lexer.scan();
	cout << t->toString();

	return 0;
}
