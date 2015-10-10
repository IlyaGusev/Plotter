#include "variableNode.h"
#include <iostream>

variableNode::variableNode(string num): num(num) { }

variableNode::variableNode(){}

variableNode::~variableNode(){}

string variableNode::toMathML(int notation) {
	string result;
	computeResult(result, notation);
	return result;
};
string variableNode::toOpenMath(int notation) { return ""; };
string variableNode::toTeX(int notation) { return ""; }
void variableNode::computeResult(string& result, int notation)
{
	char* endptr = 0;
	strtod(num.c_str(), &endptr);

	if (*endptr != '\0' || endptr == num) {
		switch (notation) {
		case mathml:
			result = " <mi> " + num + " </mi> ";
		case tex:
			result = num;
		case openmath:
			result = num;
		}
	}
	else {
		switch (notation) {
		case mathml:
			result = " <mn> " + num + " </mn> ";
		case tex:
			result = num;
		case openmath:
			result = num;
		}
	}
};

string variableNode::getValue() {
	return this->num;
}