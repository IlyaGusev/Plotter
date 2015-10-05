#pragma once
#include "Node.h"
class variableNode :
	public Node
{
public:
	variableNode();
	variableNode(double num);
	~variableNode();
	string toMathML();
	string toOpenMath();
	string toTeX();

private:
	double num;
};

