#include "Graph.h"



Graph::Graph(vector<rule>& rules)
{
	myRules = rules;
	post = 1;
}

Graph::Graph()
{
}


Graph::~Graph()
{
}

void Graph::setRules(vector<rule>& rules) {
	myRules = rules;
}
void Graph::createGraph()
{
	map<string, list<int>> reference;
	int i = 0;
	for (rule r : myRules) {
		if (reference.find(r.getHead().getID()) == reference.end()) {
			list<int> rList;
			rList.push_back(i);
			reference.insert(pair<string, list<int>>(r.getHead().getID(), rList));
			i++;
		}
		else {
			reference[r.getHead().getID()].push_back(i);
			i++;
		}
	}
	i = 0;
	for (rule r : myRules) {
		node n;
		n.setName(i);
		for (predicate p : r.getPreds()) {
			for (int i : reference[p.getID()]) {
				n.addDependent(i);
			}
		}
		nodes.insert(pair<int,node>(i, n));
		i++;
	}
}

std::string Graph::getDependency()
{
	stringstream ss;
	for (int i = 0;i < nodes.size();i++) {
		ss << "  R" << i << ":" << nodes[i].toString() << endl;
	}
	return ss.str();
}

std::string Graph::getReverse()
{
	stringstream ss;
	for (int i = 0;i < nodes.size();i++) {
		ss << "  R" << i << ":" << nodes[i].reverseGraph() << endl;
	}
	return ss.str();
}

list<int> Graph::getPostOrder()
{
	set<pair<int, int>> order;
	int j = 1;
	for (int i : postO) {
		order.insert(pair<int, int>(i, j));
		j++;
	}
	list<int> yay;
	for (pair<int, int> p : order) {
		yay.push_back(p.second);
	}
	return yay;
}

list<int> Graph::getSccOrder()
{
	return sccOrder;
}

void Graph::reverseGraph()
{
	list<set<int>> dependents;
	for (int i = 0;i < nodes.size();i++) {
		dependents.push_back(nodes[i].getDependents());
	}
	int i = 0;
	for (set<int> d : dependents) {
		for (int j : d) {
			nodes[j].addReverse(i);
		}
		i++;
	}
}

void Graph::setPostOrder()
{
	int post = 1;
	for (pair<const int, node> & q : nodes) {
		if (!q.second.getVisited()) {
			explore(q.second);
		}
	}
}

void Graph::postvisit(node& n) {
	n.setPost(post);
	post++;
	postOrder.push(n.getName());
}

void Graph::explore(node& n)
{
	n.setVisited(true);
	for (int i : n.getReverse()) {
		if (!nodes[i].getVisited()) {
			explore(nodes[i]);
		}
	}
	postvisit(n);
}

void Graph::exploreForward(node & n, set<int>& found)
{
	n.setVisited(true);
	for (int i : n.getDependents()) {
		if (!nodes[i].getVisited()) {
			exploreForward(nodes[i], found);
			found.insert(i);
		}		
	}
}

list<set<int>> Graph::findSCC()
{
	list<set<int>> scc;
	for (pair<const int, node> &p:nodes) {
		p.second.setVisited(false);
	}
	while (!postOrder.empty()) {
		postO.push_front(postOrder.top());
		sccOrder.push_back(postOrder.top());
		if (!nodes[postOrder.top()].getVisited()) {
			set<int> found;
			found.insert(postOrder.top());
			exploreForward(nodes[postOrder.top()], found);
			postOrder.pop();
			scc.push_back(found);
		}
		else {
			
			postOrder.pop();
		}
	}
	return scc;
}

bool Graph::checkDependency(int i)
{
	if (nodes[i].getDependents().size() == 0) {
		return true;
	}
	else if (nodes[i].getDependents().size()>=1) {
		set<int> dep = nodes[i].getDependents();
		if (dep.find(i) == dep.end()) {
			return true;
		}
	}
	else {
		return false;
	}
	return false;

}




