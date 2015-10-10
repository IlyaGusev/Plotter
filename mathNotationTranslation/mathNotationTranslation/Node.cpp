#include "Node.h"
Node::~Node() {}

void Node::computeResult(string & result, int operation, int notation)
{
}

string Node::toMathML(int notation)
{
	return string();
}

string Node::toOpenMath(int notation)
{
	return string();
}

string Node::toTeX(int notation)
{
	return string();
}

//void Node::setLeft(unique_ptr <Node> left) {
//	this->left = left;
//}
//
//void Node::setRight(unique_ptr <Node> right) {
//	this->right = right;
//}
//
//unique_ptr <Node> Node::getLeft() {
//	return left;
//}
//
//unique_ptr <Node> Node::getRight() {
//	return right;
//}
