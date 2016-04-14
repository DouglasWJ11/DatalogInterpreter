#include "node.h"



node::node()
{
	visited=false;
}


node::~node()
{
}

void node::addDependent(int i)
{
	dependents.insert(i);
}

void node::setVisited(bool visit)
{
	visited = visit;
}

bool node::getVisited() const
{
	return visited;
}

std::string node::toString() const 
{
	stringstream ss;
	for (int i : dependents) {
		ss << " R" << i ;
	}
	return ss.str();
}

std::string node::reverseGraph() const
{
	stringstream ss;
	for (int i : reverse) {
		ss << " R" << i;
	}
	return ss.str();
}

set<int> node::getDependents() const 
{
	return dependents;
}

set<int> node::getReverse() const
{
	return reverse;
}

void node::addReverse(int r)
{
	reverse.insert(r);
}

void node::setPost(int p)
{
	postorderNumber = p;
}

int node::getPost()
{
	return postorderNumber;
}

void node::setName(int i)
{
	name = i;
}

int node::getName()
{
	return name;
}

