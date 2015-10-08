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
	bool processText(istream& is, unique_ptr <Node>& position);
	string getType(int pos);
	void addNode();
	unique_ptr <Node> prev;
	void arithmeticOperation(istream& is, int oper, unique_ptr <Node>& position);

private:
	string fileType[2];
};

