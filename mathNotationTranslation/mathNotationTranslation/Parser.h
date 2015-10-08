#pragma once
#include "Node.h"
#include <iostream>
#include <fstream>  
class Parser
{
public:
	Parser();
	Parser(string input, string output);
	~Parser();
	bool processText(istream& is, int operation, unique_ptr<Node> position);
	string getType(int pos);
	void addNode();

private:
	string fileType[2];
	unique_ptr<Node> Tree;
};

