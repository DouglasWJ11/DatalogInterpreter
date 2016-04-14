#pragma once
#include<string>
#include<set>
#include<vector>
#include<iostream>
#include"datalogProgram.h"
#include"relation.h"
#include"fact.h"
#include"tuple.h"
#include<map>
#include"Graph.h"

struct compareRelation{
	bool operator()(relation a, relation b){
		return a.getName() < b.getName();
	}
};
class relationalDatabase
{
public:
	relationalDatabase(datalogProgram& myProgram);
	~relationalDatabase();
	void query();
	int getSize();
	void doRules(list<rule>& rules);
	void evalRules(list<rule>& rules);
	void createDatabase(list<predicate> schemes, list<fact> myFacts);
	string toString();
	string runRD();
	void combine(list<relation>& intermediates, predicate p);
private:
	Graph g;
	map<string, relation> myRs;
	set<relation> myRelations, newFacts;
	vector<relation> select;
	vector<relation> project;
	vector<relation> rename;
	map<string,relation> queryResults;
	set<vector<string>> myMatches;
	datalogProgram program;
	int numPasses;
	vector<rule> myRules;
	string output;
	string factOut();
	string ruleOut();
	string cycleOut();
	string sendOut(rule r);
	relation checkDups(relation& newTuples, relation& oldTuples);
	string optimizeOut();
};

