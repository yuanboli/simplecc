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

	/*emitlabel emit the label before instructions*/
	void emitlabel(int i){cout << "L" << i << ":";}
	void emit(string s){cout << "\t" << s << endl;}
	Node(){lexline = Lexer::line;}
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
	virtual void jumping(int t, int f) {emitjump(toString(), t, f);}
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



class Unary: public Op
{
public:
	Expr* expr;
	Unary(Token *tok, Expr *x);
	virtual Expr* gen(){return new Unary(op, expr->reduce());}
	virtual string toString();
} ;



class Constant : public Expr
{
public:
	Constant(Token* tok, Type* p):Expr(tok, p){}
	Constant(int i):Expr(new Num(1), Type::t_int){}
	static Constant *c_true;
	static Constant *c_false;
	virtual void jumping(int t, int f);// t is for true, f is for false
} ;


//class Logical provide some common functionality for classes Or, And and Not.
class Logical : public Expr
{
public:
	Expr* expr1;
	Expr* expr2;
	Logical(Token* tok, Expr* x1, Expr* x2);
	
	/* check whether p1 and p2 are bool 
		if p1 and p2 are both Type::t_bool
		return Type::t_bool
		else NULL
	*/
	virtual Type* check(Type* p1, Type* p2);

	virtual Expr* gen();
	
	virtual string toString();

} ;



//class Or
class Or :public Logical
{
public:
	Or(Token* tok, Expr* x1, Expr* x2):Logical(tok, x1, x2){}
	virtual void jumping(int t, int f);
} ;


class And : public Logical
{
public:
	And(Token* tok, Expr* x1, Expr* x2):Logical(tok, x1, x2){}
	virtual void jumping(int t, int f);
} ;


class Not: public Logical
{
public:
	Not(Token* tok, Expr* x2):Logical(tok, x2, x2){}
	virtual void jumping(int t, int f){expr2->jumping(f, t);}
	virtual string toString();
} ;

/*	class Rel implements the operators <, <=, ==, !=, >= and >*/
class Rel: public Logical
{
public:
	Rel(Token* tok, Expr* x1, Expr* x2):Logical(tok, x1, x2){}
	virtual Type* check(Type* p1, Type* p2);// checks that the two operands have the same type and that they are not arrays
	virtual void jumping(int t, int f);
} ;


/*this Access class is for index of the array*/
class Access:public Op
{
public:
	Id* array;
	Expr* index;
	Access(Id* a, Expr* i, Type* p):Op(new Word("[]", Tag::INDEX), p){array = a; index = i;}
	virtual Expr* gen(){return new Access(array, index->reduce(), type);}
	virtual void jumping(int t, int f){emitjump(reduce()->toString(), t, f);}
	virtual string toString();
} ;


class Stmt: public Node
{
public:
	int after;
	Stmt(){after = 0;}
	virtual void gen(int b, int a){}//calld with label begin and after
	

	static Stmt* Null;//represents and empty sequence of statement
	static Stmt* Enclosing; // used for break stmts
} ;


class If:public Stmt
{
public:
	Expr* expr;
	Stmt* stmt;
	If(Expr* x, Stmt* s);
	
	
	/* b represents the beginning of the code for the statement, a marks the first instruction after the code for this statement. */ 
	virtual void gen(int b, int a);
} ;



class Else: public Stmt
{
public:
	Expr* expr;
	Stmt* stmt1;
	Stmt* stmt2;
	Else(Expr* x, Stmt* s1, Stmt* s2);
	virtual void gen(int b, int a);
} ;


class While: public Stmt
{
public:
	Expr* expr;
	Stmt* stmt;
	While(){expr = NULL; stmt = NULL;}
	void init(Expr* x, Stmt* s);
	/*b for the begining of the code for the statement, a for first instruction After the code*/
	virtual void gen(int b, int a); 
} ;


class Do:public Stmt
{
public:
	Expr* expr;
	Stmt* stmt;
	Do(){expr = NULL; stmt = NULL;}
	void init(Stmt* s, Expr* x);
	virtual void gen(int b, int a);
} ;


/*class Set implements assignments with an identifier on the left side and an expression on the right*/
class Set:public Stmt
{
public:
	Id* id;
	Expr* expr;
	Set(Id* i, Expr* x);
	virtual Type* check(Type* p1, Type* p2);
	virtual void gen(int b, int a);
} ;

/*class SetElem implements assignments to an array element*/
class SetElem:public Stmt
{
public:
	Id* array;
	Expr* index;
	Expr* expr;
	SetElem(Access* x, Expr* y);
	virtual Type* check(Type* p1, Type* p2);
	virtual void gen(int b, int a);
} ;


/*class Seq implements a sequence of statements*/
class Seq:public Stmt
{
public:
	Stmt* stmt1;
	Stmt* stmt2;
	Seq(Stmt* s1, Stmt* s2){stmt1 = s1; stmt2 = s2;}
	virtual void gen(int b, int a);
} ;


class Break:public Stmt
{
public:
	Stmt* stmt;
	Break();
	virtual void gen(int b, int a);

} ;
#endif
