#include "tuple.h"



Tuple::Tuple()
{
}


Tuple::~Tuple()
{
}

void Tuple::add(std::string s) {
	attributes.push_back(s);
}

int Tuple::size() const{
	return attributes.size();
}

std::string Tuple::getValue(int position) const {
	return attributes[position];
}

bool Tuple::operator<(const Tuple &other)  const{
	return attributes < other.attributes;
}

bool Tuple::operator==(const Tuple &other) const {
	return other.attributes==attributes;
}
std::vector<std::string> Tuple::getAttributes()
{
	return attributes;
}
