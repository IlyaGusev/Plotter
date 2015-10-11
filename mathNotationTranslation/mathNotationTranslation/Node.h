#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <array>
using namespace std;
int NOTATION = 0;
enum LANGUAGE{
	MATHML,
	OPENMATH,
	TEX
};

class Node
{
public:
	virtual ~Node(){}
	virtual string translate(int notation)
	{
		return string();
	}
	static map<string, array<string, 3>> createMap()
	{
		map<string, array<string, 3>> m;
		m["+"] = { " <mo> + </mo> ", "", " + " };
		m["-"] = { " <mo> - </mo> ", "", " - " };
		m["*"] = { " <mo> * </mo> ", "", " * " };
		m["/"] = { " <mo> / </mo> ", "", " / " };
		m["="] = { " <mo> = </mo> ", "", " = " };
		return m;
	}
};

class NumNode : public Node
{
public:
	float number;
	NumNode(float _number): number(_number) {}
	~NumNode(){}
	string translate(int notation){
		string result;
		string num = string(to_string(number));
		switch (notation) {
			case MATHML:
				result = " <mn> " + num + " </mn> ";
				break;
			case OPENMATH:
				result = num;
				break;
			case TEX:
				result = num;
				break;
		}
		return result;
	}
};

class IdNode : public Node
{
public:
	string id;
	IdNode(string _id): id(_id) {}
	~IdNode(){}
	string translate(int notation){
		string result;
		switch (notation) {
			case MATHML:
				result = " <mi> " + id + " </mi> ";
				break;
			case OPENMATH:
				result = id;
				break;
			case TEX:
				result = id;
				break;
		}
		return result;
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
	string translate(int notation){
		if (operation == "frac"){
			string result;
			switch (notation) {
				case MATHML:
					result = " <mfrac> <mrow>" + left->translate(notation) + "</mrow> <mrow>" +
								   right->translate(notation) + "</mrow> </mfrac> ";
					break;
				case OPENMATH:
					result = "";
					break;
				case TEX:
					result = "\\frac {"+left->translate(notation)+"}{"+right->translate(notation)+"}";
					break;
			}
			return result;
		}
		else
			return left->translate(notation) + " " +createMap()[operation][notation] + " " + right->translate(notation);
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
	string translate(int notation){
		string s;
		for (int i=0; i<nodes.size(); i++){
			 s += nodes[i]->translate(notation);
		}
		return s;
	}
};

#endif // NODE_H_INCLUDED
