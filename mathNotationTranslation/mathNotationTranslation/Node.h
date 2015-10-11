#pragma once
#include <string>
#include <memory>
#include <vector>
using std::to_string;

using namespace std;
class Node
{
public:
	virtual ~Node();
	virtual string Translate(int notation);
};

class NumNode : public Node
{
public:
	float number;
	NumNode(float _number): number(_number) {}
	~NumNode(){}
	string Translate(int notation){
		return string(to_string(number));
	}
};

class IdNode : public Node
{
public:
	string id;
	IdNode(string _id): id(_id) {}
	~IdNode(){}
	string Translate(int notation){
		return id;
	}
};

class BinOpNode : public Node
{
public:
	Node* left;
	Node* right;
	string operation;

	BinOpNode(Node* _left, Node* _right, string _operation):
		operation(_operation), left(_left), right(_right) {}
	~BinOpNode(){
		delete left;
		delete right;
	}
	string Translate(int notation){
		return left->Translate(notation) + " " +operation + " " + right->Translate(notation);
	}
};

class CompositeNode : public Node
{
public:
	vector<Node*> nodes;

	CompositeNode() {}
	~CompositeNode(){
		for (int i=0; i<nodes.size(); i++){
			delete nodes[i];
		}
	}
	void add(Node* node){
		nodes.push_back(node);
	}
	string Translate(int notation){
		string s;
		for (int i=0; i<nodes.size(); i++){
			 s += nodes[i]->Translate(notation);
		}
		return s;
	}
};
