#ifndef _TAGDECLARE_CPP
#define _TAGDECLARE_CPP
#include "TagDeclare.h"
#include "TagCi.h"

void CTagDeclare::operator ()(const CNode& node) const
{
	checkAttributes(node, { "type","nargs" });
	if (strcmp(node.first_child().name(),"ci") != 0)//first child must be ci
		throwException(node.name(), node.offset_debug(), UNEXPECTED_CHILD);
	if (strcmp(node.first_child().value(), "") != 0)//ci must contain name of identifier
		throwException(node.first_child().name(), node.first_child().offset_debug(), UNEXPECTED_CHILD);
	if (node.attribute("type").as_string() != string(""))
	{
		if (node.attribute("type").as_string() == string("fn"))
			addFunction(node);
		else
			addIdentifier(node);
	}
	else
	{
		addIdentifier(node);
	}
}

void CTagDeclare::addTempIdentifiers(int count, const string& nodeName, int position) const
{
	if (count <= 0)
		return;
	//default arguments x,y,z,t
	CTagCi::AddIdentifier("x", NUMBER, nodeName, position);
	if (count <= 1)
		return;
	CTagCi::AddIdentifier("y", NUMBER, nodeName, position);
	if (count <= 2)
		return;
	CTagCi::AddIdentifier("z", NUMBER, nodeName, position);
	if (count <= 3)
		return;
	CTagCi::AddIdentifier("t", NUMBER, nodeName, position);
}

void CTagDeclare::ereaseTempIdentifiers(int count) const
{
	if (count <= 0)
		return;
	CTagCi::deleteIdentifier("x");
	if (count <= 1)
		return;
	CTagCi::deleteIdentifier("y");
	if (count <= 2)
		return;
	CTagCi::deleteIdentifier("z");
	if (count <= 3)
		return;
	CTagCi::deleteIdentifier("t");
}

void CTagDeclare::addFunction(const CNode& node) const
{
	//nargs must be daclated
	if (node.attribute("nargs").as_string() == string(""))
		throwException(node.name(), node.offset_debug(), INVALID_ATTRIBUTE_ARGUMENT);

	int CountTempIdentifiers = node.attribute("nargs").as_int();
	string functionName = deleteSpaces(node.first_child().first_child().text().as_string());
	if (functionName.empty())
		throwException(node.first_child().name(), node.first_child().offset_debug(), UNEXPECTED_VALUE);
	switch (CountTempIdentifiers) {
	case 1:
		CTagContainer::addTag(functionName, new CTagNArgFunction<NUMBER, NUMBER, 1>(), node.offset_debug());
		break;
	case 2:
		CTagContainer::addTag(functionName, new CTagNArgFunction<NUMBER, NUMBER, 2>(), node.offset_debug());
		break;
	case 3:
		CTagContainer::addTag(functionName, new CTagNArgFunction<NUMBER, NUMBER, 3>(), node.offset_debug());
		break;
	case 4:
		CTagContainer::addTag(functionName, new CTagNArgFunction<NUMBER, NUMBER, 4>(), node.offset_debug());
		break;
	};
	//add function arguments as identifiers
	addTempIdentifiers(CountTempIdentifiers, node.name(), node.offset_debug());
	if (!(CTagContainer::getTag(node.first_child().next_sibling().name())->type & NUMBER))
		throwException(node.name(), node.offset_debug(), UNEXPECTED_CHILD);
	//check coputability the body of function
	(*CTagContainer::getTag(node.first_child().next_sibling().name()))(node.first_child().next_sibling());
	//delete function args
	ereaseTempIdentifiers(CountTempIdentifiers);
}

void CTagDeclare::addIdentifier(const CNode& node) const
{
	CTagCi::AddIdentifier(node.first_child(), NUMBER);
	if (!(CTagContainer::getTag(node.first_child().next_sibling().name())->type & NUMBER))
		throwException(node.name(), node.offset_debug(), UNEXPECTED_CHILD);
	(*CTagContainer::getTag(node.first_child().next_sibling().name()))(node.first_child().next_sibling());
}

#endif