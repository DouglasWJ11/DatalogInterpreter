#pragma once
#include<string>
#include<sstream>
#include<list>
#include<vector>
#include"Parameter.h"
using namespace std;
class predicate
{
public:
	predicate(string myID);
	~predicate();
	void addID(Parameter newID);
	string toString();
	string getID() const;
	list<Parameter> getParameters() const;
	vector<string> getParamsasStrings();
	vector<string> getIDs() const;
	vector<list<int>> duplicates() const;
	vector<int> idLocations() const;
private:
	string id;
	list<Parameter> ruleIDs;
};

