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
	string toString() {char a[1]; a[0] = tag; string str(a); return str;}
} ;

class Num : public Token
{
public:
	int value;
	Num(int v):Token(Tag::NUM){value = v;}
	string toString(){string str(to_string(value)); return str;}
} ;

//this class Word manages lexemes for reserved words, identifiers, and composite tokens.
class Word : public Token
{
public:
	string lexeme;

	Word(string s, int tag):Token(tag){lexeme = s;}
	string toString(){return lexeme;}
} ;

//some reserved Words.
static Word 
	w_and = Word("&&", Tag::AND),
	w_or = Word("||", Tag::OR),
	w_eq = Word("==", Tag::EQ),
	w_ne = Word("!=", Tag::NE),
	w_le = Word("<=", Tag::LE),
	w_ge = Word(">=", Tag::GE),
	w_minus = Word("minus", Tag::MINUS),
	w_true = Word("true", Tag::TRUE),
	w_false = Word("false", Tag::FALSE),
	w_temp = Word("t", Tag::TEMP);



//Class Real is for floating point numebrs
class Real : public Token
{
public:
	float value;
	Real(float v):Token(Tag::REAL){value = v;}
	string toString() {return to_string(value);}
} ;


class Lexer
{
public:
	int line;
	char peek;
	map<string, Word> words;
	void reserve(Word w){words.insert(make_pair(w.lexeme, w));}
	Lexer();
	//readch should read input character into variable peed.
	void readch();
	bool readch(char c);// the function will use up a char in input buffer and see whether it is the same with parameter c.
	Token scan();
} ;
#endif
