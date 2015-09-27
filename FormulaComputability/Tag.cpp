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
		( *CTagContainer::getTag( child.name() ) )( child );
		child = child.next_sibling();
	} 
}

void CTag::hasNoAttributes(const CNode& node) const
{
	if (node.attributes().begin() != node.attributes().end()) {
        throwException("this tag can't have any attribute", node.offset_debug());
    }
}

void CTag::checkAttributes(const CNode& node, const set<string>& attributes) const
{
	string errorMessage;
	for ( auto it = node.attributes_begin(); it != node.attributes_end(); ++it ) {
		if ( attributes.find(it->name()) == attributes.end() ) {
			errorMessage += string( it->name() ) + ' ';
        }
    }
	if ( !errorMessage.empty() ) {
        throwException("unknown attribute(s): " + errorMessage, node.offset_debug());
    }
}

void CTag::hasNoText(const CNode& node) const
{
	if (node.text().as_string() != "") {
		throwException("this tag can't have any value", node.offset_debug());
    }
}

void CTag::hasNoChilds(const CNode& node) const
{
	if (node.children().begin() != node.children().end())
		throwException("this tag can't have any value", node.offset_debug());
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
	throw invalid_argument("exception: " + text + " in tag");
};

const string& CTag::getName() const 
{
    return name;
}
CType CTag::getType() const 
{
    return type;
}

const CNode CTagAtamar::checkSignature(const CNode& Node) const
{
	return Node.next_sibling();
}

CTagApply::CTagApply()  
{
    type = NUMBER;
}

void CTagApply::operator ()(const CNode& node)const
{
	hasNoAttributes(node);
	hasNoText(node);

	auto child = node.first_child();
	const CTag* func = CTagContainer::getTag(child.name());
	if (!((func->type & CALCULATEBLE) && (func->type & NUMBER)))
		throwException("wrong 1-st argument",node.offset_debug());
	child = func->checkSignature(child);
	if (!( child.empty() ))
		throwException("wrong last argument", node.offset_debug());
	enterToAllChilds(node);
}

CTagBinaryNumFunction::CTagBinaryNumFunction() 
{
    type = NUMBER | FUNCTION | CALCULATEBLE;
} 

void CTagBinaryNumFunction::operator ()(const CNode& node)const
{
	hasNoAttributes(node);
	hasNoChilds(node);
	hasNoText(node);
}
const CNode  CTagBinaryNumFunction::checkSignature(const CNode& node)const
{
	auto arg = node.next_sibling();
	if (arg.empty())
		throwException("incorrect argument", node.offset_debug());
	CType argType = CTagContainer::getTag(arg.name())->type;
	if ((!(argType & NUMBER)) || (argType & (~NUMBER)))
		throwException("incorrect argument", node.offset_debug());
	arg = arg.next_sibling();
	if (arg.empty())
		throwException("incorrect argument", node.offset_debug());
	if ((!(argType & NUMBER)) || (argType & (~NUMBER)))
		throwException("incorrect argument", node.offset_debug());
	return arg.next_sibling();
}

CTagCn::CTagCn()
{
     type = NUMBER;
}


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
}

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
}

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
}