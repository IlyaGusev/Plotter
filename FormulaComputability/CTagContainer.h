#pragma once
#ifndef _CTAGCONTAINER_H
#define _CTAGCONTAINER_H
#include "common.h"
class CTag;

class CTagContainer
{
public:
    static CTag& getTag(const string& name);
    static map< string, unique_ptr< CTag > > tags;
	static void addTag(const string&, CTag*, int);
private:
	static map< string, unique_ptr< CTag > > CTagContainerBuild();
};

#endif
