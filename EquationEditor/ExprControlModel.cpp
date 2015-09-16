#include "ExprControlModel.h"

CExprControlModel::CExprControlModel( ) {
	rect.bottom = rect.top = rect.left = rect.right = 0;
	parent = nullptr;
}

IBaseExprModel* CExprControlModel::GetParent( ) {
	return parent;
}

void CExprControlModel::SetParent( IBaseExprModel* newParent ) {
	parent = newParent;
}

std::list<IBaseExprModel*> CExprControlModel::GetChilds( ) {
	return childs;
}

RECT CExprControlModel::GetRect() {
	return rect;
}

void CExprControlModel::SetRect( RECT newRect ) {
	rect = newRect;
}

void CExprControlModel::AddChild( IBaseExprModel* child ) {
	childs.push_back( child );
}