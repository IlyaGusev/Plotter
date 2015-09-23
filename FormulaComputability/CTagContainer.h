#include "Tag.h"


class CTagContainer
{
public:
	static const unordered_map<string, CTag*> Tags;
private:
	static const unordered_map<string, CTag*> CTagContainerBuild();
};