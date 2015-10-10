#include "CTagContainer.h"
#include "Tag.h"
#include "TagQualifiers.h"
#include "TagFunctions.h"


map< string, unique_ptr< CTag > > CTagContainer::CTagContainerBuild()
{
    map< string, unique_ptr<CTag> > tagsToFill;

    /***********insert here tags*********************/

    tagsToFill.emplace( "apply", unique_ptr<CTag>( ( CTag* ) new CTagApply() ) );
    tagsToFill.emplace( "plus", unique_ptr<CTag>( ( CTag* ) new CTagVarArgFunction<NUMBER, NUMBER>() ) );
    tagsToFill.emplace( "cn", unique_ptr<CTag>( (CTag*) new CTagCn() ) );
    tagsToFill.emplace( "ci", unique_ptr<CTag>( (CTag*) new CTagCi() ) );
    tagsToFill.emplace( "degree", unique_ptr<CTag>( (CTag*) new CTagQualifiers( DEGREE ) ) );
    tagsToFill.emplace( "lowlimit", unique_ptr<CTag>( (CTag*) new CTagQualifiers(LIMIT_LO) ) );
    tagsToFill.emplace( "uplimit", unique_ptr<CTag>( (CTag*) new CTagQualifiers(LIMIT_UP) ) );
    tagsToFill.emplace( "condition", unique_ptr<CTag>( (CTag*) new CTagCondition() ) );


    /**********************************************/

	return tagsToFill;
};

const map< string, unique_ptr< CTag > > CTagContainer::tags = CTagContainer::CTagContainerBuild();


CTag& CTagContainer::getTag(const string& name)
{
    auto foundTagIterator = tags.find( name );
	if ( foundTagIterator == tags.end() )
		throw invalid_argument( ( string( "no such tag: " ) + name ) );
	return *(foundTagIterator->second);
};