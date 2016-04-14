#include "rule.h"


rule::rule(predicate head)
	:headPredicate(head)
{

}


rule::~rule()
{
}


string rule::toString()
{
	stringstream ss;
	ss << headPredicate.toString() << " :- ";
	int i = 0;
	for (predicate p : predList){
		ss << p.toString();
		if (i != predList.size()-1){
			ss << ",";
		}
		i++;
	}
	return ss.str();
}


void rule::addPredicate(predicate newPred)
{
	predList.push_back(newPred);
}

list<predicate> rule::getPreds()
{
	return predList;
}

predicate rule::getHead()
{
	return headPredicate;
}

void rule::assignNumber(int i)
{
	ruleNumber = i;
}

int rule::getNumber()
{
	return ruleNumber;
}
