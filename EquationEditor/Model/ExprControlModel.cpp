#include "Model/ExprControlModel.h"

CExprControlModel::CExprControlModel( ) {
	rect.bottom = rect.top = rect.left = rect.right = -1;
	parent = nullptr;
}

IBaseExprModel* CExprControlModel::GetParent( ) {
	return parent;
}

void CExprControlModel::SetParent( IBaseExprModel* newParent ) {
	parent = newParent;
}

std::list<IBaseExprModel*> CExprControlModel::GetChildren( ) {
	return children;
}

RECT CExprControlModel::GetRect() {
	return rect;
}

void CExprControlModel::SetRect( RECT newRect ) {
	rect = newRect;
}

void CExprControlModel::AddChild( IBaseExprModel* child ) {
	children.push_back( child );
	if( rect.left != -1 ) {
		rect.left = min( rect.left, child->GetRect().left );
		rect.top = min( rect.top, child->GetRect().top );
		rect.right = max( rect.right, child->GetRect().right );
		rect.bottom = max( rect.bottom, child->GetRect().bottom );
	} else {
		rect.left = child->GetRect().left;
		rect.top = child->GetRect().top;
		rect.right = child->GetRect().right;
		rect.bottom = child->GetRect().bottom;
	}
}