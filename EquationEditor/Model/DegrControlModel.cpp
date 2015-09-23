#include "Model/DegrControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/GeneralFunct.h"


CDegrControlModel::CDegrControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = 0;
	parent = nullptr;

	сhild = std::make_shared<CExprControlModel>( CExprControlModel( ) );
	linkChildWithParent(сhild, this);

}

std::list<std::shared_ptr<IBaseExprModel>> CDegrControlModel::GetChildren( ) {
	return std::list<std::shared_ptr<IBaseExprModel>> { сhild };
}

ViewType CDegrControlModel::GetType() {
	return DEGR;
}
