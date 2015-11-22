#pragma once
#ifndef _TAGDECLARE_H
#define _TAGDECLARE_H
#include "CTag.h"
#include "CTagFunctions.h"
#include "CTagContainer.h"


class CTagDeclare : public CTagAtomic
{
public:
	void operator ()( const CNode& node ) const;
private:
    void addTempIdentifiers( int count, const CNode& node, int position ) const;// add functions arguments (x,y,z,t) to identifiers
    void eraseTempIdentifiers( int count ) const;//delete function's arguments
    void addFunction( const CNode& node ) const;//add Function to TagContainer
                                            //(just 0 - 4 arg func x,y,z,t - it's arguments
    void addIdentifier( const CNode& node ) const;
};

#endif
