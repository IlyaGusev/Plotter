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
	int width = MAX( firstChild->Rect().GetWidth(), secondChild->Rect().GetWidth());
	int height = firstChild->Rect().GetHeight() + secondChild->Rect().GetHeight() + 5; // +5 для промежутка между числителем и знаменателем

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CFracControlModel::PermutateChildren( )
{
	CRect newRect;
	int middle = (rect.Right() + rect.Left()) / 2;
	
	CRect oldRect = firstChild->Rect( );
	newRect.Top() = rect.Top();
	newRect.Bottom() = rect.Top() + oldRect.GetHeight();
	newRect.Left() = middle - oldRect.GetWidth() / 2;
	newRect.Right() = middle + oldRect.GetWidth() / 2;
	firstChild->SetRect( newRect );
	
	oldRect = secondChild->Rect( );
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = rect.Bottom() - oldRect.GetHeight();
	newRect.Left() = middle - oldRect.GetWidth() / 2;
	newRect.Right() = middle + oldRect.GetWidth() / 2;
	secondChild->SetRect( newRect );
}

int CFracControlModel::GetMiddle( ) const
{
	return (firstChild->Rect().Bottom() + secondChild->Rect().Top()) / 2 - rect.Top();
}

std::list<std::shared_ptr<IBaseExprModel>> CFracControlModel::GetChildren() const {
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CFracControlModel::SetRect( CRect rect ) {
	this->rect = rect;
	params.polygon.front( ).Set( rect.Left( ), Rect( ).Top( ) + GetMiddle( ), rect.Right( ), Rect( ).Top( ) + GetMiddle( ) );
	//CRect firstChildRect = firstChild->Rect();
	//CRect secondChildRect = secondChild->Rect();
	//params.polygon.front().Set(rect.Left(), secondChildRect.Top() - 1, rect.Right(), secondChildRect.Top() - 1);
}

ViewType CFracControlModel::GetType() const {
	return FRAC;
}

void CFracControlModel::MoveBy( int dx, int dy ) {
	rect.MoveBy( dx, dy );
	params.polygon.front().MoveBy( dx, dy );
}
