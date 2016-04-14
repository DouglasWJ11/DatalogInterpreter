#include "token.h"
using namespace std;

token::token(string input,int line,string type){
	lineNumber = line;
	text = input;
	tokenType = type;
}
string token::typeToken(){
	return tokenType;
}
token::~token(){
}
string token::output(){
	stringstream ss;
	ss << "("<<tokenType << ",\"" << text << "\"," << lineNumber <<")"<< endl;
	return ss.str();
}
string token::getValue() {
	return text;
}
string token::getData() {
	stringstream ss;
	ss << "(" << tokenType << ",\"" << text << "\"," << lineNumber << ")";
	return ss.str();
}