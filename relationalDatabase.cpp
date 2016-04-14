#include "relationalDatabase.h"
using namespace std;

relationalDatabase::relationalDatabase(datalogProgram& myProgram){
	program = myProgram;
	vector<rule> rr;
	int i = 0;
	for (rule r : myProgram.getRules()) {
		r.assignNumber(i);
		rr.push_back(r);
		i++;
	}
	g.setRules(rr);
	myRules = rr;
}

relationalDatabase::~relationalDatabase(){
}

string relationalDatabase::runRD(){
	createDatabase(program.getSchemes(), program.getFacts());
	output += factOut();
	output += "\nRule Evaluation\n\n";
	g.createGraph();
	g.reverseGraph();
	g.setPostOrder();
	list<set<int>> SCCs=g.findSCC();
	output += optimizeOut();
	for (set<int> scc : SCCs) {
		bool independent = false;
		list<rule> rules;
		output += "SCC:";
		for (int i : scc) {
			output += " R";
			output += to_string(i);
			rules.push_back(myRules[i]);
			if (scc.size() == 1) {
				independent=g.checkDependency(i);
			}
		}
		output += "\n";
		if (independent) {
			evalRules(rules);
		}
		else {
			doRules(rules);
		}
		output += "\n";
	}
	//doRules(myRules);
	output += "Rule Evaluation Complete\n";
	output += ruleOut();
	query();
	output += toString();
	return output;
}



string relationalDatabase::factOut() {
	list<predicate> myQ = program.getQueries();
	stringstream ss;
	ss << "Scheme Evaluation\n\nFact Evaluation\n";
	for (relation r : myRelations) {
		ss << endl << r.getName() << endl;
		ss << r.toString();
	}
	return ss.str();
}

string relationalDatabase::ruleOut() {
	stringstream ss;
	for (relation r : myRelations) {
		ss << endl << r.getName() << endl;
		ss << r.toString();
	}
	return ss.str();
}
string relationalDatabase::sendOut(rule r) {
	stringstream ss;
	ss << r.toString() << endl;
	for (relation s : newFacts) {
		if (s.getName() == r.getHead().getID()) {
			ss << s.toString();
		}
	}	
	return ss.str();
}
string relationalDatabase::cycleOut() {
	stringstream ss;
	for (rule r : myRules) {
		ss << r.toString() << endl;
		for (relation s : newFacts) {
			if (s.getName() == r.getHead().getID()) {
				ss << s.toString();
			}
		}
	}
	return ss.str();
}
void relationalDatabase::createDatabase(list<predicate> schemes, list<fact> myFacts){
	for (predicate p : schemes){
		Scheme s;
		s.setAttributes(p.getParamsasStrings());
		relation myrelation(p.getID(),s);
		for (fact f : myFacts){
			if (f.getID() == p.getID()){
				Tuple attributes;
				list<string> fa = f.getFacts();
				for (string s : fa){
					attributes.add(s);
				}
				myrelation.addVector(attributes);
			}
		}
		myRs.insert(pair<string,relation>(p.getID(), myrelation));
		myRelations.insert(myrelation);
	}
}

void relationalDatabase::query(){
	list<predicate> myQ = program.getQueries();
	for (predicate q : myQ){
		//for (relation r : myRelations) {
			//if (q.getID() == r.getName()) {
		relation r = myRs[q.getID()];
		list<relation> answers=r.evaluateQuery(q,r);
				select.push_back(answers.front());
				answers.pop_front();
				project.push_back(answers.front());
				answers.pop_front();
				rename.push_back(answers.front());
				answers.pop_front();
			//}
		//}
	}
}

int relationalDatabase::getSize() {
	int size = 0;
	for (relation r:myRelations) {
		size += r.size();
	}
	return size;
}

void relationalDatabase::doRules(list<rule>& rules)
{
	int sizeBefore, sizeAfter, passes;
	bool keepGoing = true;
	sizeBefore = sizeAfter = passes = 0;

	while (keepGoing)
	{
		sizeBefore = getSize();
		evalRules(rules);
		passes++;
		sizeAfter = getSize();

		if (sizeBefore != sizeAfter)
		{
			keepGoing = true;
		}
		else
		{
			keepGoing = false;
		}
	}

	numPasses = passes;
}

void relationalDatabase::evalRules(list<rule>& rules)
{
	for (rule r : rules) {
		list<relation> intermediates;
		list<predicate> myR = r.getPreds();
		for (const predicate& q : myR) {
		//	for (relation m : myRelations) {
			//	if (q.getID() == m.getName()) {
			relation m = myRs[q.getID()];
			list<relation> answers = m.evaluateRule(q, m);
					relation result = answers.back();
					intermediates.push_back(result);
					
				//}
			//}
					
		}

		combine(intermediates,r.getHead());
		output += sendOut(r);
		newFacts.clear();
	}
}

void relationalDatabase::combine(list<relation> &intermediates,predicate p) {
	if (intermediates.size() > 1) {
		while (intermediates.size()!=1) {
			relation one = intermediates.front();
			intermediates.pop_front();
			relation two = intermediates.front();
			intermediates.pop_front();
			one=one.join(two);
			intermediates.push_front(one);
		}
	}
	relation r = intermediates.front();
	r.setName(p.getID());
	Scheme s;
	s.setAttributes(p.getParamsasStrings());
	vector<int> checked = r.findMatches(s.getAttributes());
	r = r.project(checked);
	//relation m = myRs[r.getName()];
	for (relation m : myRelations) {
		if (r.getName() == m.getName()) {
			if (r.getAttributes().size() > 0){
				r=checkDups(r, m);
				newFacts.insert(r);
			}
			r=r.unionWith(m);
			
		}
	}
	myRs.erase(r.getName());
	myRs.insert(pair<string, relation>(r.getName(), r));
	myRelations.erase(r);
	myRelations.insert(r);
}

relation relationalDatabase::checkDups(relation& newTuples, relation& oldTuples) {
	set<Tuple> add;
	for(const Tuple& t:newTuples.getAttributes()){
		int size=oldTuples.size();
		oldTuples.addVector(t);
		if(oldTuples.size()>size){
			add.insert(t);
		}
	}
	relation r;
	r.setName(newTuples.getName());
	r.setScheme(oldTuples.getScheme());
	r.setAttributes(add);
	return r;
}

string relationalDatabase::optimizeOut()
{
	stringstream ss;
	ss << "Dependency Graph\n";
	ss << g.getDependency();
	ss << "\nReverse Graph\n";
	ss << g.getReverse();
	ss << "\nPostorder Numbers\n";
	int j = 0;
	for (int i : g.getPostOrder()) {
		ss << "  R" << j << ": " << i << endl;
		j++;
	}
	ss << "\nSCC Search Order\n";
	for (int i : g.getSccOrder()) {
		ss << "  R" << i << endl;
	}
	ss << endl;
	return ss.str();
}

string relationalDatabase::toString(){
	list<predicate> myQ = program.getQueries();
	stringstream ss;
	ss << "\nQuery Evaluation\n";
	for (int i = 0;i < select.size();i++) {
		ss << endl << myQ.front().toString();
		if (select[i].size()>0) {
			ss << "? Yes(" << select[i].size() << ")\nselect\n";
			ss << select[i].toString();
			ss << "project\n";
				ss << project[i].toString();
			ss << "rename\n";
				ss << rename[i].toString();
		}
		else {
			ss << "? No\n";
		}
		myQ.pop_front();
	}
	return ss.str();
}

