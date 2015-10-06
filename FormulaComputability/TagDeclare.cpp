#ifndef _TAGDECLARE_CPP
#define _TAGDECLARE_CPP
#include "TagDeclare.h"
#include "TagCi.h"

void CTagDeclare::operator ()(const CNode& node) const
{
	checkAttributes(node, { "type","nargs" });
	if (node.first_child().name() != "ci")//first child must be ci
		throwException(node.name(), node.offset_debug(), UNEXPECTED_CHILD);
	if (node.first_child().value() == "")//ci must contain name of identifier
		throwException(node.first_child().name(), node.first_child().offset_debug(), UNEXPECTED_CHILD);
	if (node.attribute("type").as_string() != "")
	{
		if (node.attribute("type").as_string() == "fn")
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
	if (node.attribute("nargs").as_string() == "")
		throwException(node.name(), node.offset_debug(), INVALID_ATTRIBUTE_ARGUMENT);

	int CountTempIdentifiers = node.attribute("nargs").as_int();
	switch (CountTempIdentifiers) {
	case 1:
		CTagContainer::addTag(node.first_child().first_child().text().as_string(), new CTagNArgFunction<NUMBER, NUMBER, 1>(), node.offset_debug());
		break;
	case 2:
		CTagContainer::addTag(node.first_child().first_child().text().as_string(), new CTagNArgFunction<NUMBER, NUMBER, 2>(), node.offset_debug());
		break;
	case 3:
		CTagContainer::addTag(node.first_child().first_child().text().as_string(), new CTagNArgFunction<NUMBER, NUMBER, 3>(), node.offset_debug());
		break;
	case 4:
		CTagContainer::addTag(node.first_child().first_child().text().as_string(), new CTagNArgFunction<NUMBER, NUMBER, 4>(), node.offset_debug());
		break;
	};
	addTempIdentifiers(CountTempIdentifiers, node.name(), node.offset_debug());
	if (CTagContainer::getTag(node.first_child().next_sibling().name())->type & NUMBER)
		throwException(node.name(), node.offset_debug(), UNEXPECTED_CHILD);
	//check coputability the body of function
	(*CTagContainer::getTag(node.first_child().next_sibling().name()))(node.first_child().next_sibling());
	ereaseTempIdentifiers(CountTempIdentifiers);
}

void CTagDeclare::addIdentifier(const CNode& node) const
{
	CTagCi::AddIdentifier(node.first_child(), NUMBER);
	if (CTagContainer::getTag(node.first_child().next_sibling().name())->type & NUMBER)
		throwException(node.name(), node.offset_debug(), UNEXPECTED_CHILD);
	(*CTagContainer::getTag(node.first_child().next_sibling().name()))(node.first_child().next_sibling());
}

#endif