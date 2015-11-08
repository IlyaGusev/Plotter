#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <array>
using namespace std;

enum LANGUAGE{
	MATHML,
	OPENMATH,
	TEX
};

class Node{
protected:
	string lfence;
	string rfence;
public:
	virtual ~Node(){}
	virtual string translate(int notation) const;
	static map<string, array<string, 3>> createMap();
	virtual void setFence(string _lfence, string _rfence);
	virtual string addFence(int notation, string _s) const;
};

class NumNode : public Node{
private:
	float number;
public:
	NumNode(float _number);
	~NumNode();
	string translate(int notation) const;
};

class IdNode : public Node{
private:
	string id;
public:
	IdNode(string _id);
	~IdNode();
	string translate(int notation) const;
};

class BinOpNode : public Node{
private:
	Node* left;
	Node* right;
	string operation;
public:
	BinOpNode(Node* _left, Node* _right, string _operation);
	~BinOpNode();
	string translate(int notation) const;
};

class CompositeNode : public Node{
private:
	vector<Node*> nodes;
public:
	CompositeNode(string, string);
	CompositeNode(Node*);
	CompositeNode(Node*, string, string);
	~CompositeNode();
	void add(Node* _node);
	string translate(int notation) const;
};

#endif // NODE_H_INCLUDED
