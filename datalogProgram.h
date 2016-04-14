#pragma once
#include"token.h"
#include"predicate.h"
#include"rule.h"
#include"fact.h"
#include"expression.h"
#include<list>
#include<set>
#include<iostream>
class datalogProgram
{
private:
	list<token> tokens;
	list<predicate> mySchemes;
	list<string> currentList; 
	list<fact> myFacts;
	list<rule> myRules;
	list<Parameter> myParameters;
	list<predicate> myPredicates;
	list<predicate> myQueries;
	set<string> domains;
public:
	datalogProgram(list<token> myTokens);
	datalogProgram();
	~datalogProgram();
	string runProgram();
	void checkQuery();
	void schemes();
	void facts();
	void rules();
	void queries();
	void schemeList();
	void ruleList();
	void factList();
	void checkRule();
	bool checkScheme();
	void failure(token t);
	void idList();
	string output();
	bool checkFacts();
	void stringList();
	predicate checkPredicate();
	Parameter checkParameter();
	expression checkExpression();
	void parameterList();
	void predicateList();
	void queryList();
	list<predicate> getSchemes();
	list<fact> getFacts();
	list<rule> getRules();
	list<predicate> getQueries();
};

