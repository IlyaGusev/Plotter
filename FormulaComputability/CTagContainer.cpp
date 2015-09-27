#include "CTagContainer.h"
#include "Tag.h"


map< string, unique_ptr< CTag > > CTagContainer::CTagContainerBuild()
{
    map< string, unique_ptr<CTag> > tagsToFill;

    /***********insert here tags*********************/

    tagsToFill.emplace( "apply", unique_ptr<CTag>( ( CTag* ) new CTagApply() ) );
    tagsToFill.emplace( "plus", unique_ptr<CTag>( ( CTag* ) new CTagVarArgFunction() ) );
    tagsToFill.emplace( "cn", unique_ptr<CTag>( (CTag*) new CTagCn() ) );

    /**********************************************/

	return tagsToFill;
};

const map< string, unique_ptr< CTag > > CTagContainer::tags = CTagContainer::CTagContainerBuild();


CTag* CTagContainer::getTag(const string& name)
{
    auto foundTagIterator = tags.find( name );
	if ( foundTagIterator == tags.end() )
		throw invalid_argument( ( string( "no such tag: " ) + name ) );
	return foundTagIterator->second.get();
};