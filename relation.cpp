#include "relation.h"


relation::relation(string named, Scheme schemed)
{
	name = named;
	scheme = schemed;
	matched = false;
}
relation::relation(){}

relation::~relation()
{
}

void relation::addVector(const Tuple &myTuple){
	strings.insert(myTuple);
}

string relation::getName() const{
	return name;
}

void relation::setName(string nam)
{
	name = nam;
}

Scheme relation::getScheme(){
	return scheme;
}

void relation::setScheme(vector<string>& schem)
{
	Scheme s;
	s.setAttributes(schem);
	scheme = s;
}

void relation::setScheme(Scheme temp) {
	scheme = temp;
}

set<Tuple> relation::getAttributes() {
	return strings;
}

void relation::setAttributes(set<Tuple>& temp)
{
	strings = temp;
}

bool relation::operator<(const relation& other) const {
	return name < other.getName();
}

void relation::setMatched(bool match){
	matched = match;
}

bool relation::match(){
	return matched;
}

bool relation::checkVariables(const predicate& q, const Tuple& t)
{
	map<string, string> variables;
	int i = 0;
	list<Parameter> myP = q.getParameters();
	for (Parameter p : myP) {
		if (!p.checkID()) {
			if (t.getValue(i) == p.toString()) {
				++i;
			}
			else return false;
		}
		else {
			if (variables.find(p.toString()) != variables.end()) {
				if (variables[p.toString()] == t.getValue(i)) {
					++i;
				}
				else return false;
			}
			else {
				variables.insert(std::pair<string, string>(p.toString(), t.getValue(i)));
				++i;
			}
		}
	}
	return true;
}

list<relation> relation::chooseProject(relation& source, relation& stemp, const predicate& query,vector<list<int>> dups) {
	relation ptemp, rtemp;
	vector<string> ids = query.getIDs();
	vector<int> loc = query.idLocations();
	if (ids.size() > 1) {
		vector<int> checked;
//		vector <list<int>> ck = query.duplicates();
		for (int i : loc) {
			if (dups[i].empty()) {
				checked.push_back(i);
			}
		}
		if (stemp.getName() == "") {
			ptemp = source.project(checked);
		}
		else {
			ptemp = stemp.project(checked);
		}
		rtemp = chooseRename(ptemp, ids, checked);
		
	}
	else if(ids.size() == 1) {
		if (stemp.getName() == "") {
			ptemp = source.project(loc);
		}
		else {
			ptemp = stemp.project(loc);
		}
		rtemp = ptemp.rename(ids[0], 0);
	}
	list<relation> result;
	result.push_back(ptemp);
	result.push_back(rtemp);
	return result;
}

relation relation::chooseRename(relation& ptemp, vector<string>& ids, vector<int>& checked) {
	relation rtemp;
	for (int i = 0;i < checked.size();++i) {
		if (rtemp.getName() == "") {
			rtemp = ptemp.rename(ids[i], i);
		}
		else {
			rtemp = rtemp.rename(ids[i], i);
		}
	}
	return rtemp;
}
relation relation::chooseSelect(const Tuple&t,list<Parameter> queryValues, relation& source,int& i){
	relation stemp;
	for (const Parameter & p:queryValues) {
		if (!p.checkID()) {
			if (stemp.getName() == "") {
				stemp = source.select(i, p.toString());
			}
			else {
				stemp = stemp.select(i, p.toString());
			}
		}
		i++;
	}
	return stemp;
}
list<relation> relation::evaluateRule(const predicate& query, relation& source){
	list<relation> results;		
	relation stemp;
	list<Parameter> queryValues = query.getParameters();
	vector<list<int>> dups = query.duplicates();
	set<Tuple> myT = source.getAttributes();
	int size=0;
	for(list<int> l:dups){
		size+=l.size();
	}
	if(size==0){
		stemp.setName(name);
		stemp.setScheme(scheme);
		stemp.setAttributes(strings);
	}
	for (const Tuple& t : myT) {
		int i = 0;
		if (checkVariables(query, t)) {
			if (query.getIDs().size() == queryValues.size()) {
				//stemp = source.select(dups);
			} else{
				stemp=chooseSelect(t,queryValues,source,i);
			}
		}
		else stemp = chooseSelect(t, queryValues, source, i);
	}

	list<relation> tt = chooseProject(source, stemp, query,dups);
	//tt.push_front(stemp);
	results.push_back(stemp);
	results.push_back(tt.front());
	tt.pop_front();
	results.push_back(tt.front());
	return results;
}
list<relation> relation::evaluateQuery(const predicate& query, relation& source)
{
	list<relation> results;		
	relation stemp;
	list<Parameter> queryValues = query.getParameters();
	vector<list<int>> dups = query.duplicates();
	set<Tuple> myT = source.getAttributes();

	for (const Tuple& t : myT) {
		int i = 0;
		if (checkVariables(query, t)) {
			if (query.getIDs().size() == queryValues.size()) {
				stemp = source.select(dups);
			} else{
				stemp=chooseSelect(t,queryValues,source,i);
			}
		}
	}

	list<relation> tt = chooseProject(source, stemp, query,dups);
	//tt.push_front(stemp);
	results.push_back(stemp);
	results.push_back(tt.front());
	tt.pop_front();
	results.push_back(tt.front());
	return results;
}


