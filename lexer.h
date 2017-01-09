#ifndef LEXER_H
#define LEXER_H

#include <cstdlib>
#include <string>
#include <map>
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
	virtual string toString() {char a[1]; a[0] = tag; string str(a); return str;}
} ;

class Num : public Token
{
public:
	int value;
	Num(int v):Token(Tag::NUM){value = v;}
	virtual string toString(){string str(to_string(value)); return str;}
} ;

//this class Word manages lexemes for reserved words, identifiers, and composite tokens.
class Word : public Token
{
public:
	string lexeme;

	Word(string s, int tag):Token(tag){lexeme = s;}
	virtual string toString(){return lexeme;}
} ;




//Class Real is for floating point numebrs
class Real : public Token
{
public:
	float value;
	Real(float v):Token(Tag::REAL){value = v;}
	virtual string toString() {return to_string(value);}
} ;


//Class Type is defined to be a subclass of Word, it is used for basic type names like int.
class Type : public Word
{
public:
	int width; //width is used for storage allocation
	Type(string s, int tag, int w):Word(s, tag){width = w;}
} ;



class Lexer
{
public:
//some reserved Words.
	Word *w_and, *w_or, *w_eq, *w_ne, *w_le, *w_ge, *w_minus, *w_true, *w_false, *w_temp; 
	Type *t_int, *t_float, *t_char, *t_bool;
	static int line;
	char peek;
	map<string, Word*> words;
	void reserve(Word* w){words.insert(make_pair(w->lexeme, w));}
	Lexer();
	//readch should read input character into variable peed.
	void readch();
	bool readch(char c);// the function will use up a char in input buffer and see whether it is the same with parameter c.
	Token* scan();
} ;
#endif
