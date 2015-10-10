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
	string ident(deleteSpaces(node.text().as_string()));
	if (ident.empty()) {
		throwException(node.name(), node.offset_debug(), INCORRECT_VALUE);
	}
	if(identifiers.find(ident) == identifiers.end())
		throwException(node.name(), node.offset_debug(), UNKNOWN_IDENTIFIER);
}

void CTagCi::AddIdentifier(const CNode& node, CType Type)
{
	string ident(node.text().as_string());
	if (ident.empty()) {
		throwException(node.name(), node.offset_debug(), INCORRECT_VALUE);
	};
	AddIdentifier(ident, Type, node.name(), node.offset_debug());
}

void CTagCi::AddIdentifier(const string& name, CType Type, const string& nameNode, int position)
{
	string _name = deleteSpaces(name);
	if (identifiers.find(_name) != identifiers.end())
		throwException(nameNode, position, IDENTIFIER_ALREADY_EXIST);
	identifiers[_name] = Type;
}

void CTagCi::deleteIdentifier(const string name)
{
	identifiers.erase(deleteSpaces(name));
}

#endif