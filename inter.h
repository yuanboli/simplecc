#ifndef INTER_H
#define INTER_H

#include "lexer.h"

class Node
{
public:
	int lexline;
	Node(){lexline = Lexer::line;}
} ;

#endif
