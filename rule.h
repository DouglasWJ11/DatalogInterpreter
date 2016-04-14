#pragma once
#include"predicate.h"
#include<sstream>
class rule
{
private:
	list<predicate> predList;
	predicate headPredicate;
	int ruleNumber;
public:
	rule(predicate head);
	~rule();
	string toString();
	void addPredicate(predicate newPred);
	list<predicate> getPreds();
	predicate getHead();
	void assignNumber(int i);
	int getNumber();
};

