#include "CTagContainer.h"
#include "Tag.h"


const map<string, CTag*> CTagContainer::CTagContainerBuild()
{
	pair< string, CTag* > TagList[] = {

		/***********write here tags*********************/

		pair< string, CTag* >("apply", (CTag*) new CTagApply()),
		pair< string, CTag* >("plus", (CTag*) new CTagBinaryNumFunction() ),
		pair< string, CTag* >("cn", (CTag*) new CTagCn())
		/**********************************************/

	};
	return map<string, CTag*>(TagList, TagList + sizeof(TagList) / sizeof(pair< string, CTag* >));
};

const map<string, CTag*> CTagContainer::Tags = CTagContainer::CTagContainerBuild();

void CTagContainer::CTagContainerDestroy() {
    auto itEnd = Tags.end();
    for ( auto it = Tags.begin(); it != Tags.end(); ++it ) {
        delete it->second;
    }
};

CTag* getTag(string name)
{
	if (CTagContainer::Tags.find(name) == CTagContainer::Tags.end())
		throw exception((string("no such tag: ") + name).c_str());
	return CTagContainer::Tags.find(name)->second;
};