#include "Model/DegrControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/Utils/GeneralFunct.h"

#include <string>

void CDegrControlModel::Resize()
{
	int width = firstChild->GetRect().GetWidth() + secondChild->GetRect().GetWidth();
	int height = firstChild->GetRect().GetHeight() - getExponentHeight( 4 * firstChild->GetRect().GetHeight() / 3 ) + secondChild->GetRect().GetHeight();
	
	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CDegrControlModel::PlaceChildren()
{
	CRect newRect;

	CRect oldRect = secondChild->GetRect();
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = rect.Bottom() - oldRect.GetHeight();
	newRect.Left() = rect.Left();
	newRect.Right() = rect.Left() + oldRect.GetWidth();
	secondChild->SetRect(newRect);
	
	oldRect = firstChild->GetRect();
	newRect.Top() = rect.Top();
	newRect.Bottom() = rect.Top() + oldRect.GetHeight();
	newRect.Left() = secondChild->GetRect().Right();
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	firstChild->SetRect(newRect);
}

int CDegrControlModel::GetMiddle() const
{
	return (rect.GetHeight() - secondChild->GetRect().GetHeight() + secondChild->GetMiddle());
}

void CDegrControlModel::InitializeChildren()
{
	CRect firstChildRect = CRect( 0, 0, 0, 3 * getExponentHeight( rect.GetHeight() ) );
	firstChild = std::make_shared<CExprControlModel>( firstChildRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	firstChild->InitializeChildren();

	CRect secondChildRect = CRect( 0, 0, 0, rect.GetHeight() );
	secondChild = std::make_shared<CExprControlModel>( secondChildRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	secondChild->InitializeChildren();

	Resize();
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CDegrControlModel::GetChildren() const 
{
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CDegrControlModel::SetRect(const CRect& rect) 
{
	this->rect = rect;
}

ViewType CDegrControlModel::GetType() const 
{
	return DEGR;
}

void CDegrControlModel::MoveBy(int dx, int dy) 
{
	rect.MoveBy(dx, dy);
	params.polygon.front().MoveBy(dx, dy);
}

void CDegrControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret ) const 
{
	// Если пришли из показателя - идём в основание
	if( from == firstChild.get() ) {
		secondChild->MoveCaretLeft( this, caret );
	}
	//если пришли из родителя - идём в показатель
	else if ( from == parent.lock().get() ) {
		firstChild->MoveCaretLeft( this, caret );
	} else {
		// Иначе идем наверх
		parent.lock()->MoveCaretLeft( this, caret );
	}
}

void CDegrControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret ) const 
{
	// Если пришли из родителя - идем в основание
	if( from == parent.lock().get() ) {
		secondChild->MoveCaretRight( this, caret );
	}
	//если из основания - в показатель
	else if( from == secondChild.get() ) {
		firstChild->MoveCaretRight( this, caret );
	} else {
		// Иначе идем наверх
		parent.lock()->MoveCaretRight( this, caret );
	}
}

// Высота выступающего над основанием показателя степени
int CDegrControlModel::getExponentHeight( int rectHeight )
{
	return rectHeight / 4 > 3 ? rectHeight / 4 : 3;
}
