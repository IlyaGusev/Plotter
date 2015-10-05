#include "variableNode.h"

variableNode::variableNode(double num) :num(num){}

variableNode::variableNode(){}

variableNode::~variableNode(){}

string variableNode::toMathML() { return ""; };
string variableNode::toOpenMath() { return ""; };
string variableNode::toTeX() { return ""; };