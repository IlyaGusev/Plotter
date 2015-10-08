#pragma once
#include <string>
#include <memory>
#include <vector>
using namespace std;
class Node
{
public:
	virtual ~Node();
	virtual string toMathML();
	virtual string toOpenMath();
	virtual string toTeX();
	void setLeft(unique_ptr <Node> left);
	void setRight(unique_ptr <Node> right);
	unique_ptr <Node> getLeft();
	unique_ptr <Node> getRight();

//protected:
	unique_ptr <Node> left;
	unique_ptr <Node> right;
};