relation relation::select(int position, string value) {
	relation r(name, scheme);

	for (const Tuple& t : strings) {
		if (value == t.getValue(position)) {
			r.addVector(t);
		}
	}
	return r;
}

relation relation::select(vector<list<int>>& locations) {
	relation r(name, scheme);
	for (const Tuple& t : strings) {
		bool add = true;
		if (locations.empty()) {
			r.addVector(t);
			//break;
		}else{
			int i = 0;
			for (const list<int>& l : locations) {
				list<int> temp = l;
				while (!temp.empty()) {
					int first = temp.front();
					temp.pop_front();
					if (t.getValue(first) != t.getValue(i)) {
						add = false;
						break;
					}
				}
				i++;
			}
			if (add) {
				r.addVector(t);
			}
		}
	}
	return r;
}

relation relation::project(std::vector<int>& locations) {
	if (locations.size() == 0) return  relation();
	Scheme s;
	vector<string> temp;
	for (int i : locations) {
		temp.push_back(scheme.getValue(i));
	}
	s.setAttributes(temp);
	relation r(name, s);

	for (const Tuple& t : strings) {
		Tuple projected;
		for (int i : locations) {
			projected.add(t.getValue(i));
		}
		r.addVector(projected);
	}
	return r;
}

relation relation::rename(string s, int position) {
	relation r;
	Scheme temp = scheme;
	temp.setAtt(position, s);
	r.setName(name);
	r.setScheme(temp);
	r.setAttributes(strings);
	return r;
}

relation relation::unionWith(relation& toUnion)
{

	for (const Tuple& t : strings)
	{
		toUnion.addVector(t);
	}

	return toUnion;
}

relation relation::join(relation& r)
{
	relation s;
	Tuple d;
	vector<pair<int, int>> locations;
	s.setScheme(join(getScheme(),r.getScheme(), locations));
	set<Tuple> twoT = r.getAttributes();
	for (const Tuple& t:strings){
		for (const Tuple& tt : twoT) {

			if (joinable(t, tt, locations)) {
				d = join(t, tt, locations);
				s.addVector(d);
			}
		}
	}
	return s;
}

relation relation::join(relation& one, relation& two)
{
	relation r;
	Tuple d;
	vector<pair<int, int>> locations;
	r.setScheme(join(one.getScheme(), two.getScheme(), locations));
	set<Tuple> oneT = one.getAttributes();
	set<Tuple> twoT = two.getAttributes();
	for (const Tuple& t: oneT)
	{
		for (const Tuple& tt: twoT)
		{
			if (joinable(t, tt,locations))
			{
				d = join(t, tt,locations);
				r.addVector(d);
			}
		}
	}

	return r;
}


Scheme relation::join(Scheme s1, Scheme s2,vector<pair<int,int>>& locations)
{
	Scheme s;
	bool toAdd;
	vector<string> oneAtt = s1.getAttributes();
	vector<string> twoAtt = s2.getAttributes();
	int temp;
	for (int i = 0; i < twoAtt.size(); i++)
	{
		toAdd = true;
		for (int j = 0; j < oneAtt.size(); j++)
		{
			if (twoAtt[i] == oneAtt[j])
			{
				toAdd = false;
				temp = j;
				break;
			}
		}
		if (toAdd)
		{
			oneAtt.push_back(twoAtt[i]);
		}
		else {
			locations.push_back(pair<int, int>(temp, i));
		}
	}
	s.setAttributes(oneAtt);
	return s;
}

Tuple relation::join(const Tuple& t1, const Tuple& t2, vector<pair<int,int>>& locations)
{
	Tuple t;
	bool toAdd;
	for (int i = 0; i < t1.size(); ++i)
	{
		t.add(t1.getValue(i));
	}
	for (int j = 0;j < t2.size();++j) {
		toAdd = true;
		for (pair<int, int>p : locations) {
			if (j == p.second) {
				toAdd = false;
				break;
			}
		}
		if (toAdd) {
			t.add(t2.getValue(j));
		}
	}
	return t;
}

bool relation::joinable(const Tuple& t1, const Tuple& t2,vector<pair<int,int>>& locations)
{
	for (pair<int, int> p : locations) {
		if (t1.getValue(p.first)!=t2.getValue(p.second)) {
			return false;
		}
	}
	return true;
}

string relation::toString()
{
	stringstream ss;
	if (getName() == "") {
		return ss.str();
	}

	for (const Tuple& t : strings) {
		ss<<" ";
		for (int i = 0;i < t.size();i++) {
			vector<string> scheme = getScheme().getAttributes();
			ss << " " << scheme[i] << "=" << t.getValue(i);
			if (i == t.size() - 1) {
				ss << endl;
			}
		}
	}
	return ss.str();
}

int relation::size() {
	return strings.size();
}

vector<int> relation::findMatches(vector<string> sch) {
	vector<int> results;
	vector<string> curr = scheme.getAttributes();
	for (int i = 0;i < sch.size();++i) {
		for (int j = 0;j < curr.size();++j) {
			if (sch[i] == curr[j]) {
				results.push_back(j);
				break;
			}
		}
	}
	return results;
}

