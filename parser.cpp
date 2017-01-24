#include "parser.h"

void Parser::match(int t)
{
	if(look->tag == t)
		move();
	else
		error("syntax error");
}

void Parser::program()//program --> block
{
	Stmt* s = block();
	int begin = s->newlabel();
	int after = s->newlabel();
	s->emitlabel(begin);
	s->gen(begin, after);
	s->emitlabel(after);
}


Stmt* Parser::block() // block --> { decls stmts }
{
	match('{');
	Env* savedEnv = top;
	top = new Env(top);
	decls();
	Stmt* s = stmts();
	match('}');
	top = savedEnv;
	return s;
}


void Parser::decls()
{
	while(look->tag == Tag::BASIC) // D --> type ID
	{
		Type* p = type();
		Token* tok = look;
		match(Tag::ID);
		match(';');
		Id* id = new Id((Word*) tok, p, used);
		top->put(tok, id);
		used += p->width;
	}
}


Type* Parser::type()
{
	Type* p = (Type*)look; // expect look->tag == Tag::BASIC
	match(Tag::BASIC);
	if(look->tag != '[')
		return p; // T -> basic
	else
		return dims(p);
}


Type* Parser::dims(Type* p)
{
	match('[');
	Token* tok = look;
	match(Tag::NUM);
	match(']');
	if(look->tag == '[')
	p = dims(p);
	return new Array(((Num*)tok)->value, p);
}

Stmt* Parser::stmts()
{
	if(look->tag == '}')
		return Stmt::Null;
	else return new Seq(stmt(), stmts());
}


Stmt* Parser::stmt()
{
	Expr* x;
	Stmt* s;
	Stmt* s1;
	Stmt* s2;
	Stmt* savedStmt; // save enclosing loop for breaks

	switch(look->tag)
	{
	case ';':
	{
		move();
		return Stmt::Null;
	}
	case Tag::IF:
	{
		match(Tag::IF);
		match('(');
		x = method_bool();
		match(')');
		s1 = stmt();
		if(look->tag != Tag::ELSE)
			return new If(x, s1);
		match(Tag::ELSE);
		s2 = stmt();
		return new Else(x, s1, s2);
	}
	case Tag::WHILE:
	{
		While* whilenode = new While();
		savedStmt = Stmt::Enclosing;
		Stmt::Enclosing = whilenode;
		match(Tag::WHILE);
		match('(');
		x = method_bool();
		match(')');
		s1 = stmt();
		whilenode->init(x, s1);
		Stmt::Enclosing = savedStmt; // reset Stmt->Exclosing
		return whilenode;
	}
	case Tag::DO:
	{
		Do* donode = new Do();
		savedStmt = Stmt::Enclosing;
		Stmt::Enclosing = donode;
		match(Tag::DO);
		s1 = stmt();
		match(Tag::WHILE);
		match('(');
		x = method_bool();
		match(')');
		match(';');
		donode->init(s1, x);
		Stmt::Enclosing = savedStmt;
		return donode;
	}
	case Tag::BREAK:
	{
		match(Tag::BREAK);
		match(';');
		return new Break();
	}
	case '{':
	{
		return block();
	}
	default:
		return assign();
	}
}

Stmt* Parser::assign()
{
	Stmt* stmt;
	Token* t = look;
	match(Tag::ID);
	Id* id = top->get(t);
	if(id == NULL)
	{
		std::string res;
		res += t->toString();
		res += " undeclared";
		error(res);
	}
	if(look->tag == '=') // S --> id = E
	{
		move();
		stmt = new Set(id, method_bool());
	}
	else // S --> L = E
	{
		Access* x = offset(id);
		match('=');
		stmt = new SetElem(x, method_bool());
	}
	match(';');
	return stmt;
}


