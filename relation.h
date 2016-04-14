#pragma once
#include<string>
#include<vector>
#include<set>
#include<list>
#include<sstream>
#include<map>
#include"Parameter.h"
#include"predicate.h"
#include"tuple.h"
#include"Scheme.h"
using namespace std;
class relation
{
private:
	string name;
	Scheme scheme;
	set<Tuple> strings;
	bool matched;
public:
	relation(string named, Scheme schemed);
	relation();
	~relation();
	void addVector(const Tuple &myTuple);
	string getName() const;
	void setName(string nam);
	Scheme getScheme();
	void setScheme(vector<string>& schem);
	void setScheme(Scheme temp);
	set<Tuple> getAttributes() ;
	void setAttributes(set<Tuple>& temp);
	bool operator<(const relation& other)const;
	void setMatched(bool match);
	bool match();
	bool checkVariables(const predicate& q, const Tuple& t);
	list<relation> chooseProject(relation& source, relation& stemp, const predicate& query, vector<list<int>> dups);
	relation chooseRename(relation& ptemp, vector<string>& ids, vector<int>& checked);
	relation chooseSelect(const Tuple&t,list<Parameter> queryValues, relation& source,int& i);
	list<relation> evaluateRule(const predicate& query, relation& source);
	list<relation> evaluateQuery(const predicate& query, relation& source);
	relation select(int position, string value);
	relation select(vector<list<int>>& locations);
	relation project(std::vector<int>& locations);
	relation rename(string s,int location);
	relation unionWith(relation& toUnion);
	relation join(relation &r);
	relation join(relation &one, relation& two);
	Scheme join(Scheme s1, Scheme s2, vector<pair<int, int>>& locations);
	Tuple join(const Tuple& t1, const Tuple& t2, vector<pair<int,int>>& locations);
	bool joinable(const Tuple& t1, const Tuple& t2,vector<pair<int,int>>& locations);
	string toString();
	int size();
	vector<int> findMatches(vector<string> sch);
};

