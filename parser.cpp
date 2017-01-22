#include "parser.h"

void Parser::match(int t)
{
	if(look->tag == t)
		move();
	else
		error("syntax error");
}

void Parser::program()//program --> block
{
	Stmt* s = block();
	int begin() = s->newlabel();
	int after = s->newlabel();
	s->emitlabel(begin);
	s->gen(begin, after);
	s->emitlabel(after);
}
