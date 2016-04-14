#include "datalogProgram.h"


datalogProgram::datalogProgram(list<token> myTokens)
{
	tokens = myTokens;
}
datalogProgram::datalogProgram()
{
}
datalogProgram::~datalogProgram()
{
}
string datalogProgram::runProgram(){
	schemes();
	facts();
	rules();
	queries();
	if (tokens.front().typeToken() == "EOF") {
		tokens.pop_front();
	}
	if (!tokens.empty()) {
		throw tokens.front();
	}
	return output();
}
void datalogProgram::queries()
{
	if (tokens.front().typeToken() == "QUERIES"){
		tokens.pop_front();
		if (tokens.front().typeToken() == "COLON"){
			tokens.pop_front();
			checkQuery();
			queryList();
		}
		else throw(tokens.front());
	}
	else throw(tokens.front());
}
void datalogProgram::checkQuery()
{
	if (tokens.front().typeToken() == "ID"){
		predicate query(checkPredicate());
		if (tokens.front().typeToken() == "Q_MARK"){
			tokens.pop_front();
			myQueries.push_back(query);
		}
		else throw(tokens.front());
	}
	else throw(tokens.front());
}

void datalogProgram::queryList()
{
	if (tokens.front().typeToken() == "ID"){
		checkQuery();
		queryList();
	}
}
void datalogProgram::schemes()
{
	if (tokens.front().typeToken() == "SCHEMES"){
		tokens.pop_front();
		if (tokens.front().typeToken() == "COLON"){
			tokens.pop_front();
			checkScheme();
			schemeList();
		}
		else throw(tokens.front());
	}
}

bool datalogProgram::checkScheme()
{
	if (tokens.front().typeToken() == "ID"){
		predicate myScheme(tokens.front().getValue());
		tokens.pop_front();
		if (tokens.front().typeToken() == "LEFT_PAREN"){
			tokens.pop_front();
			if (tokens.front().typeToken() == "ID" || tokens.front().typeToken() == "STRING"){
				if (tokens.front().typeToken() == "STRING") {
					domains.insert(tokens.front().getValue());
				}
				currentList.push_back(tokens.front().getValue());
				tokens.pop_front();
				idList();
				for (string s : currentList){
					Parameter myP(s,true);
					myScheme.addID(myP);
				}
				currentList.erase(currentList.begin(), currentList.end());
				mySchemes.push_back(myScheme);
				if (tokens.front().typeToken() == "RIGHT_PAREN"){
					tokens.pop_front();
					return true;
				}
				else throw(tokens.front());
			}
			else throw(tokens.front());
		}
		else throw(tokens.front());
	}
	else throw(tokens.front());
}


void datalogProgram::schemeList()
{
	if (tokens.front().typeToken()=="ID"){
		checkScheme();
		schemeList();
	}
}


void datalogProgram::facts()
{
	if (tokens.front().typeToken() == "FACTS"){
		tokens.pop_front();
		if (tokens.front().typeToken() == "COLON"){
			tokens.pop_front();
			if (tokens.front().typeToken() == "ID"){
				checkFacts();
				factList();
			}
		}
	}
}


bool datalogProgram::checkFacts()
{
	if (tokens.front().typeToken() == "ID"){
		fact myFact(tokens.front().getValue());
		tokens.pop_front();
		if (tokens.front().typeToken() == "LEFT_PAREN"){
			tokens.pop_front();
			if (tokens.front().typeToken() == "STRING"||tokens.front().typeToken()=="ID"){
				currentList.push_back(tokens.front().getValue());
				tokens.pop_front();
				stringList();
				for (string s : currentList){
					myFact.addFacts(s);
				}
				currentList.erase(currentList.begin(), currentList.end());
				myFacts.push_back(myFact);
				if (tokens.front().typeToken() == "RIGHT_PAREN"){
					tokens.pop_front();
					if (tokens.front().typeToken() == "PERIOD"){
						tokens.pop_front();
						return true;
					}
					else{
						throw(tokens.front());
					}
				}
				else{
					throw(tokens.front());
				}
			}
			else{
				throw(tokens.front());
			}
		}
		else{
			throw(tokens.front());

		}
	}
	else{
		throw(tokens.front());
	}
}

void datalogProgram::factList()
{
	if (tokens.front().typeToken()=="ID"){
		checkFacts();
		factList();
	}
}


void datalogProgram::rules()
{
	if (tokens.front().typeToken() == "RULES"){
		tokens.pop_front();
		if (tokens.front().typeToken() == "COLON"){
			tokens.pop_front();
			if (tokens.front().typeToken() == "ID"||tokens.front().typeToken()=="STRING"){
				checkRule();
				ruleList();
			}
		}
		else throw(tokens.front());
	}
	else throw(tokens.front());
}

void datalogProgram::ruleList()
{
	if (tokens.front().typeToken() == "ID"){
		checkRule();
		ruleList();
	}
}


