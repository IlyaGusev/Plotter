#include "Model/DegrControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/Utils/GeneralFunct.h"


CDegrControlModel::CDegrControlModel() {
	parent = nullptr;
	child = std::make_shared<CExprControlModel>( CExprControlModel( ) );

	linkChildWithParent(child, this); 
}

std::list<std::shared_ptr<IBaseExprModel>> CDegrControlModel::GetChildren( ) const {
	return std::list<std::shared_ptr<IBaseExprModel>> { child };
}

void CDegrControlModel::Resize( )
{
	throw std::exception( "not implemented" );
}

void CDegrControlModel::PlaceChildren( )
{
	throw std::exception( "not implemented" );
}

int CDegrControlModel::GetMiddle( ) const
{
	throw std::exception( "not implemented" );
}

ViewType CDegrControlModel::GetType() const {
	return DEGR;
}
