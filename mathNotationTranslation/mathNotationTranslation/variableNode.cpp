#include "variableNode.h"

variableNode::variableNode(string num): num(num) { }

variableNode::variableNode(){}

variableNode::~variableNode(){}

string variableNode::toMathML() { return ""; };
string variableNode::toOpenMath() { return ""; };
string variableNode::toTeX() { return ""; };

string variableNode::getValue() {
	return this->num;
}