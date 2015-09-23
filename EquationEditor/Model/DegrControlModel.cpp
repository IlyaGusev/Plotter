#include "Model/DegrControlModel.h"
#include "Model/EditControlModel.h"

CDegrControlModel::CDegrControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = 0;
	parent = nullptr;

	ñhild = std::make_shared<CExprControlModel>( CExprControlModel( ) );
	ñhild->SetParent( std::shared_ptr<IBaseExprModel>(this) );
	std::shared_ptr<CEditControlModel> ñhildEdit( new CEditControlModel() );

	ñhild->AddChild(ñhildEdit);
	ñhildEdit->SetParent(ñhild);
}

std::list<std::shared_ptr<IBaseExprModel>> CDegrControlModel::GetChildren( ) {
	return std::list<std::shared_ptr<IBaseExprModel>> { ñhild };
}

ViewType CDegrControlModel::GetType() {
	return DEGR;
}
