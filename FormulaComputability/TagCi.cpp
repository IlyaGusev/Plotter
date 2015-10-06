#ifndef _TAGCI_CPP
#define _TAGCI_CPP
#include "TagCi.h"

map<string, CType> CTagCi::buildIdentifiers()
{
	return  map<string, CType>();
}

map<string, CType> CTagCi::identifiers = CTagCi::buildIdentifiers();

CTagCi::CTagCi()
{
	type = NUMBER;
}

void CTagCi::operator()(const CNode& node) const
{
	hasNoChilds(node);
	string ident(node.value());
	if (ident.empty()) {
		throwException(node.name(), node.offset_debug(), INCORRECT_VALUE);
	}
	if(identifiers.find(ident) == identifiers.end())
		throwException(node.name(), node.offset_debug(), UNKNOWN_IDENTIFIER);
}

void CTagCi::AddIdentifier(const CNode& node, CType Type)
{
	hasNoChilds(node);
	string ident(node.value());
	if (ident.empty()) {
		throwException(node.name(), node.offset_debug(), INCORRECT_VALUE);
	};
	AddIdentifier(node.value(), Type, node.name(), node.offset_debug());
}

void CTagCi::AddIdentifier(const string& name, CType Type, const string& nameNode, int position)
{
	if (identifiers.find(name) != identifiers.end())
		throwException(nameNode, position, IDENTIFIER_ALREADY_EXIST);
	identifiers[name] = Type;
}

void CTagCi::deleteIdentifier(const string name)
{
	identifiers.erase(name);
}

#endif