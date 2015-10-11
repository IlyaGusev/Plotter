#pragma once
#ifndef _TAG_H
#define _TAG_H
#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include <set>
#include "../pugixml/pugixml.hpp"
#include "CTagContainer.h"

//битовые маски, определяющие типы тегов
#define VOID (~0)
#define NUMBER			0x00000001
#define BOOL			0x00000002
#define VARIABLE		0x00000004
#define FUNCTION		0x00000008
#define CALCULATEBLE	0x00000010
#define LIMIT_LO		0x00000020
#define LIMIT_UP		0x00000040
#define LIMITABLE		0x00000080 //sum, product
#define BOUND 			0x00000100 //bvar
#define QUALIFIER		0x00000200 //lowlimit, uplimit, bvar, degree
#define DEGREE			0x00000400
#define CONDITION		0x00000800
#define SPECIAL			0x00001000

using namespace std;
using namespace pugi;

typedef unsigned int CType;
typedef pugi::xml_node CNode;

enum ErrorType 
{
	UNKNOWN_IDENTIFIER,
	INVALID_ARGUMENT, NO_ARGUMENT, MISSED_ARGUMENT, UNKNOWN_ATTRIBUTE, INCORRECT_VALUE, 
	UNEXPECTED_ATTRIBUTE, UNEXPECTED_VALUE, UNEXPECTED_CHILD, INVALID_ATTRIBUTE_ARGUMENT,
	ATTRIBUTE_REQUIRED, IDENTIFIER_ALREADY_EXIST,
};

//base abstract class
class CTag
{
public:
	CTag();
	virtual void operator ()(const CNode& node) const = 0;//check coputability all internal tags
	virtual const CNode checkSignature(const CNode& Node)const = 0;//check that siblings are 
																	//possible for this tag
	virtual ~CTag();
	CType getType() const;
	const string name;
	CType type;
protected:
	static string deleteSpaces(const string& s);//delete white spaces in the begin and end of the string
	static void enterToAllChilds(const CNode& node);//call operator() of all child tags
	static void throwException(const CNode& errorTag, int position, ErrorType errType);
	static void hasNoChilds(const CNode& node);
	void hasNoAttributes(const CNode& node) const;
	void checkAttributes(const CNode& node,const set<string>& attributes) const;
	void hasNoText(const CNode& node) const;
	void hasNChilds(const CNode& node,int N)const;
	const CNode checkArgumentType(const CNode& node, const CNode& parentNode, int requieredType) const;
	const string& getName() const;
};

//tag doesn't requier any siblings
class CTagAtamar : public CTag 
{
public:
	virtual const CNode  checkSignature(const CNode& node) const;
};

//класс для работы с тегом <bvar>, объявляющим локальную переменную
class CTagBVar : public CTagAtamar 
{
public:
	CTagBVar();
	virtual void operator ()(const CNode& node)const;
};

//класс для работы с тегом <condition>
class CTagCondition : public CTagAtamar
{
public:
	CTagCondition();
	virtual void operator()(const CNode& node) const;
};

//класс для работы с тегами <sum/> и <product/>
class CTagLimitable: public CTag
{
public:
	CTagLimitable();
	virtual const CNode checkSignature(const CNode& node) const;
	virtual void operator()(const CNode& node) const;
};

 
#endif