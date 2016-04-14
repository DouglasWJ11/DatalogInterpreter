#pragma once
#include"Parameter.h"
#include<list>
#include<sstream>
class expression
{
public:
	expression(Parameter first,string op,Parameter second);
	~expression();
	string toString();
private:
	Parameter one, two;
	string oper;
};

