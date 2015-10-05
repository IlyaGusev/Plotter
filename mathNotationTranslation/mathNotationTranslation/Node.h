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
};

