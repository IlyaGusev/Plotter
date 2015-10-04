#pragma once
//#ifndef _TAG_QUALIFIER_H
//#define _TAG_QUALIFIER_H

#include "Tag.h"

class CTagQualifiers : public CTagAtamar
{
public:
    CTagQualifiers(int _type);
    virtual void operator()(const CNode& node) const;
};

//#endif