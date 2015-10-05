#include "addNode.h"



addNode::addNode()
{
}


addNode::~addNode()
{
	this->expLeft.release();
	this->expRight.release();
}

std::string addNode::toMathML()
{
	return std::string();
}

std::string addNode::toOpenMath()
{
	return std::string();
}

std::string addNode::toTeX()
{
	return std::string();
}
