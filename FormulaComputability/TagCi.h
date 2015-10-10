#pragma once
#ifndef _TAGCI_H
#define _TAGCI_H
#include "Tag.h"


class CTagCi : public CTagAtamar
{
	static map<string, CType> identifiers;
	static map<string, CType> buildIdentifiers();
public:
	CTagCi();
	virtual void operator()(const CNode& node) const;
	static void AddIdentifier(const CNode& node, CType Type);//if this tag define new identifier call this function not operator()
	static void AddIdentifier(const string& name, CType Type, const string& nameNode, int position);
	static void deleteIdentifier(const string name);
};

#endif