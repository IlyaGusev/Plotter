#include "Tag.h"
#include "TagQualifiers.h"
#include "TagFunctions.h"
#include "TagCi.h"
#include "TagDeclare.h"
#include "CTagContainer.h"

map< string, unique_ptr< CTag > > CTagContainer::CTagContainerBuild()
{
	map< string, unique_ptr<CTag> > tagsToFill;

	/***********insert here tags*********************/

	tagsToFill.emplace( "apply", unique_ptr<CTag>(  new CTagApply() ) );

	tagsToFill.emplace( "plus", unique_ptr<CTag>(  new CTagVarArgFunction<NUMBER, NUMBER>() ) );
	tagsToFill.emplace( "times", unique_ptr<CTag>(  new CTagVarArgFunction<NUMBER, NUMBER>() ) );
	tagsToFill.emplace( "max", unique_ptr<CTag>(  new CTagVarArgFunction<NUMBER, NUMBER>() ) );
	tagsToFill.emplace( "min", unique_ptr<CTag>(  new CTagVarArgFunction<NUMBER, NUMBER>() ) );

	/***** unary elementary classical functions *****/
	// Trigonometric functions
	tagsToFill.emplace( "sin", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "cos", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "tan", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "sec", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "csc", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "cot", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));

	// Hyperbolic functions
	tagsToFill.emplace( "sinh", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "cosh", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "tanh", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "sech", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "csch", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "coth", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));

	// Inverse trigonometric and hyperbolic functions
	tagsToFill.emplace( "arcsin", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arccos", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arctan", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arccosh", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arccot", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arccoth", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arccsc", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arccsch", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arcsec", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arcsech", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arcsinh", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "arctanh", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));


	tagsToFill.emplace( "exp", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "ln", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));
	tagsToFill.emplace( "log", unique_ptr<CTag>( new CTagNArgFunction<NUMBER, NUMBER, 1>()));

	/******/

	tagsToFill.emplace( "minus", unique_ptr<CTag>( new CTagVarArgFunction<NUMBER, NUMBER>()));
	tagsToFill.emplace( "mull", unique_ptr<CTag>( new CTagVarArgFunction<NUMBER, NUMBER>()));
	tagsToFill.emplace( "cn", unique_ptr<CTag>(  new CTagCn() ) );
	tagsToFill.emplace( "ci", unique_ptr<CTag>(  new CTagCi() ) );
	tagsToFill.emplace( "degree", unique_ptr<CTag>(  new CTagQualifiers( DEGREE ) ) );
	tagsToFill.emplace( "lowlimit", unique_ptr<CTag>(  new CTagQualifiers(LIMIT_LO) ) );
	tagsToFill.emplace( "uplimit", unique_ptr<CTag>(  new CTagQualifiers(LIMIT_UP) ) );
	tagsToFill.emplace( "condition", unique_ptr<CTag>(  new CTagCondition() ) );
	tagsToFill.emplace( "declare", unique_ptr<CTag>(  new CTagDeclare() ) );
    tagsToFill.emplace( "bvar", unique_ptr<CTag>( new CTagBVar() ) );

    /* limitable functions*/
    tagsToFill.emplace( "sum", unique_ptr<CTag>( new CTagLimitable() ) );
    tagsToFill.emplace( "product", unique_ptr<CTag>( new CTagLimitable() ));

	/**********************************************/

	return tagsToFill;
};

map< string, unique_ptr< CTag > > CTagContainer::tags = CTagContainer::CTagContainerBuild();


CTag& CTagContainer::getTag(const string& name)
{
	auto foundTagIterator = tags.find( name );
	if ( foundTagIterator == tags.end() )
		throw invalid_argument( ( string( "no such tag: " ) + name ) );
	return *(foundTagIterator->second);
};


void CTagContainer::addTag(const string& name, CTag* tag, int)
{
	if (tags.find(name) != tags.end()) {
		throw invalid_argument("identifier " + name + "has already exist");
    }
    tags.emplace( name, unique_ptr<CTag>( tag ) );
}
