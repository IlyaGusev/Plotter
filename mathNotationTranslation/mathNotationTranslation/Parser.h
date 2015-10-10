#pragma once
#include "Node.h"
#include <iostream>
#include <fstream>
class Parser
{
public:
	Parser();
	~Parser();
	bool processText(istream& is, unique_ptr <Node>& position);
	void addNode();
	unique_ptr <Node> prev;
	void arithmeticOperation(istream& is, int oper, unique_ptr <Node>& position);
};
