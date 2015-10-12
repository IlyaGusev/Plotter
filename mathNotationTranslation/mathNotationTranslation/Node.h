#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include <string>
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
	virtual string translate(int notation){
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
	virtual void setFence(string _lfence, string _rfence){
		lfence = _lfence;
		rfence = _rfence;
	}
	virtual string addFence(int notation, string _s){
		string s = _s;
		switch (notation) {
			case MATHML:
				if (lfence == "(")
					s = "<mfenced>" + s + "</mfenced>";
				break;
			case OPENMATH:
				s = "";
				break;
			case TEX:
				s = lfence + s + rfence;
				break;
		}
		return s;
	}
};

class NumNode : public Node{
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
		return addFence(notation, result);
	}
};

class IdNode : public Node{
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
		return addFence(notation, result);
	}
};

class BinOpNode : public Node{
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
		if (operation == "frac" || operation == "sup"){
			string result;
			switch (notation) {
				case MATHML:
					if (operation == "frac")
						result = " <mfrac>" + left->translate(notation) +
								   right->translate(notation) + "</mfrac> ";
				  if (operation == "sup")
				  	result = " <msup>" + left->translate(notation) +
								   right->translate(notation) + "</msup> ";
					break;
				case OPENMATH:
					result = "";
					break;
				case TEX:
					if (operation == "frac")
						result = "\\frac " + left->translate(notation) + right->translate(notation);
					if (operation == "sup")
						result = left->translate(notation) + " ^ " + right->translate(notation);
					break;
			}
			return addFence(notation, result);
		}
		else
			return addFence(notation, left->translate(notation) + createMap()[operation][notation] + right->translate(notation));
	}
};

class CompositeNode : public Node{
public:
	vector<Node*> nodes;

	CompositeNode(string _lfence = "", string _rfence = ""){
		setFence(_lfence, _rfence);
	}
	CompositeNode(Node* _node, string _lfence = "", string _rfence = ""){
		setFence(_lfence, _rfence);
		nodes.push_back(_node);
	}
	~CompositeNode(){
		for (int i=0; i<nodes.size(); i++){
			delete nodes[i];
		}
	}
	void add(Node* _node){
		nodes.push_back(_node);
	}
	string translate(int notation){
		string s;
		for (int i=0; i<nodes.size(); i++){
			 s += nodes[i]->translate(notation) + " ";
		}
		s = addFence(notation, s);
		if (notation == MATHML && Node::lfence != "(")
			s = "<mrow>" + s + "</mrow>";
		return s;
	}
};

#endif // NODE_H_INCLUDED