Expr* Parser::method_bool()
{
	Expr* x = join();
	while(look->tag == Tag::OR)
	{
		Token* tok = look;
		move();
		x = new Or(tok, x, join());
		Type* checked_type = ((Or*)x)->check(((Or*)x)->expr1->type, ((Or*)x)->expr2->type);
		if(checked_type == NULL)
			error("type error");
	}
	return x;
}

Expr* Parser::join()//for And
{
	Expr* x = equality();
	while(look->tag == Tag::AND)
	{
		Token* tok = look;
		move();
		x = new And(tok, x, equality());
		Type* checked_type = ((And*)x)->check(((And*)x)->expr1->type, ((And*)x)->expr2->type);
		if(checked_type == NULL)
			error("type error");
	}
	return x;
}

Expr* Parser::equality()
{
	Expr* x = rel();
	while(look->tag == Tag::EQ || look->tag == Tag::NE)
	{
		Token* tok = look;
		move();
		x = new Rel(tok, x, rel());
		Type* checked_type = ((Rel*)x)->check(((Rel*)x)->expr1->type, ((Rel*)x)->expr2->type);
		if(checked_type == NULL)
			error("type error");

	}
	return x;
}

Expr* Parser::rel()
{
	Expr* x = expr();
	switch(look->tag)
	{
	case '<': case Tag::LE: case Tag::GE: case '>':
	{
		Token* tok = look;
		move();
		Rel* newrel = new Rel(tok, x, expr());
		
		Type* checked_type = newrel->check(newrel->expr1->type, newrel->expr2->type);
		if(checked_type == NULL)
			error("type error");
	}
	default:
		return x;
	}
}


Expr* Parser::expr()
{
	Expr* x = term();
	while(look->tag == '+' || look->tag == '-')
	{
		Token* tok = look;
		move();
		x = new Arith(tok, x, term());
	}
	return x;
}


Expr* Parser::term()
{
	Expr* x = unary();
	while(look->tag == '*' || look->tag == '/')
	{
		Token* tok = look;
		move();
		x = new Arith(tok, x, unary());
	}
	return x;
}

Expr* Parser::unary()
{
	if(look->tag == '-')
	{
		move();
		return new Unary(Word::w_minus, unary());
	}
	else if(look->tag == '!')
	{
		Token* tok;
		move();
		return new Not(tok, unary());
	}
	else 
		return factor();
}

Expr* Parser::factor()
{
	Expr* x = NULL;
	switch(look->tag)
	{
		case '(':
		{
			move();
			x = method_bool();
			match(')');
			return x;
		}
		case Tag::NUM:
		{
			x = new Constant(look, Type::t_int);
			move();
			return x;
		}
		case Tag::REAL:
		{
			x = new Constant(look, Type::t_float);
			move();
			return x;
		}
		case Tag::TRUE:
		{
			x = Constant::c_true;
			move();
			return x;
		}
		case Tag::FALSE:
		{
			x = Constant::c_false;
			move();
			return x;
		}
		case Tag::ID:
		{
			string s = look->toString();
			Id* id = top->get(look);
			if(id == NULL)
			{
				string res;
				res += look->toString();
				res += " undeclared";
			}
			move();
			if(look->tag != '[')
				return id;
			else
				return offset(id);
		}
		default:
		{
			error("syntax error");
			return x;
		}

	}
}

Access* Parser::offset(Id* a)
{
	Expr* i;
	Expr* w;
	Expr* t1;
	Expr* t2;
	Expr* loc;
	Type* type = a->type;
	match('[');
	i = method_bool();
	match(']');
	type = ((Array*) type)->of;
	w = new Constant(type->width);
	t1 = new Arith(new Token('*'), i, w);
	loc = t1;
	while(look->tag == '[')// multi-dimen
	{
		match('[');
		i = method_bool();
		match(']');
		type = ((Array*)type)->of;
		w = new Constant(type->width);
		t1 = new Arith(new Token('*'), i, w);
		t2 = new Arith(new Token('+'), loc, t1);
		loc = t2;
	}
	return new Access(a, loc, type);
}
