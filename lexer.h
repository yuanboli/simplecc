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
	virtual string toString() {char a; a = tag; string str; str += a; return str;}
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
	static Word *w_and, *w_or, *w_eq, *w_ne, *w_le, *w_ge, *w_minus, *w_true, *w_false, *w_temp; 

	Word(string s, int tag):Token(tag){lexeme = s;}
	virtual string toString(){return lexeme;}
} ;


	Word* Word::w_and = new Word("&&", Tag::AND);
	Word* Word::w_or = new Word("||", Tag::OR);
	Word* Word::w_eq = new Word("==", Tag::EQ);
	Word* Word::w_ne = new Word("!=", Tag::NE);
	Word* Word::w_le = new Word("<=", Tag::LE);
	Word* Word::w_ge = new Word(">=", Tag::GE);
	Word* Word::w_minus = new Word("minus", Tag::MINUS);
	Word* Word::w_true = new Word("true", Tag::TRUE);
	Word* Word::w_false = new Word("false", Tag::FALSE);
	Word* Word::w_temp = new Word("t", Tag::TEMP);


//Class Real is for floating point numebrs
class Real : public Token
{
public:
	float value;
	Real(float v):Token(Tag::REAL){value = v;}
	virtual string toString() {return to_string(value);}
} ;




//types
//Class Type is defined to be a subclass of Word, it is used for basic type names like int.
class Type : public Word
{
public:
	static Type *t_int;
	static Type *t_float;
	static Type *t_char;
	static Type *t_bool;
	int width; //width is used for storage allocation
	Type(string s, int tag, int w):Word(s, tag){width = w;}
	static bool numeric(Type* p);//use for type conversion
	static Type* max(Type* p1, Type* p2);
} ;

Type* Type::t_int = new Type("int", Tag::BASIC, 4);
Type* Type::t_float = new Type("float", Tag::BASIC, 8);
Type* Type::t_char = new Type("char", Tag::BASIC, 1);
Type* Type::t_bool = new Type("bool", Tag::BASIC, 1);





class Lexer
{
public:
//some reserved Words.
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
