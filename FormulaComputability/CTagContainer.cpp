#include "CTagContainer.h"


const unordered_map<string, CTag*> CTagContainer::CTagContainerBuild()
{
	pair< string, CTag* > TagList[] = {

		/***********write here tags*********************/

		pair< string, CTag* >("apply", (CTag*) new CTagApply())

		/**********************************************/

	};
	return unordered_map<string, CTag*>(TagList, TagList + sizeof(TagList) / sizeof(pair< string, CTag* >));
};

const unordered_map<string, CTag*> CTagContainer::Tags = CTagContainer::CTagContainerBuild();