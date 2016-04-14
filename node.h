#pragma once
#include<set>
#include<string>
#include<sstream>
using namespace std;
class node
{
public:
	node();
	~node();
	void addDependent(int i);
	void setVisited(bool visit);
	bool getVisited() const;
	std::string toString() const;
	std::string reverseGraph()const;
	set<int> getDependents() const ;
	set<int> getReverse() const;
	void addReverse(int r);
	void setPost(int p);
	int getPost();
	void setName(int i);
	int getName();
private:
	std::set<int> dependents;
	std::set<int> reverse;
	bool visited;
	int postorderNumber;
	int name;
};

