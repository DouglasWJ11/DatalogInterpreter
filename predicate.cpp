#include "predicate.h"


predicate::predicate(string myID)
{
	id = myID;
}


predicate::~predicate()
{
}


void predicate::addID(Parameter newID)
{
	ruleIDs.push_back(newID);
}


string predicate::toString()
{
	stringstream ss;
	ss << id << "(";
	int i = 0;
	for (Parameter s : ruleIDs){
		ss << s.toString();
		if (i != ruleIDs.size() - 1) {
			ss << ",";
		}
		i++;
	}
	ss << ")";
	return ss.str();
}


string predicate::getID() const
{
	return id;
}

list<Parameter> predicate::getParameters() const{
	return ruleIDs;
}

std::vector<string> predicate::getParamsasStrings()
{
	std::vector<string> temp;
	for (Parameter p : ruleIDs) {
		temp.push_back(p.toString());
	}
	return temp;
}

vector<string> predicate::getIDs() const
{
	vector<string> ids;
	for (Parameter p : ruleIDs) {
		if (p.checkID()) {
			ids.push_back(p.toString());
		}
	}
	return ids;
}
vector<int> predicate::idLocations() const {
	vector<int> locations;
	list<Parameter> temp = ruleIDs;
	int i = 0;
	while (!temp.empty()) {
		Parameter s = temp.front();
		if (s.checkID()) locations.push_back(i);
		i++;
		temp.pop_front();
	}
	return locations;
}
vector<list<int>> predicate::duplicates() const{
	vector<list<int>> duplicates;
	list<int> empty;
	for (int i = 0;i < ruleIDs.size();i++) {
		duplicates.push_back(empty);
	}
	vector<string> finder = getIDs();
	for (int i = 0;i < finder.size();i++) {
		for (int j = i + 1;j < finder.size();j++) {
			if (finder[i] == finder[j]) {
				duplicates[i].push_back(j);
			}
		}
	}
	return duplicates;
}
