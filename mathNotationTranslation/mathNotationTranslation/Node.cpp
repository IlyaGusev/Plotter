#include "Node.h"

string Node::translate(int notation) const {
	return string();
}

map<string, array<string, 3>> Node::createMap()
{
	map<string, array<string, 3>> m;
	m["+"] = { " <mo> + </mo> ", "<OMS cd=\"arith1\" name=\"plus\"/>", " + " };
	m["-"] = { " <mo> - </mo> ", "<OMS cd=\"arith1\" name=\"minus\"/>", " - " };
	m["*"] = { " <mo> * </mo> ", "<OMS cd=\"arith1\" name=\"times\"/>", " * " };
	m["/"] = { " <mo> / </mo> ", "<OMS cd=\"arith1\" name=\"divide\"/>", " / " };
	m["="] = { " <mo> = </mo> ", "<OMS cd=\"realtion1\" name=\"eq\"/>", " = " };
	return m;
}

void Node::setFence(string _lfence, string _rfence) {
	lfence = _lfence;
	rfence = _rfence;
}
string Node::addFence(int notation, string _s) const {
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

NumNode::NumNode(float _number): number(_number) {}

NumNode::~NumNode() {}

string NumNode::translate(int notation) const {
	string result;
	string num = string(to_string(number));
	switch (notation) {
		case MATHML:
			result = " <mn> " + num + " </mn> ";
			break;
		case OPENMATH:
			result = " <OMI> " + num + " </OMI> ";
			break;
		case TEX:
			result = num;
			break;
	}
	return addFence(notation, result);
}

IdNode::IdNode(string _id): id(_id) {}

IdNode::~IdNode() {}

string IdNode::translate(int notation) const {
	string result;
	switch (notation) {
		case MATHML:
			result = " <mi> " + id + " </mi> ";
			break;
		case OPENMATH:
			result = "<OMV name=\"" + id + "\">"; 
			break;
		case TEX:
			result = id;
			break;
	}
	return addFence(notation, result);
}

BinOpNode::BinOpNode(Node* _left, Node* _right, string _operation):
	operation(_operation), left(_left), right(_right) {}

BinOpNode::~BinOpNode() {
	delete left;
	delete right;
}

string BinOpNode::translate(int notation) const {
	if (operation == "frac" || operation == "sup") {
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


CompositeNode::CompositeNode(string _lfence, string _rfence){
	setFence(_lfence, _rfence);
}

CompositeNode::CompositeNode(Node* _node, string _lfence = "", string _rfence = "") {
	setFence(_lfence, _rfence);
	nodes.push_back(_node);
}

CompositeNode::CompositeNode(Node* _node) {
	setFence("", "");
	nodes.push_back(_node);
}

CompositeNode::~CompositeNode() {
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
}

void CompositeNode::add(Node* _node) {
	nodes.push_back(_node);
}

string CompositeNode::translate(int notation) const {
	string s;
	for (int i = 0; i < nodes.size(); i++) {
		s += nodes[i]->translate(notation) + " ";
	}
	s = addFence(notation, s);
	if (notation == MATHML && Node::lfence != "(")
		s = "<mrow>" + s + "</mrow>";
	if (notation == OPENMATH && Node::lfence != "(")
		s = "<OMA>" + s + "</OMA>"
	return s;

}
