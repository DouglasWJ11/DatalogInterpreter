#pragma once
#include "node.h"
#include "rule.h"
#include <map>
#include <sstream>
#include <stack>
using namespace std;
class Graph
{
public:
	Graph(vector<rule>& rules);
	Graph();
	~Graph();
	void setRules(vector<rule>& rules);
	void createGraph();
	std::string getDependency();
	std::string getReverse();
	list<int> getPostOrder();
	list<int> getSccOrder();
	void reverseGraph();
	void setPostOrder();
	void postvisit(node& n);
	void explore(node& n);
	void exploreForward(node& n, set<int>& found);
	list<set<int>> findSCC();
	bool checkDependency(int i);
private:
	std::map<int,node> nodes;
	vector<rule> myRules;
	int post;
	stack<int> postOrder;
	list<int> sccOrder;
	list<int> postO;
};

