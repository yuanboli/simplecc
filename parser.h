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
	int used; //byte used
public:
	Parser(Lexer* l){lex = l; top = NULL; used = 0; move();}
	void move(){look = lex->scan();}
	void error(std::string s){try{throw 0;}catch(int e){std::cout << "near line " << lex->line << ": " << s << std::endl;}}
	void match(int t);
	void program();
	Stmt* block();
	void decls();
	Type* type();
	Type* dims(Type* p); // for array
	Stmt* stmts();
	Stmt* stmt();
	Stmt* assign();
	Expr* method_bool();
	Expr* join();
	Expr* equality();
	Expr* rel();
	Expr* expr();
	Expr* term();
	Expr* unary();
	Expr* factor();
	Access* offset(Id* a);
} ;

#endif
