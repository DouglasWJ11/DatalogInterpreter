#pragma once
#include <string>
#include <sstream>
using namespace std;
class token
{
public:
	token(string input, int line, string type);
	~token();
	string output();
	string typeToken();
	string getValue();
	string getData();
private:
	string text;
	int lineNumber;
	string tokenType;
};

