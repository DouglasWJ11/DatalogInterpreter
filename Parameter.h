#pragma once
#include<string>
using namespace std;
class Parameter
{
public:
	Parameter(string it, bool type);
	~Parameter();
	string toString() const;
	string getName();
	void setName(string n);
	void setValue(string v);
	bool checkID() const;
private:
	string value;
	string name;
	bool isID;
};

