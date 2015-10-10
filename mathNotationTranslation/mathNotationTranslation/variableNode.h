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
	string Translate(int notation);
	void computeResult(string& result, int notation);

private:
	string num;
};

