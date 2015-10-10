#include "arithmeticNode.h"
#include <iostream>
#include <map>

#define thisLeft ((Node*) this)->left
#define thisRight ((Node*) this)->right

const map<int, array<string, 3>> arithmeticNode::notations = createMap();

void arithmeticNode::computeResult(string& result, int notation) {
	if (thisLeft != NULL)
		result += thisLeft->Translate(notation);
	result += notations.at(operation)[notation];
	if (thisRight != NULL)
		result += thisRight->Translate(notation);
}

arithmeticNode::arithmeticNode()
{
}


arithmeticNode::~arithmeticNode()
{
}

string arithmeticNode::Translate(int notation) {
	string result;
	computeResult(result, notation);
	return result;
};
