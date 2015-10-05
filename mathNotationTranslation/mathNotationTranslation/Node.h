#pragma once
#include <string>
#include <memory>
using namespace std;
class Node
{
public:
	virtual ~Node();
	virtual string toMathML();
	virtual string toOpenMath();
	virtual string toTeX();
};

