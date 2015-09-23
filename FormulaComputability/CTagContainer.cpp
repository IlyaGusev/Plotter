#include "CTagContainer.h"


const unordered_map<string, CTag*> CTagContainer::CTagContainerBuild()
{
	pair< string, CTag* > TagList[] = {

		/***********write here tags*********************/

		pair< string, CTag* >("apply", (CTag*) new CTagApply())

		/**********************************************/

	};
	//return unordered_map<string, CTag*>(TagList, TagList + sizeof(TagList) / sizeof(pair< string, CTag* >));
    return unordered_map<string, CTag*>(TagList);
};

const unordered_map<string, CTag*> CTagContainer::tags = CTagContainer::CTagContainerBuild();

void CTagContainer::CTagContainerDestroy() {
    std::unordered_map<std::string, CTag*>::iterator itEnd = tags.end();
    std::unordered_map<std::string, CTag*>::iterator it;
    for ( it = tags.begin(); it != itEnd; ++it ) {
        delete it->second;
    }
}