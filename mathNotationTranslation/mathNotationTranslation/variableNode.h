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
	string toMathML(int notation);
	string toOpenMath(int notation);
	string toTeX(int notation);
	void computeResult(string& result, int notation);

private:
	string num;
};

