#include "lexer.h"
#include "parser.h"
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
/*	if(argc == 1)
	{
		cout << "Usage: simplecc FILENAME\n";
		return 0;
	}
*/	//for test
	Lexer* lexer = new Lexer();
	Parser* parse = new Parser(lexer);
	parse->program();
	cout << endl;

	return 0;
}
