#include "Node.h"

Node::~Node() {}

string Node::toMathML()
{
	return string();
}

string Node::toOpenMath()
{
	return string();
}

string Node::toTeX()
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
