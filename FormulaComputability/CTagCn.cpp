#ifndef _TacCn_CPP
#define _TacCn_CPP

#include "CTagCn.h"

CTagCn::CTagCn()
{
	type = NUMBER;
};


//check node value is int
void CTagCn::nodeIsInteger(const CNode& node)const
{
	size_t pos = 0;
	string num = deleteSpaces(node.text().as_string());
	double d = stoi(num, &pos);
	if (pos != num.length() || !num.length())
		throwException(node, node.offset_debug(), INCORRECT_VALUE);
}
//check node value is float
void CTagCn::nodeIsReal(const CNode& node)const
{
	size_t pos = 0;
	string num = deleteSpaces(node.text().as_string());
	double d = stod(num, &pos);
	if (pos != num.length() || !num.length())
		throwException(node, node.offset_debug(), INCORRECT_VALUE);
}

void CTagCn::nodeIsReal(const CNode& node, CTreeNode& tree_node)const
{
	nodeIsReal(node);
	size_t pos = 0;
	string num = deleteSpaces(node.text().as_string());
	double d = stod(num, &pos);
	tree_node.value = d;
}

void CTagCn::operator ()(const CNode& node)const
{
	checkAttributes(node, { "type" });
	string attr = node.attribute("type").as_string();
	CNode child = node.first_child();
	if (attr == "real" || attr == "")
	{
		hasNChilds(node, 1);
		nodeIsReal(child);
		return;
	}
	if (attr == "integer")
	{
		hasNChilds(node, 1);
		nodeIsInteger(child);
		return;
	};
	// syntax "real <sep/> real"
	if ((attr == "complex-cartesian") || (attr == "complex-polar"))
	{
		hasNChilds(node, 3);
		nodeIsReal(child);
		nodeIsReal(child.next_sibling().next_sibling());
		if (string(child.next_sibling().name()) != string("sep"))
			throwException(node, node.offset_debug(), INCORRECT_VALUE);
		return;
	};

	// syntax "int <sep/> imt"
	if (attr == "rational")
	{
		hasNChilds(node, 3);
		nodeIsInteger(child);
		nodeIsInteger(child.next_sibling().next_sibling());
		if (string(child.next_sibling().name()) != string("sep"))
			throwException(node, node.offset_debug(), INCORRECT_VALUE);
		return;
	};

	if (attr == "constant")
	{
		return;
	};

	throwException(node, node.offset_debug(), UNKNOWN_ATTRIBUTE);
}

void CTagCn::operator ()(const CNode& node, CTreeNode& tree_node)
{
	(*this)(node);
	checkAttributes(node, { "type" });
	string attr = node.attribute("type").as_string();
	CNode child = node.first_child();
	if (attr == "real" || attr == "")
	{
		hasNChilds(node, 1);
		nodeIsReal(child, tree_node.Step("cn"));
		return;
	}
}

#endif
