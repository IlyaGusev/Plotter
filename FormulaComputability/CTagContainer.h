#pragma once
#include <map>
using namespace std;

class CTag;

class CTagContainer
{
public:
	static const map<string, CTag*> Tags;
	static void CTagContainerDestroy();
private:
	static const map<string, CTag*> CTagContainerBuild();
};

CTag* getTag(string name);