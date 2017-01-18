#ifndef ENV_H
#define ENV_H

#include <map>
#include "lexer.h"
#include "inter.h"

class Env
{
private:
	std::map<Token*, Id*> *table;
protected:
	Env* prev;
public:
	Env(Env *n){table = new std::map<Token*, Id*>; prev = n;}
	void put(Token *w, Id *i){table->insert(std::make_pair(w, i));}
	Id* get(Token* w)
	{
		for(Env *e = this; e != NULL; e = e->prev)
		{
			if(e->table->find(w) != table->end())
			{
				Id* found = (Id*)(e->table->find(w))->second;
				return found;
			}
		}
		return NULL;
	}
} ;


#endif
