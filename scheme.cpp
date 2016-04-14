#include "Scheme.h"

Scheme::Scheme() {

}

Scheme::~Scheme() {
	
}

void Scheme::setAttributes(vector<string> att)
{
	myAttributes = att;
}

void Scheme::setAtt(int position, string s)
{
	myAttributes[position] = s;
}

vector<string> Scheme::getAttributes()
{
	return myAttributes;
}

string Scheme::getValue(int i)
{
	return myAttributes[i];
}

bool Scheme::contains(string comp)
{
	if (myAttributes.size() > 0) {
		for (string s : myAttributes) {
			if (s == comp) {
				return true;
			}
		}
	}
	return false;
}

void Scheme::addAtt(string s)
{
	myAttributes.push_back(s);
}

