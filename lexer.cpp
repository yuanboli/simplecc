#include "lexer.h"
#include <iostream>
#include <cctype>

Lexer::Lexer()
{
	line = 1;
	peek = ' ';
	reserve(Word("if", Tag::IF));
	reserve(Word("else", Tag::ELSE));
	reserve(Word("while", Tag::WHILE));
	reserve(Word("do", Tag::DO));
	reserve(Word("break", Tag::BREAK));
	reserve(w_true);
	reserve(w_false);
	/* the following need symbol table */
	/* 
	reserve(Type.Int);
	reserve(Type.Char);
	reserve(Type.bool);
	reserve(Type.
	*/
}



// read input character into variable peed. It should deal with IO exception.
void Lexer::readch()
{
	cin >> peek;
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
Token Lexer::scan()
{
	for(;;readch())
	{
		if(peek == ' ' || peek == '\t')
			continue;
		else if(peek == 'n')
			line++;
		else
			break;
	}// skip whitespace

	switch(peek){
	case '&':
		if(readch('&'))
			return w_and;
		else
			return Token('&');
	case '|':
		if(readch('|'))
			return w_or;
		else
			return Token('|');
	case '=':
		if(readch('='))
			return w_eq;
		else
			return Token('=');
	case '!':
		if(readch('='))
			return w_ne;
		else
			return Token('!');
	case '<':
		if(readch('='))
			return w_le;
		else
			return Token('<');
	case '>':
		if(readch('='))
			return w_ge;
		else
			return Token('>');
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
			return Num(v);
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
		return Real(x);
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
			Word w = words.find(str)->second;
			return w;
		}
		Word w = Word(str, Tag::ID);
		words.insert(make_pair(str, w));
		return w;
		
	}


	//The remain other character (not letter, not digit, others) are returned as tokens
	Token tok = Token(peek);
	peek = ' ';
	return tok;

}
