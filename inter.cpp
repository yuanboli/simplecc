#include "inter.h"


Constant* Constant::c_true = new Constant(Word::w_true, Type::t_bool);
Constant* Constant::c_false = new Constant(Word::w_false, Type::t_bool);
Stmt* Stmt::Null = new Stmt();
Stmt* Stmt::Enclosing = Stmt::Null;
int Temp::count = 0;
int Node::labels = 0;

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


void Constant::jumping(int t, int f)
{
	if(this == Constant::c_true && t != 0)
	{
		string res = "goto L";
		res += std::to_string(t);
		emit(res);
	}
	else if(this == Constant::c_false && f != 0)
	{
		string res = "goto L";
		res += std::to_string(f);
		emit(res);
	}
}


Logical::Logical(Token* tok, Expr* x1, Expr* x2):Expr(tok, NULL)
{
	expr1 = x1;
	expr2 = x2;
	type = check(expr1->type, expr2->type);
	if(type == NULL)
		error("type error");
}


Type* Logical::check(Type* p1, Type* p2)
{
	if(p1 == Type::t_bool && p2 == Type::t_bool)
		return Type::t_bool;
	else
		return NULL;
}



Expr* Logical::gen()
{
	int f = newlabel();
	int a = newlabel();
	Temp* temp = new Temp(type);
	this->jumping(0, f);
	
	string res1;
	res1 += temp->toString();
	res1 += " = true";
	emit(res1);

	emitlabel(f);
	
	string res2;
	res2 += temp->toString();
	res2 += " = false";
	emit(res2);

	emitlabel(a);
	
	return temp;
}


string Logical::toString()
{
	string res; 
	res += expr1->toString(); 
	res += " "; 
	res += op->toString();
	res += " ";
	res += expr2->toString();
	return res;
}



void Or::jumping(int t, int f)
{
	int label = t != 0 ? t : newlabel(); // if t != 0 , label = t; else label = newlabel
	expr1->jumping(label, 0);
	expr2->jumping(t, f);
	if(t == 0)
		emitlabel(label);
}

void And::jumping(int t, int f)
{
	int label = f != 0 ? f : newlabel(); // if f != 0; label = f; else get a new one
	expr1->jumping(0, label);
	expr2->jumping(t, f);
	if(f == 0)
		emitlabel(label);
}

string Not::toString()
{
	string res;
	res += op->toString();
	res += " ";
	res += expr2->toString();
	return res;
}


Type* Rel::check(Type* p1, Type* p2)
{
	Array *a1, *a2;
	if((a1 = dynamic_cast<Array*>(p1)) || (a2 = dynamic_cast<Array*>(p2)))
		return NULL;
	else if(p1 == p2)
		return Type::t_bool;
	else
		return NULL;
}

void Rel::jumping(int t, int f)
{
	Expr* a = expr1->reduce();
	Expr* b = expr2->reduce();
	string test = a->toString();
	test += " ";
	test += op->toString();
	test += " ";
	test += b->toString();
	emitjump(test, t, f);
}


string Access::toString()
{
	std::string res;
	res += array->toString();
	res += " [ ";
	res += index->toString();
	res += " ]";
	return res;
}


If::If(Expr* x, Stmt* s)
{
	expr = x;
	stmt = s;
	if(expr->type != Type::t_bool)
		expr->error("boolean required in if");
}


void If::gen(int b, int a)
{
	int label = newlabel(); //label for the code for stmt
	expr->jumping(0, a); //fall throught on true, goto a on false
	emitlabel(label);
	stmt->gen(label, a);
}


Else::Else(Expr* x, Stmt* s1, Stmt* s2)
{
	expr = x;
	stmt1 = s1;
	stmt2 = s2;
	if(expr->type != Type::t_bool)
		expr->error("boolean required in if");
}

void Else::gen(int b, int a)
{
	int label1 = newlabel(); //label1 for stmt1
	int label2 = newlabel(); //label2 for stmt2
	expr->jumping(0, label2);//fall throught to stmt1 on true;
	emitlabel(label1);
	stmt1->gen(label1, a);

	string res;
	res += "goto L";
	res += std::to_string(a);
	emit(res);

	emitlabel(label2);
	stmt2->gen(label2, a);
}


void While::init(Expr* x, Stmt* s)
{
	expr = x;
	stmt = s;
	if(expr->type != Type::t_bool)
		expr->error("boolean required in while");
}

void While::gen(int b, int a)
{
	after = a;//save label a
	expr->jumping(0, a);
	int label = newlabel();
	emitlabel(label);
	stmt->gen(label, b);
	
	string res;
	res += "goto L";
	res += to_string(b);
	emit(res);
}


void Do::init(Stmt* s, Expr* x)
{
	expr = x;
	stmt = s;
	if(expr->type != Type::t_bool)
		expr->error("boolean required in do"); 
}



void Do::gen(int b, int a)
{
	after = a;
	int label = newlabel();//label for expr
	stmt->gen(b, label);
	emitlabel(label);
	expr->jumping(b, 0);
}



Set::Set(Id* i, Expr* x)
{
	id = i;
	expr = x;
	if(check(id->type, expr->type) == NULL)
		error("type error");
}

Type* Set::check(Type* p1, Type* p2)
{
	if(Type::numeric(p1) && Type::numeric(p2))
		return p2;
	else if(p1 == Type::t_bool && p2 == Type::t_bool)
		return p2;
	else
		return NULL;
}

void Set::gen(int b, int a)
{
	string res;
	res += id->toString();
	res += " = ";
	res += expr->gen()->toString();
	emit(res);
}



SetElem::SetElem(Access* x, Expr* y)
{
	array = x->array;
	index = x->index;
	expr = y;
	if(check(x->type, expr->type) == NULL)
		error("type error");
}



Type* SetElem::check(Type* p1, Type* p2)
{
	Array* a1, *a2;
	if((a1 = dynamic_cast<Array*>(p1)) || (a2 = dynamic_cast<Array*>(p2)))
		return NULL;
	else if(p1 == p2)
		return p2;
	else if(Type::numeric(p1) && Type::numeric(p2))
		return p2;
	else
		return NULL;
}


void SetElem::gen(int b, int a)
{
	string s1 = index->reduce()->toString();
	string s2 = expr->reduce()->toString();
	
	string res;
	res += array->toString();
	res += " [ ";
	res += s1;
	res += " ] = ";
	res += s2;

	emit(res);
}


void Seq::gen(int b, int a)
{
	if(stmt1 == Stmt::Null)
		stmt2->gen(b, a);
	else if(stmt2 == Stmt::Null)
		stmt1->gen(b, a);
	else
	{
		int label = newlabel();
		stmt1->gen(b, label);
		emitlabel(label);
		stmt2->gen(label, a);
	}
}


Break::Break()
{
	if(Stmt::Enclosing == NULL)
		error("unenclosed break");
	stmt = Stmt::Enclosing;
}

void Break::gen(int b, int a)
{
	string res;
	res += "goto L";
	res += stmt->after;
	emit(res);
}
