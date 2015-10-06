#pragma once
#include "Tag.h"
#include "TagFunctions.h"
#ifndef _TAGDECLARE_H
#define _TAGDECLARE_H
#include "CTagContainer.h"

class CTagDeclare : public CTagAtamar
{
	void addTempIdentifiers(int count, const string& nodeName, int position)const;// add functions arguments (x,y,z,t) to identifiers 
	void ereaseTempIdentifiers(int count)const;//delete function's arguments
	void addFunction(const CNode& node)const;//add Function to TagContainer (just 0 - 4 arg func x,y,z,t - it's arguments
	void addIdentifier(const CNode& node)const;
public:

	void operator ()(const CNode& node) const;
};

#endif