#include "Tag.h"



CTag::CTag()
{
}

CTag::~CTag()
{
}

void CTag::enterToAllChilds(const CNode& node)
{
	auto child = node.first_child();
	while (! child.empty())
	{
		(*getTag(child.name()))(child);
		child = child.next_sibling();
	} 
};

void CTag::hasNoAttributes(const CNode& node) const
{
	if (node.attributes().begin() != node.attributes().end())
		throwException("this tag can't have any attribute",node.offset_debug());
};

void CTag::checkAttributes(const CNode& node, const set<string>& attributes) const
{
	string errorMassage;
	for (auto i = node.attributes_begin(); i != node.attributes_end(); ++i)
		if (attributes.find(i->name()) == attributes.end())
			errorMassage += " " + string(i->name());
	if (errorMassage != "")
		throwException("unknown attribute(s) " + errorMassage, node.offset_debug());
};

void CTag::hasNoText(const CNode& node) const
{
	if (node.text().as_string() != "")
		throwException("this tag can't have any value", node.offset_debug());
};

void CTag::hasNoChilds(const CNode& node) const
{
	if (node.children().begin() != node.children().end())
		throw exception("this tag can't have any value");
};

void CTag::hasNChilds(const CNode& node, int N)const
{
	int i = 0;
	CNode child = node.first_child();
	while (!child.empty())
	{
		++i;
		child = child.next_sibling();
	};
	if (i != N)
		throwException("too much childs in tag", node.offset_debug());
};

void CTag::throwException(string text, int position) const
{
	throw exception(("exception: " + text + " in tag").c_str());
};

CNode  CTagAtamar::checkSignature(const CNode& Node)const
{
	return Node.next_sibling();
}

CTagApply::CTagApply()
{
	CTag::type = NUMBER;
};

void CTagApply::operator ()(const CNode& node)const
{
	hasNoAttributes(node);
	hasNoText(node);

	auto child = node.first_child();
	const CTag* func = getTag(child.name());
	if (!((func->type & CALCULATEBLE) && (func->type & NUMBER)))
		throwException("wrong 1-st argument",node.offset_debug());
	child = func->checkSignature(child);
	if (!( child.empty() ))
		throwException("wrong last argument", node.offset_debug());
	enterToAllChilds(node);
};

CTagBinaryNumFunction::CTagBinaryNumFunction()
{
	CTag::type = NUMBER | FUNCTION | CALCULATEBLE;
};

void CTagBinaryNumFunction::operator ()(const CNode& node)const
{
	hasNoAttributes(node);
	hasNoChilds(node);
	hasNoText(node);
}
CNode  CTagBinaryNumFunction::checkSignature(const CNode& node)const
{
	auto arg = node.next_sibling();
	if (arg.empty())
		throw exception("incorrect argument");
	CType argType = getTag(arg.name())->type;
	if ((!(argType & NUMBER)) || (argType & (~NUMBER)))
		throw exception("incorrect argument");
	arg = arg.next_sibling();
	if (arg.empty())
		throw exception("incorrect argument");
	if ((!(argType & NUMBER)) || (argType & (~NUMBER)))
		throw exception("incorrect argument");
	return arg.next_sibling();
};

CTagCn::CTagCn()
{
	CTag::type = NUMBER;
};


void CTagCn::nodeIsInteger(const CNode& node)const 
{
	try
	{
		int i = stoi(node.text().as_string());
	}
	catch (...)
	{
		throwException("incorrect integer", node.offset_debug());
	};
};

void CTagCn::nodeIsReal(const CNode& node)const
{
	try
	{
		double d = stod(node.text().as_string());
	}
	catch (...)
	{
		throwException("incorrect integer", node.offset_debug());
	};
};

void CTagCn::operator ()(const CNode& node)const
{
	checkAttributes(node, { "type" });
	string attr = node.attribute("type").as_string();
	CNode child = node.first_child();
	if (attr == "real" || attr == "")
	{
		hasNChilds(node, 1);
		nodeIsReal(node.first_child());
		return;
	}
	if (attr == "integer")
	{
		hasNChilds(node, 1);
		nodeIsInteger(node.first_child());
		return;
	};
	if (attr == "rational")
	{
		hasNChilds(node, 2);
		nodeIsInteger(node.first_child());
		nodeIsInteger(node.first_child().next_sibling());
		return;
	};
	if (attr == "complex-cartesian")
	{
		return;
	};
	if (attr == "complex-polar")
	{
		return;
	};
	if (attr == "constant")
	{
		return;
	};
	throwException("unexceted attribute name", node.offset_debug());
};