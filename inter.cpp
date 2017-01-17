#include "inter.h"

void Expr::emitjump(string test, int t, int f)//t is the label that test is true should go, f is the false label
{
	if(t != 0 && f != 0)//t and f do not fall
	{
		string res1;
		res1 += "if ";
		res1 += test;
		res1 += " goto L";
		res1 += (char)(t + '0');
		emit(res1);
		string res2;
		res2 += "goto L";
		res2 += (char)(f + '0');
		emit(res2);
	}//emit(if test goto Lt\n goto Lf)
	else if(t != 0)//false fall
	{
		string res;
		res += "if ";
		res += test;
		res += " goto L";
		res += (char)(t+'0');
	}
	else if(f != 0)//true fall
	{
		string res;
		res += "iffalse ";
		res += test;
		res += " goto L";
		res += (char)(f + '0');
	}
	else ;
	return;

}



Expr* Op::reduce()
{
	Expr *x = gen();
	Temp *t = new Temp(type);
	string res;
	res += t->toString();
	res += " = ";
	res += x->toString();
	emit(res);
	return t;
}

  Arith::Arith(Token *tok, Expr *x1, Expr *x2): Op(tok, NULL)
{
	expr1 = x1;
	expr2 = x2;
	type = Type::max(expr1->type, expr2->type);
	if(type == NULL)
		error("type error");
}

string Arith::toString()
{
	string res;
	res += expr1->toString();
	res += " ";
	res += op->toString();
	return res;
}


string Temp::toString()
{
	string res;
	res += "t";
	res += to_string(number);
	return res;
}



Unary::Unary(Token *tok, Expr *x):Op(tok, NULL) // deal with ! and -
{
	expr = x;
	type = Type::max(Type::t_int, expr->type);
	if(type == NULL)
		error("type error");
}



string Unary::toString()
{
	string res;
	res += op->toString();
	res += " ";
	res += expr->toString();
	return res;
}
