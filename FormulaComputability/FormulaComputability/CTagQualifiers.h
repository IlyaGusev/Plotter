#pragma once
#ifndef _TAG_QUALIFIER_H
#define _TAG_QUALIFIER_H

#include "CTag.h"

//класс для работы с тегами degree, lowlimit, uplimit
class CTagQualifiers : public CTagAtomic
{
public:
    CTagQualifiers( int _type ); //_type определяет конкретный тип тега
    virtual void operator()( const CNode& node ) const;
    virtual void operator()( const CNode& node, CTreeNode& tree_node );
};

#endif
