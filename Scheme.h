#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#pragma once
using namespace std;


class Scheme{
public:
	Scheme();
	~Scheme();
	void setAttributes(vector<string> att);
	void setAtt(int position, string s);
	vector<string> getAttributes();
	string getValue(int i);
	bool contains(string);
	void addAtt(string s);
private:
	vector<string> myAttributes;	
};