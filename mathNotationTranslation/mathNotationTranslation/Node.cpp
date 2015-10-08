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

void Node::setLeft(unique_ptr<Node> left) {
	this->left.reset();
	this->left = move(left);
}

void Node::setRight(unique_ptr<Node> right) {
	this->right.reset();
	this->right = move(right);
}

unique_ptr<Node> Node::getLeft() {
	return move(left);
}

unique_ptr<Node> Node::getRight() {
	return move(right);
}
