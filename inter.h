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

//Class Expr implements expression contructs 
class Expr : public Node
{
public:
	Token *op; //operator
	Type *type;
	Expr(Token *tok, Type *p){op = tok, type = p;}
	
	//gen() returns a "term" that can fit the right side of a three address instruction
	virtual Expr* gen(){return this;}

	//method reduce computes or "reduce" an expression down to a single address
	Expr* reduce(){return this;}
	void jumping(int t, int f) {emitjump(toString(), t, f);}
	void emitjump(string test, int t, int f);
	virtual string toString() {return op->toString();}

} ;


//class Id is a class for identifier, inherits the reduce() and gen() from Expr
class Id : public Expr
{
public:
	int offset; //provided for relative address;
	Id(Word *id, Type *p, int b):Expr(id, p){offset = b;}
} ;

//class for operator
class Op : public Expr
{
public:
	Op(Token *tok, Type *p): Expr(tok, p){}
	Expr* reduce();

} ;


class Arith : public Op
{
public:
	Expr *expr1, *expr2;
	Arith(Token *tok, Expr *x1, Expr *x2);
	virtual Expr* gen(){return new Arith(op, expr1->reduce(), expr2->reduce());}
	virtual string toString();
} ;

class Temp : public Expr
{
public:
	static int count;
	int number = 0;
	Temp(Type* p):Expr(Word::w_temp, p){number = ++count;}
	virtual string toString();
} ;

int Temp::count = 0;


class Unary: public Op
{
public:
	Expr* expr;
	Unary(Token *tok, Expr *x);
	virtual Expr* gen(){return new Unary(op, expr->reduce());}
	virtual string toString();
} ;
#endif
