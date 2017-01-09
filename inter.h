#ifndef INTER_H
#define INTER_H

#include "lexer.h"
#include <iostream>
#include <string>
using namespace std;

class Node
{
public:
	int lexline;
	void error(string s){try{throw 1;}catch(int e){cout << "near line " << lexline << ":" << s; }}
	static int labels;
	int newlabel(){return ++labels;}
	void emitlabel(int i){cout << "L" << i << ":";}
	void emit(string s){cout << "\t" << s << endl;}
	Node(){lexline = Lexer::line; labels = 0;}
} ;


class Expr : public Node
{
	Token op;
} ;

#endif
