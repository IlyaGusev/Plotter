// #include "arithmeticNode.h"
// #include <iostream>
// #include <map>
//
// const map<int, array<string, 3>> arithmeticNode::notations = createMap();
//
// void arithmeticNode::computeResult(string& result, int notation) {
// 	unique_ptr<Node> tmpLeft = getLeft();
// 	unique_ptr<Node> tmpRight = getRight();
//
// 	if (tmpLeft != NULL)
// 		result += tmpLeft->Translate(notation);
// 	result += notations.at(operation)[notation];
// 	if (tmpRight != NULL)
// 		result += tmpRight->Translate(notation);
// }
//
// arithmeticNode::arithmeticNode()
// {
// }
//
//
// arithmeticNode::~arithmeticNode()
// {
// }
//
// string arithmeticNode::Translate(int notation) {
// 	string result;
// 	computeResult(result, notation);
// 	return result;
// };
//
//
// void arithmeticNode::setLeft(unique_ptr <Node> left) {
// 	this->left = move(left);
// }
//
// void arithmeticNode::setRight(unique_ptr <Node> right) {
// 	this->right = move(right);
// }
//
// unique_ptr <Node> arithmeticNode::getLeft() {
// 	return move(((Node*)this)->getLeft());
// }
//
// unique_ptr <Node> arithmeticNode::getRight() {
// 	return move(((Node*)this)->getRight());
// }
//
// void arithmeticNode::setOperation(int oper) {
// 	this->operation = oper;
// }
