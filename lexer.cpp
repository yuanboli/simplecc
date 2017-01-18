#include "lexer.h"
#include <iostream>
#include <cctype>
int Lexer::line = 1;
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
Type* Type::t_int = new Type("int", Tag::BASIC, 4);
Type* Type::t_float = new Type("float", Tag::BASIC, 8);
Type* Type::t_char = new Type("char", Tag::BASIC, 1);
Type* Type::t_bool = new Type("bool", Tag::BASIC, 1);

Lexer::Lexer()
{
	//some reserved words

	//init
	//this->line = 1;
	peek = ' ';
	reserve(new Word("if", Tag::IF));
	reserve(new Word("else", Tag::ELSE));
	reserve(new Word("while", Tag::WHILE));
	reserve(new Word("do", Tag::DO));
	reserve(new Word("break", Tag::BREAK));
	reserve(Word::w_true);
	reserve(Word::w_false); 
	reserve(Type::t_int);
	reserve(Type::t_char);
	reserve(Type::t_bool);
	reserve(Type::t_float);
	
}



// read input character into variable peed. It should deal with IO exception.
void Lexer::readch()
{
	cin.get(peek);
}

// the function will use up a char in input buffer and see whether it is the same with parameter c.
bool Lexer::readch(char c)
{
	readch();
	if(peek != c)
		return false;
	peek = ' ';
	return true;
}


//method scan begins by skipping white space. It recognize composite tokens like <= and numbers before collecting words.
//also it should deal with IO exception.
Token* Lexer::scan()
{
	for(;;readch())
	{
		if(peek == ' ' || peek == '\t')
			continue;
		else if(peek == 'n')
			(this->line)++;
		else
			break;
	}// skip whitespace

	switch(peek){
	case '&':
		if(readch('&'))
			return Word::w_and;
		else
			return new Token('&');
	case '|':
		if(readch('|'))
			return Word::w_or;
		else
			return new Token('|');
	case '=':
		if(readch('='))
			return Word::w_eq;
		else
			return new Token('=');
	case '!':
		if(readch('='))
			return Word::w_ne;
		else
			return new Token('!');
	case '<':
		if(readch('='))
			return Word::w_le;
		else
			return new Token('<');
	case '>':
		if(readch('='))
			return Word::w_ge;
		else
			return new Token('>');
	}

	if(isdigit(peek))
	{
		int v = 0;
		do
		{
			v = 10*v + (peek - '0');
			readch();
		}while(isdigit(peek));
		if(peek != '.')
			return new Num(v);
		float x = v;
		float d = 10;
		while(true)
		{
			readch();
			if(!isdigit(peek))
				break;
			x += (peek - '0')/d;
			d *= 10;
		}
		return new Real(x);
	}

	if(isalpha(peek))
	{
		string str;
		do
		{
			str+= peek;
			readch();
		}while(isalpha(peek) || isdigit(peek));
		if(words.find(str) != words.end())
		{
			Word* w = words.find(str)->second;
			return w;
		}
		Word* w = new Word(str, Tag::ID);
		words.insert(make_pair(str, w));
		return w;
		
	}


	//The remain other character (not letter, not digit, others) are returned as tokens
	Token* tok = new Token(peek);
	peek = ' ';
	return tok;

}


bool Type::numeric(Type* p)
{
	if(p == t_char || p == t_int || p == t_float)
		return true;
	else 
		return false;
}

Type* Type::max(Type* p1, Type* p2)
{
	if(!numeric(p1) || !numeric(p2))
		return NULL;
	else if(p1 == t_float || p2 == t_float)
		return t_float;
	else if(p1 == t_int || p2 == t_int)
		return t_int;
	else 
		return t_char;
}



string Array::toString()
{
	string res;
	res += "[";
	res += to_string(size);
	res += "] ";
	res += of->toString();
}
