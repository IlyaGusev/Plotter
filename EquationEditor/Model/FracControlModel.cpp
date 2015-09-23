#include "Model/FracControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/GeneralFunct.h"

CFracControlModel::CFracControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = 0;
	parent = nullptr;

	firstChild = std::make_shared<CExprControlModel>( CExprControlModel( ) );
	linkChildWithParent(firstChild, this);
	
	secondChild = std::make_shared<CExprControlModel>( CExprControlModel() );
	linkChildWithParent(secondChild, this);

	params.polygon.push_back( CLine(0, 0, 0, 0) );
}

std::list<std::shared_ptr<IBaseExprModel>> CFracControlModel::GetChildren() {
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CFracControlModel::SetRect( RECT rect ) {
	this->rect = rect;
	params.polygon.front().Set( rect.left, (rect.bottom + rect.top) / 2, rect.right, (rect.bottom + rect.top) / 2 );
}

ViewType CFracControlModel::GetType( ) {
	return FRAC;
}
