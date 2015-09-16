#include "EditControlModel.h"

CEditControlModel::CEditControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = 0;
	parent = nullptr;
}

IBaseExprModel* CEditControlModel::GetParent( ) {
	return parent;
}

void CEditControlModel::SetParent( IBaseExprModel* newParent ) {
	parent = newParent;
}

RECT CEditControlModel::GetRect() {
	return rect;
}

void CEditControlModel::SetRect( RECT newRect ) {
	rect = newRect;
}

std::list< IBaseExprModel* > CEditControlModel::GetChilds( ) {
	return std::list< IBaseExprModel* >();
}

void CEditControlModel::AddChild( IBaseExprModel* child ) {
}