#include "Model/FracControlModel.h"
#include "Model/EditControlModel.h"

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

IBaseExprModel* CFracControlModel::GetParent() const
{
	return parent;
}

void CFracControlModel::SetParent( IBaseExprModel* newParent )
{
	parent = newParent;
}

std::list<IBaseExprModel*> CFracControlModel::GetChildren() const
{
	return std::list<IBaseExprModel*> { firstChild, secondChild };
}

RECT CFracControlModel::GetRect() const {
	return rect;
}

void CFracControlModel::SetRect( RECT newRect ) {
	rect = newRect;
}

void CFracControlModel::Resize( )
{
	int width = firstChild->GetRect().right - firstChild->GetRect().left;
	if (secondChild->GetRect().right - secondChild->GetRect().left > width)
	{
		width = secondChild->GetRect().right - secondChild->GetRect().left;
	}
	int height = firstChild->GetRect().bottom - firstChild->GetRect().top 
		+ secondChild->GetRect().bottom - secondChild->GetRect().top
		+ 5; // +5 для промежутка между числителем и знаменателем

	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
}

void CFracControlModel::PermutateChildren( )
{
	RECT newRect;
	int middle = rect.right + rect.left / 2;
	
	RECT oldRect = firstChild->GetRect( );
	newRect.top = rect.top;
	newRect.bottom = rect.top + oldRect.bottom - oldRect.top;
	newRect.left = middle - (oldRect.right - oldRect.left) / 2;
	newRect.right = middle + (oldRect.right - oldRect.left) / 2;
	firstChild->SetRect( newRect );
	
	oldRect = secondChild->GetRect( );
	newRect.bottom = rect.bottom;
	newRect.top = rect.bottom - (oldRect.bottom - oldRect.top);
	newRect.left = middle - (oldRect.right - oldRect.left) / 2;
	newRect.right = middle + (oldRect.right - oldRect.left) / 2;
	secondChild->SetRect( newRect );
}

CDrawParams CFracControlModel::GetDrawParams() const {
	CDrawParams params;
	params.polygon.push_back( CLine( rect.left, (rect.bottom + rect.top) / 2, rect.right, (rect.bottom + rect.top) / 2 ) );
	return params;
}
