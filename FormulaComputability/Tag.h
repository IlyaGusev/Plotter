#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../pugixml/pugixml.hpp"
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

typedef unsigned int CType;
//typedef CAttributes;
typedef pugi::xml_node CNode;

class CTag
{
public:
	CTag();

	virtual void operator ()(CNode& node) = 0;

	virtual CNode checkSignature(CNode& node) const = 0;

	virtual ~CTag();
};


class CTagAtamar : public CTag
{
public:
	virtual void operator ()(CNode& node) {}
	virtual CNode  checkSignature(CNode& node) const
	{
		return node.next_sibling();
	}
private:
	static const CType type;
};

class CTagApply : public CTagAtamar
{
public:
	virtual void operator ()(CNode& node)
	{

	};
	virtual CNode checkSignature(CNode& node) const {}
private:
	static const CType type = NUMBER;
};

