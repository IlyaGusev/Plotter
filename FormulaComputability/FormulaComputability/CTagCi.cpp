#ifndef _TAGCI_CPP
#define _TAGCI_CPP
#include "CTagCi.h"

map<string, CType> CTagCi::buildIdentifiers()
{
	return  map<string, CType>();
}

map<string, CType> CTagCi::identifiers = CTagCi::buildIdentifiers();

CTagCi::CTagCi()
{
	type = NUMBER | VARIABLE;
}

void CTagCi::operator()(const CNode& node) const
{
	string ident(deleteSpaces(node.text().as_string()));
	if (ident.empty()) {
		throwException(node, node.offset_debug(), INCORRECT_VALUE);
	}
	if( identifiers.find(ident) == identifiers.end() ) {
		throwException(node, node.offset_debug(), UNKNOWN_IDENTIFIER);
	}
}

void CTagCi::operator()(const CNode& node, CTreeNode& tree_node)
{
	string ident(deleteSpaces(node.text().as_string()));
	tree_node.Step(node.name());
	if (ident.empty()) {
		throwException(node, node.offset_debug(), INCORRECT_VALUE);
	}
	if( identifiers.find(ident) == identifiers.end() ) {
		throwException(node, node.offset_debug(), UNKNOWN_IDENTIFIER);
	}
}

void CTagCi::AddIdentifier(const CNode& node, CType Type)
{
	string ident(node.text().as_string());
	if (ident.empty()) {
		throwException(node, node.offset_debug(), INCORRECT_VALUE);
	}
	AddIdentifier(ident, Type, node, node.offset_debug());
}

void CTagCi::AddIdentifier(const string& name, CType Type, const CNode& node, int position)
{
	string _name = deleteSpaces(name);
	if (identifiers.find(_name) != identifiers.end()) {
		throwException(node, position, IDENTIFIER_ALREADY_EXIST);
	}
	identifiers.emplace(_name, Type);
}

void CTagCi::deleteIdentifier(const string& name)
{
	identifiers.erase(deleteSpaces(name));
}

#endif
