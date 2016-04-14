#pragma once
#include<string>
#include<list>
#include<sstream>
using namespace std;
class fact
{
public:
	fact(string factID);
	~fact();
	void addFacts(string fact);
	string toString();
	list<string> getFacts();
	string getID();
private:
	string id;
	list<string> myFacts;
};

