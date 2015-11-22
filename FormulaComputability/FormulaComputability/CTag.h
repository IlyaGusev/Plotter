#pragma once
#ifndef _TAG_H
#define _TAG_H
#include "common.h"
#include "CTagContainer.h"
#include "CTree.h"

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
	virtual void operator ()( const CNode& node ) const = 0; //check coputability all internal tags
	virtual const CNode checkSignature( const CNode& Node ) const = 0; //check that siblings are
																	 //possible for this tag
  virtual void operator ()( const CNode& node, CTreeNode& tree_node  ) { return (*this)(node); }
  virtual const CNode checkTree( const CNode& Node, CTreeNode& tree_node ) const { return this->checkSignature(Node); }
	virtual ~CTag();
	CType getType() const;
	const string name;
	CType type;
protected:
	static string deleteSpaces( const string& s );//delete white spaces in the begin and end of the string
	static void enterToAllChilds( const CNode& node );//call operator() of all child tags
	static void enterToAllChilds( const CNode& node, CTreeNode& tree_node);
	static void throwException( const CNode& errorTag, int position, ErrorType errType );
	static void hasNoChilds( const CNode& node );
	void hasNoAttributes( const CNode& node ) const;
	void checkAttributes( const CNode& node, const set<string>& attributes ) const;
	void hasNoText( const CNode& node ) const;
	void hasNChilds( const CNode& node,int N ) const;
	const CNode checkArgumentType( const CNode& node, const CNode& parentNode, int requieredType ) const;
	const CNode checkArgumentType( const CNode& node, const CNode& parentNode, int requieredType, CTreeNode& tree_node );
	const string& getName() const;
};

//tag doesn't requier any siblings
class CTagAtomic : public CTag
{
public:
	virtual const CNode checkSignature( const CNode& node ) const;
};

//класс для работы с тегом <bvar>, объявляющим локальную переменную
class CTagBVar : public CTagAtomic
{
public:
	CTagBVar();
	virtual void operator() ( const CNode& node )const;
	virtual void operator() ( const CNode& node, CTreeNode& tree_node );
};

//класс для работы с тегом <condition>
class CTagCondition : public CTagAtomic
{
public:
	CTagCondition();
	virtual void operator()( const CNode& node ) const;
};

//класс для работы с тегами <sum/> и <product/>
class CTagLimitable: public CTag
{
public:
	CTagLimitable( );
	virtual const CNode checkSignature( const CNode& node ) const;
	virtual void operator()( const CNode& node ) const;
};


#endif
