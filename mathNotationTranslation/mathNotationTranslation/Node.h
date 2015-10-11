#pragma once
#include <string>
#include <memory>
#include <vector>

using namespace std;
class Node
{
public:
	virtual ~Node();
	void computeResult(string& result, int operation, int notation);
	virtual string Translate(int notation);
	void setLeft(unique_ptr <Node> left);
	void setRight(unique_ptr <Node> right);
	unique_ptr<Node> getLeft();
	unique_ptr<Node> getRight();
	

protected:
	enum yytokentype {
	NUMBER = 258,
	ID,
	ADD,
	MUL,
	SUB,
	DIV,
	EQ,
	LOPER,
	ROPER,
	LNUM,
	RNUM,
	LID,
	RID,
	LROW,
	RROW,
	LROOT,
	RROOT,
	LFRAC,
	RFRAC,
	LSQRT,
	RSQRT,
	LSUP,
	RSUP
	};
	enum layout {
		mathml,
		openmath,
		tex
	};
	unique_ptr <Node> left;
	unique_ptr <Node> right;
};
