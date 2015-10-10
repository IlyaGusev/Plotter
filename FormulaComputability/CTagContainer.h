#pragma once
#ifndef _CTAGCONTAINER_H
#define _CTAGCONTAINER_H

#include <map>
#include <memory>
#include <utility>

using namespace std;

class CTag;

class CTagContainer 
{
public:
    static CTag& getTag(const string& name);
    static const map< string, unique_ptr< CTag > > tags;
private:
	static map< string, unique_ptr< CTag > > CTagContainerBuild();
};

#endif