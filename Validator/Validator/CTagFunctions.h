#pragma once
#ifndef _TAGFURMULA_H
#define _TAGFURMULA_H

#include "CTag.h"

template< CType TArg,CType TRes>
class CTagFunction : public CTag
{
public:
	CNode checkArgument(const CNode node)const;//check that node is correct argument and return node.next_sibling()
	virtual void operator ()(const CNode& node)const;
	CTagFunction() {
		type = TRes | FUNCTION | CALCULATEBLE;
	};
};

template< CType TArg,CType TRes>
CNode CTagFunction< TArg, TRes>::checkArgument(const CNode node)const
{
	CType argType = CTagContainer::getTag(node.name()).getType();
	if (!(argType & TArg)) {
		throwException(node, node.offset_debug(), INVALID_ARGUMENT);
	}
	return node.next_sibling();
};

template< CType TArg,CType TRes>
void CTagFunction< TArg, TRes >::operator ()(const CNode& node)const
{
	hasNoAttributes(node);
	hasNoChilds(node);
	hasNoText(node);
};

//function with various arguments
template< CType TArg,CType TRes>
class CTagVarArgFunction : public CTagFunction< TArg, TRes>
{
public:
	virtual const CNode checkSignature(const CNode& node)const;
	virtual const CNode checkTree(const CNode& node, CTreeNode& tree_node)const;
};

//function with CountArg arguments
template< CType TArg, CType TRes, int CountArg>
class CTagNArgFunction : public CTagFunction< TArg, TRes>
{
public:
	virtual const CNode checkSignature(const CNode& node)const;
	virtual const CNode checkTree(const CNode& node, CTreeNode& tree_node)const;
};

template< CType TArg,CType TRes>
const CNode CTagVarArgFunction< TArg, TRes>::checkSignature(const CNode& node)const
{
	auto arg = node.next_sibling();
	if ( arg.empty() ) {
		CTag::throwException(node, node.offset_debug(), NO_ARGUMENT);
	}

	arg = CTagFunction< TArg, TRes>::checkArgument(arg);
	while (!arg.empty())
	{
		arg = CTagFunction< TArg, TRes>::checkArgument(arg);
	}
	return arg;
};

template< CType TArg,CType TRes>
const CNode CTagVarArgFunction< TArg, TRes>::checkTree(const CNode& node, CTreeNode& tree_node)const
{
	return (*this).checkSignature(node);
};

template< CType TArg, CType TRes, int CountArg>
const CNode CTagNArgFunction< TArg, TRes, CountArg>::checkSignature(const CNode& node)const
{
		auto arg = node.next_sibling();
		for (int i = 0; i < CountArg; ++i)
		{
			arg = CTagFunction< TArg, TRes>::checkArgument(arg);
		}
	return arg;
};

template< CType TArg, CType TRes, int CountArg>
const CNode CTagNArgFunction< TArg, TRes, CountArg>::checkTree(const CNode& node, CTreeNode& tree_node)const
{
	return (*this).checkSignature(node);
};

#endif
