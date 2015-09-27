#pragma once
#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include <set>
#include "../pugixml/pugixml.hpp"
#include "CTagContainer.h"

#define VOID (~0)
#define NUMBER			0x00000001
#define BOOL			0x00000004
#define FUNCTION		0x00000018
#define CALCULATEBLE	0x00000010
#define VARIABLE		0x00000011
#define LIMIT_LO		0x00000012
#define LIMIT_UP		0x00000014
#define LIMITABLE		0x00000030

using namespace std;
using namespace pugi;

typedef unsigned int CType;
typedef pugi::xml_node CNode;

class CTag
{
public:
	CTag();
	virtual void operator ()(const CNode& node) const = 0;
	virtual const CNode checkSignature(const CNode& Node)const = 0;
	virtual ~CTag();
	const string name;
	CType type;
protected:
	static void enterToAllChilds(const CNode& node);
	void throwException(string text, int position) const;
	void hasNoAttributes(const CNode& node) const;
	void checkAttributes(const CNode& node,const set<string>& attributes) const;
	void hasNoText(const CNode& node) const;
	void hasNoChilds(const CNode& node) const;
	void hasNChilds(const CNode& node,int N)const;
	const string& getName() const;
	CType getType() const;
};

class CTagAtamar : public CTag //tag doesn't requier any siblings
{
	virtual const CNode  checkSignature(const CNode& Node) const;
};

class CTagApply : public CTagAtamar
{
public:
	CTagApply();
	virtual void operator ()(const CNode& node)const;
};


class CTagBinaryNumFunction : public CTag
{
public:
	CTagBinaryNumFunction();
	virtual void operator ()(const CNode& node)const;
	virtual const CNode checkSignature(const CNode& node)const;
};

class CTagCn : public CTagAtamar
{
	void nodeIsInteger(const CNode& node)const;
	void nodeIsReal(const CNode& node)const;
public:
	CTagCn();
	virtual void operator ()(const CNode& node)const;
};