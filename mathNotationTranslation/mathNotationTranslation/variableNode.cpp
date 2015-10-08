#include "variableNode.h"
#include <iostream>

variableNode::variableNode(string num): num(num) { }

variableNode::variableNode(){}

variableNode::~variableNode(){}

string variableNode::toMathML() { 
	char* endptr = 0;
	strtod(num.c_str(), &endptr);
	string result;

	if (*endptr != '\0' || endptr == num) {
		result = " <mi> " + num + " </mi> ";
		return result;
	}
	else {
		result = " <mn> " + num + " </mn> ";
		return result;
	}
};
string variableNode::toOpenMath() { return ""; };
string variableNode::toTeX() { return ""; };

string variableNode::getValue() {
	return this->num;
}