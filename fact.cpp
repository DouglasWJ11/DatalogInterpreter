#include "fact.h"


fact::fact(string factID)
{
	id = factID;
}


fact::~fact()
{
}
void fact::addFacts(string fact){
	myFacts.push_back(fact);
}
string fact::toString(){
	stringstream ss;
	ss << id << "(";
	for (string s : myFacts){
		ss << s;
		//ss << "\'" << s << "\'";
		if (s != myFacts.back()){
			ss << ",";
		}
	}
	ss << ")";
	return ss.str();
}
list<string> fact::getFacts(){
	return myFacts;
}

string fact::getID() {
	return id;
}