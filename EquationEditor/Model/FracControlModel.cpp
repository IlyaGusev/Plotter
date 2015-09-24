#include "Model/FracControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/Utils/GeneralFunct.h"

CFracControlModel::CFracControlModel() {
	parent = nullptr;

	firstChild = std::make_shared<CExprControlModel>( CExprControlModel( ) );
	linkChildWithParent(firstChild, this);
	
	secondChild = std::make_shared<CExprControlModel>( CExprControlModel() );
	linkChildWithParent(secondChild, this);

	params.polygon.push_back( CLine(0, 0, 0, 0) );
}

void CFracControlModel::Resize( )
{
	int width = firstChild->GetRect().Right() - firstChild->GetRect().Left();
	if (secondChild->GetRect().Right() - secondChild->GetRect().Left() > width)
	{
		width = secondChild->GetRect().Right() - secondChild->GetRect().Left();
	}
	int height = firstChild->GetRect().Bottom() - firstChild->GetRect().Top() 
		+ secondChild->GetRect().Bottom() - secondChild->GetRect().Top()
		+ 5; // +5 для промежутка между числителем и знаменателем

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CFracControlModel::PermutateChildren( )
{
	CRectI newRect;
	int middle = (rect.Right() + rect.Left()) / 2;
	
	CRectI oldRect = firstChild->GetRect( );
	newRect.Top() = rect.Top();
	newRect.Bottom() = rect.Top() + oldRect.Bottom() - oldRect.Top();
	newRect.Left() = middle - (oldRect.Right() - oldRect.Left()) / 2;
	newRect.Right() = middle + (oldRect.Right() - oldRect.Left()) / 2;
	firstChild->SetRect( newRect );
	
	oldRect = secondChild->GetRect( );
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = rect.Bottom() - (oldRect.Bottom() - oldRect.Top());
	newRect.Left() = middle - (oldRect.Right() - oldRect.Left()) / 2;
	newRect.Right() = middle + (oldRect.Right() - oldRect.Left()) / 2;
	secondChild->SetRect( newRect );
}

std::list<std::shared_ptr<IBaseExprModel>> CFracControlModel::GetChildren() const {
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CFracControlModel::SetRect( CRectI rect ) {
	this->rect = rect;
	params.polygon.front().Set( rect.Left(), (rect.Bottom() + rect.Top()) / 2, rect.Right(), (rect.Bottom() + rect.Top()) / 2 );
}

ViewType CFracControlModel::GetType() const {
	return FRAC;
}
