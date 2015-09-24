#include "Model/DegrControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/GeneralFunct.h"


CDegrControlModel::CDegrControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = -1;
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

void CDegrControlModel::PermutateChildren( )
{
	throw std::exception( "not implemented" );
}

ViewType CDegrControlModel::GetType() const {
	return DEGR;
}
