#pragma once
#include "Node.h"
class addNode :
	public Node
{
public:
	addNode();
	~addNode();
	std::string toMathML();
	std::string toOpenMath();
	std::string toTeX();

protected:
	std::unique_ptr<Node> expLeft;
	std::unique_ptr<Node> expRight;
};

