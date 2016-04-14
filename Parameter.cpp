#include "Parameter.h"


Parameter::Parameter(string it,bool type)
{
	value = it;
	isID = type;
}

Parameter::~Parameter()
{
}


string Parameter::toString() const
{
	return value;
}

string Parameter::getName() {
	return name;
}

void Parameter :: setName(string n) {
	name = n;
}

void Parameter::setValue(string v) {
	value = v;
}

bool Parameter::checkID() const
{
	return isID;
}