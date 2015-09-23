#include "Tag.h"


class CTagContainer
{
public:
	static const unordered_map<string, CTag*> tags;
    static void CTagContainerDestroy();
private:
	static const unordered_map<string, CTag*> CTagContainerBuild();
};