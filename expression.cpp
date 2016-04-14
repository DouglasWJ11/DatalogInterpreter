#include "expression.h"


expression::expression(Parameter first, string op, Parameter second)
	:one(first), two(second)
{
//	one = first;
//	two = second;
	oper = op;
}


expression::~expression()
{
}
string expression::toString()
{
	stringstream ss;
	ss << "(" << one.toString() << oper << two.toString() << ")";
	return ss.str();
}