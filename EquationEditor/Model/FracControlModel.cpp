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
	int width = MAX( firstChild->GetRect().GetWidth(), secondChild->GetRect().GetWidth());
	int height = firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight() + 5; // +5 для промежутка между числителем и знаменателем

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CFracControlModel::PermutateChildren( )
{
	CRect newRect;
	int middle = (rect.Right() + rect.Left()) / 2;
	
	CRect oldRect = firstChild->GetRect( );
	newRect.Top() = rect.Top();
	newRect.Bottom() = rect.Top() + oldRect.GetHeight();
	newRect.Left() = middle + oldRect.GetWidth() / 2;
	newRect.Right() = oldRect.GetWidth() / 2;
	firstChild->SetRect( newRect );
	
	oldRect = secondChild->GetRect( );
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = rect.Bottom() - oldRect.GetHeight();
	newRect.Left() = middle - oldRect.GetWidth() / 2;
	newRect.Right() = middle + oldRect.GetWidth() / 2;
	secondChild->SetRect( newRect );
}

int CFracControlModel::GetMiddle( ) const
{
	return (firstChild->GetRect().Bottom() + secondChild->GetRect().Top()) / 2 - rect.Top();
}

std::list<std::shared_ptr<IBaseExprModel>> CFracControlModel::GetChildren() const {
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CFracControlModel::SetRect( CRect rect ) {
	this->rect = rect;
	CRect firstChildRect = firstChild->GetRect();
	CRect secondChildRect = secondChild->GetRect();
	params.polygon.front().Set(rect.Left(), secondChildRect.Top() - 1, rect.Right(), secondChildRect.Top() - 1);
}

ViewType CFracControlModel::GetType() const {
	return FRAC;
}