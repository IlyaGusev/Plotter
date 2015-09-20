#include "FracControlModel.h"
#include "EditControlModel.h"

CFracControlModel::CFracControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = 0;
	parent = nullptr;

	firstChild = new CExprControlModel();
	firstChild->SetParent( this );
	CEditControlModel* firstChildEdit = new CEditControlModel();
	secondChild = new CExprControlModel();
	secondChild->SetParent( this );
	CEditControlModel* secondChildEdit = new CEditControlModel();

	firstChild->AddChild( firstChildEdit );
	firstChildEdit->SetParent( firstChild );
	secondChild->AddChild( secondChildEdit );
	secondChildEdit->SetParent( secondChild );
}

IBaseExprModel* CFracControlModel::GetParent() {
	return parent;
}

void CFracControlModel::SetParent( IBaseExprModel* newParent ) {
	parent = newParent;
}

std::list<IBaseExprModel*> CFracControlModel::GetChildren() {
	return std::list<IBaseExprModel*> { firstChild, secondChild };
}

RECT CFracControlModel::GetRect() {
	return rect;
}

void CFracControlModel::SetRect( RECT newRect ) {
	rect = newRect;
}

CDrawParams CFracControlModel::GetDrawParams() {
	CDrawParams params;
	params.polygon.push_back( CLine( rect.left, (rect.bottom + rect.top) / 2, rect.right, (rect.bottom + rect.top) / 2 ) );
	return params;
}
