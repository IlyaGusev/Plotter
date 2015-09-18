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

CDrawParams CEditControlModel::GetDrawParams() {
	return params;
}

void CEditControlModel::InsertSymbol( wchar_t symbol, int offset, int symbolWidth ) {
	params.text.insert( offset, 1, symbol );
	rect.right += symbolWidth;
}

void CEditControlModel::DeleteSymbol( int offset, int symbolWidth ) {
	params.text.erase( offset, 1 );
	rect.right -= symbolWidth;
}