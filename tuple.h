#pragma once
#include<vector>
#include<string>

class Tuple
{
public:
	Tuple();
	~Tuple();
	void add(std::string s);
	int size() const;
	std::string getValue(int position) const;
	bool operator<(const Tuple &other)const;
	bool operator==(const Tuple &other) const;
	std::vector<std::string> getAttributes();
private:
	std::vector<std::string> attributes;
};

