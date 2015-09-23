#pragma once
#include <string>
#include <vector>
#include <unordered_map>
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
typedef CAttributes;
typedef CNode;

class CTag
{
public:
	CTag();
	static const CType type;

	virtual void operator ()(CNode& Node) = 0;

	virtual CNode checkSignature(CNode Node)const = 0;

	virtual ~CTag();
};

class CTagAtamar : public CTag
{
	virtual CNode  checkSignature(CNode Node)const
	{
		return Node.next_sibling();
	}
};

class CTagApply : public CTagAtamar
{
	static const CType type = NUMBER;
	virtual void operator ()(CNode& node)
	{

	};
};


class CTagContainear
{
public:
	static const unordered_map<string, CTag*> Tags;
private:
	static const unordered_map<string, CTag*> CTagCuotainearBuild()
	{ 
		pair< string, CTag* > TagList[] = {

			/***********write here tags*********************/

			pair< string, CTag* >("apply", (CTag*) new CTagApply())

			/**********************************************/

		};
		return unordered_map<string, CTag*>(TagList, TagList + sizeof(TagList) / sizeof(pair< string, CTag* >));
	};
};

const unordered_map<string, CTag*> CTagContainear::Tags = CTagContainear::CTagCuotainearBuild();