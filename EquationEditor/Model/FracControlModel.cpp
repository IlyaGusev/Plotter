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
	int middle = (rect.right + rect.left) / 2;
	
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

std::list<std::shared_ptr<IBaseExprModel>> CFracControlModel::GetChildren() const {
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CFracControlModel::SetRect( RECT rect ) {
	this->rect = rect;
	params.polygon.front().Set( rect.left, (rect.bottom + rect.top) / 2, rect.right, (rect.bottom + rect.top) / 2 );
}

ViewType CFracControlModel::GetType() const {
	return FRAC;
}
