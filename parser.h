#ifndef PARSER_H
#define PARSER_H

#include "env.h"
#include "lexer.h"
#include <string>

class Parser
{
private:
	Lexer* lex; //lexical analyzer for this parser
	Token* look; //lookahead
	Env* top;
	int used;
public:
	Parser(Lexer* l){lex = l; top = NULL; used = 0; move();}
	void move(){look = lex->scan();}
	void error(std::string s){try{throw 0;}catch(int e){std::cout << "near line " << lex->line << ": " << s << std::endl;}}
	void match(int t);
	void program();
} ;



#endif
