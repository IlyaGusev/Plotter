//#pragma once
#ifndef _TacCn_H
#define _TacCn_H
#include "CTag.h"

//класс для работы с тегом <cn>
class CTagCn : public CTagAtomic
{
	void nodeIsInteger( const CNode& node ) const;
	void nodeIsReal ( const CNode& node ) const;
	void nodeIsReal ( const CNode& node, CTreeNode& tree_node ) const;
public:
	CTagCn();
	virtual void operator ()( const CNode& node ) const;
	virtual void operator ()(const CNode& node, CTreeNode& tree_node);
};

#endif
