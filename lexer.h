#ifndef LEXER_H
#define LEXER_H

#include <cstdlib>
#include <string>
using namespace std;
class Tag
{
public:
	enum {
	AND = 256,
	BASIC,
	BREAK,
	DO,
	ELSE,
	EQ,
	FALSE,
	GE,
	ID,
	IF,
	INDEX,
	LE,
	MINUS,
	NE,
	NUM,
	OR,
	REAL,
	TEMP,
	TRUE,
	WHILE
	};
} ;
class Token
{
public:
	int tag;
	Token(int t){tag = t;}
	string toString() {char a[1]; a[0] = tag; string str(a); return str;}
} ;

class Num : public Token
{
public:
	int value;
	Num(int v):Token(Tag::NUM){value = v;}
	string toString(){string str(to_string(value)); return str;}
} ;

#endif
