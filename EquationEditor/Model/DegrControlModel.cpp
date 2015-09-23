#include "Model/DegrControlModel.h"
#include "Model/EditControlModel.h"

CDegrControlModel::CDegrControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = 0;
	parent = nullptr;

	сhild = std::make_shared<CExprControlModel>( CExprControlModel( ) );
	сhild->SetParent( std::shared_ptr<IBaseExprModel>(this) );
	std::shared_ptr<CEditControlModel> сhildEdit( new CEditControlModel() );

	сhild->AddChild(сhildEdit);
	сhildEdit->SetParent(сhild);
}

std::list<std::shared_ptr<IBaseExprModel>> CDegrControlModel::GetChildren( ) {
	return std::list<std::shared_ptr<IBaseExprModel>> { сhild };
}

ViewType CDegrControlModel::GetType() {
	return DEGR;
}
