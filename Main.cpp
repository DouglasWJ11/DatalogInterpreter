#include"token.h"
#include"datalogProgram.h"
#include"relationalDatabase.h"
#include<list>
#include<fstream>
#include<cctype>
#include<iostream>
using namespace std;

ofstream output;
void addToken(string text, int lineNumber, string tokenType, list<token>&tokenList){
	token t(text, lineNumber, tokenType);
	tokenList.push_back(t);
	//output << t.output();
}

token alphaToken(char c, int lineNumber, ifstream &input){
	string s;
	s += c;
	while (isalnum(input.peek())){
		s += input.get();
	}
	if (s == "Schemes"){
		token tk(s, lineNumber, "SCHEMES");
		return tk;
	}
	else if (s == "Facts"){
		token tk(s, lineNumber, "FACTS");
		return tk;
	}
	else if (s == "Rules"){
		token tk(s, lineNumber, "RULES");
		return tk;
	}
	else if (s == "Queries"){
		token tk(s, lineNumber, "QUERIES");
		return tk;
	}
	else {
		token tk(s, lineNumber, "ID");
		return tk;
	}
}

void whitespace(char c,int& lineNumber){
	if (c == '\n'){
		lineNumber++;
	}
}

token quote(char c, int &lineNumber, ifstream &input){
	string s;
	s += c;
	int start = lineNumber;
	char temp;
	bool end = false;
	while (end != true){
		input.get(temp);
		if (temp == '\''){
			//s += temp;
			if (input.peek() != '\''){
				end = true;
			}
			else if (input.peek() == '\''){
				s += input.get();
			}
		}
		else if (input.peek() == EOF){
			s += temp;
			if (temp == '\n')lineNumber++;
			token tk(s, start, "UNDEFINED");
			end = true;
			//output << "Input Error on Line " << start;
			throw 20;
		}
		else if (temp == '\n'){
			lineNumber++;
			s += temp;
		}
		s += temp;
	}
	token tk(s, start, "STRING");
	//output << tk.output();
	return tk;
}

token comment(char c, int &lineNumber, ifstream &input){
	string s;
	s += c;
	int start = lineNumber;
	while (input.peek() != '#'){
		while (input.peek() != '|'){
			s += input.get();
			if (input.peek() == EOF){
				token tk(s, start, "UNDEFINED");
				return tk;
			}
			else if (input.peek() == '\n') lineNumber++;
		}
		s += input.get();
	}
	s += input.get();
	token tk(s, start, "COMMENT");
	return tk;
}

int main(int argc, char* argv[]){
	ifstream input(argv[1]);
	output.open(argv[2]);
	list<token> tokenList;
	int lineNumber=1;
	while (input.peek()!=EOF){
		char c;
		string s;
		input.get(c);
		if (isalpha(c)){
			token t = alphaToken(c, lineNumber, input);
			//output << t.output();
			tokenList.push_back(t);
		}
		else if (isspace(c)){
			whitespace(c, lineNumber);
		}
		else{
			switch (c){
			case ',':
				addToken(string(1, c), lineNumber, "COMMA", tokenList);
				break;
			case '.':
				addToken(string(1, c), lineNumber, "PERIOD", tokenList);
				break;
			case '?':
				addToken(string(1, c), lineNumber, "Q_MARK", tokenList);
				break;
			case'(':
				addToken(string(1, c), lineNumber, "LEFT_PAREN", tokenList);
				break;
			case')':
				addToken(string(1, c), lineNumber, "RIGHT_PAREN", tokenList);
				break;
			case':':
				if (input.peek() == '-'){
					s += c;
					 s+= input.get();
					addToken(s, lineNumber, "COLON_DASH", tokenList);
				}
				else addToken(string(1, c), lineNumber, "COLON", tokenList);
				break;
			case'*':
				addToken(string(1, c), lineNumber, "MULTIPLY", tokenList);
				break;
			case'+':
				addToken(string(1, c), lineNumber, "ADD", tokenList);
				break;
			case'\'':
				try{
					tokenList.push_back(quote(c, lineNumber, input));
				}
				catch (int e){
					return 0;
				}
				break;
			case'#':
				if (input.peek() == '|'){
					tokenList.push_back(comment(c, lineNumber, input));
				}
				else{
					s += c;
					while (input.peek() != '\n'){
						s += input.get();
					}
					s, lineNumber, "COMMENT", tokenList;
				}
				break;
			default:
				//output << "Input Error on line " << lineNumber;
				return 0;
			}
		}
	}
	input.close();
	addToken("", lineNumber, "EOF",tokenList);
/*	for (token t : tokenList){
		output << t.output();
	}*/
	//output << "Total Tokens = "<<tokenList.size() << endl;
	datalogProgram dataLog(tokenList);
	try{
		dataLog.runProgram();
	}
	catch(token t){
		output << "Failure!\n  " << t.getData();
	}
	relationalDatabase myData(dataLog);
	output<<myData.runRD();
	output.close();
	return 0;
}
