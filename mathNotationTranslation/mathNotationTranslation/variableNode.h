#pragma once
#include "Node.h"
class variableNode :
	public Node
{
public:
	variableNode();
	variableNode(string num);
	~variableNode();
	string getValue();
	string toMathML();
	string toOpenMath();
	string toTeX();

private:
	string num;
};