void datalogProgram::checkRule()
{
	if (tokens.front().typeToken()=="ID"){
		rule myRule(checkPredicate());
		myPredicates.erase(myPredicates.begin(), myPredicates.end());
		if (tokens.front().typeToken() == "COLON_DASH"){
			tokens.pop_front();
			if (tokens.front().typeToken() == "ID") {
				myPredicates.push_back(checkPredicate());
				predicateList();
				if (tokens.front().typeToken() == "PERIOD") {
					tokens.pop_front();
					for (predicate p : myPredicates) {
						myRule.addPredicate(p);
					}
				}
				else throw tokens.front();
			}
			else throw(tokens.front());
		}
		else throw(tokens.front());
		myRules.push_back(myRule);
	}
	else throw(tokens.front());
}


void datalogProgram::failure(token t)
{
	cout << "FAILURE!\n  " << t.getData()<<endl;
	return;
}


void datalogProgram::idList()
{
	if (tokens.front().typeToken() == "COMMA"){
		tokens.pop_front();
		if (tokens.front().typeToken() == "ID" || tokens.front().typeToken() == "STRING"){
			currentList.push_back(tokens.front().getValue());
			tokens.pop_front();
			idList();
		}
	}
}
string datalogProgram::output(){
	//set<string> domains;
	stringstream ss;
	ss << "Success!" << endl;
	ss << "Schemes(" << mySchemes.size() << "):\n";
	for (predicate s : mySchemes){
		ss <<"  "<< s.toString() << endl;
	}
	ss << "Facts(" << myFacts.size() << "):\n";
	for (fact f : myFacts){
		ss << "  "<<f.toString() << endl;
		list<string> temp = f.getFacts();
		for (string t : temp){
			domains.insert(t);
		}
	}
	ss << "Rules(" << myRules.size() << "):\n";	
	for (rule r : myRules){
		ss << "  " << r.toString() << endl;
		
	}
	ss << "Queries(" << myQueries.size() << "):\n";
	for (predicate q : myQueries){
		ss << "  " << q.toString() <<endl;
	}
	ss << "Domain(" << domains.size() << "):\n";
	for (string d : domains){
		ss << "  "<< d << endl;
	}
//	cout<< ss.str();
	return ss.str();
}


void datalogProgram::stringList()
{
	if (tokens.front().typeToken() == "COMMA"){
		tokens.pop_front();
		if (tokens.front().typeToken() == "STRING"||tokens.front().typeToken()=="ID"){
			currentList.push_back(tokens.front().getValue());
			tokens.pop_front();
			stringList();
		}
	}
}

void datalogProgram::predicateList()
{
	if (tokens.front().typeToken() == "COMMA"){
		tokens.pop_front();
		if (tokens.front().typeToken() == "ID") {
			myPredicates.push_back(checkPredicate());
			predicateList();
		}
		else throw tokens.front();
	}
}

predicate datalogProgram::checkPredicate()
{
		predicate headPredicate(tokens.front().getValue());
		tokens.pop_front();
		if (tokens.front().typeToken() == "LEFT_PAREN"){
			tokens.pop_front();
			string temp = tokens.front().typeToken();
			if (temp == "ID" || temp == "STRING" || temp == "LEFT_PAREN"){
				headPredicate.addID(checkParameter());
				parameterList();
				for (Parameter p : myParameters){
					headPredicate.addID(p);
				}
				myParameters.erase(myParameters.begin(), myParameters.end());
				if (tokens.front().typeToken() == "RIGHT_PAREN"){
					tokens.pop_front();
					return headPredicate;
				}
				else throw(tokens.front());
			}
			else throw(tokens.front());
		}
		else throw(tokens.front());
}

Parameter datalogProgram::checkParameter()
{
	string temp = tokens.front().typeToken();
	if (temp == "STRING"){
		Parameter myParameter(tokens.front().getValue(),false);
		myParameter.setValue(tokens.front().getValue());
		domains.insert(tokens.front().getValue());
		tokens.pop_front();
		return myParameter;
	}
	else if (temp == "ID") {
		Parameter myParameter(tokens.front().getValue(),true);
		myParameter.setName(tokens.front().getValue());
		tokens.pop_front();
		return myParameter;
	}
	else if (temp == "LEFT_PAREN"){
		tokens.pop_front();
		Parameter myParameter(checkExpression().toString(),false);
		return myParameter;
	}
	else{
		throw(tokens.front());
	}
}


expression datalogProgram::checkExpression()
{
	Parameter first=checkParameter();
	string op = tokens.front().typeToken();
	if (op == "ADD" || op == "MULTIPLY"){
		string oper = tokens.front().getValue();
		tokens.pop_front();
		Parameter second = checkParameter();
		expression myExpression(first, oper, second);
		if (tokens.front().typeToken() == "RIGHT_PAREN"){
			tokens.pop_front();
			return myExpression;
		}
		else throw(tokens.front());
	}
	else{
		throw(tokens.front());
	}
}


void datalogProgram::parameterList()
{
	if (tokens.front().typeToken() == "COMMA"){
		tokens.pop_front();
		myParameters.push_back(checkParameter());
		parameterList();
	}
}

list<predicate> datalogProgram::getSchemes()
{
	return mySchemes;
}


list<fact> datalogProgram::getFacts()
{
	return myFacts;
}


list<rule> datalogProgram::getRules()
{
	return myRules;
}


list<predicate> datalogProgram::getQueries()
{
	return myQueries;
}
