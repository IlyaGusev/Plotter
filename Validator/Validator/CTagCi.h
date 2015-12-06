#pragma once
#ifndef _TAGCI_H
#define _TAGCI_H
#include "CTag.h"

//класс для работы с тегом <ci>
class CTagCi : public CTagAtomic
{
public:
	CTagCi();
	virtual void operator()(const CNode& node) const;
	virtual void operator()(const CNode& node, CTreeNode& tree_node);
	static void AddIdentifier(const CNode& node, CType Type);//if this tag define new identifier
                                                            // call this function not operator()
	static void AddIdentifier(const string& name, CType Type, const CNode& node, int position);
	static void deleteIdentifier(const string& name);
private:
    static map<string, CType> identifiers;
    static map<string, CType> buildIdentifiers();
};

#endif
