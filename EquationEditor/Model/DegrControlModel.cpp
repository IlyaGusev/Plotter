#include "Model/DegrControlModel.h"
#include "Model/EditControlModel.h"

CDegrControlModel::CDegrControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent ) :
	IBaseExprModel( rect, parent ) 
{
	depth = parent.lock()->GetDepth() + 1;
}

void CDegrControlModel::Resize()
{
	// стараемся держать мидл показателя на высоте верха основания.
	// если показатель слишком большой (больше основания), то низ показателя ложится на мидл основания
	int width = firstChild->GetRect().GetWidth() + secondChild->GetRect().GetWidth();
	int height = 0;
	if( firstChild->GetRect().GetHeight() <= secondChild->GetRect().GetHeight() ) {
		height = firstChild->GetMiddle() + secondChild->GetRect().GetHeight();
	}
	else {
		height = firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight() - secondChild->GetMiddle();
	}
	
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
	newRect.Bottom() = newRect.Top() + oldRect.GetHeight();
	newRect.Left() = secondChild->GetRect().Right();
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	firstChild->SetRect(newRect);
}

int CDegrControlModel::GetMiddle() const
{
	return rect.GetHeight() - secondChild->GetRect().GetHeight() + secondChild->GetMiddle();
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

ViewType CDegrControlModel::GetType() const 
{
	return DEGR;
}

void CDegrControlModel::MoveBy(int dx, int dy) 
{
	rect.MoveBy(dx, dy);
	params.polygon.front().MoveBy(dx, dy);
}

void CDegrControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ ) 
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

void CDegrControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode/*=false */ ) 
{
	// Если пришли из родителя - идем в основание
	if( from == parent.lock().get() ) {
		secondChild->MoveCaretRight( this, caret, isInSelectionMode );
	}
	//если из основания - в показатель
	else if( from == secondChild.get() ) {
		firstChild->MoveCaretRight( this, caret, isInSelectionMode );
	} else {
		// Иначе идем наверх
		parent.lock()->MoveCaretRight( this, caret, isInSelectionMode );
	}
}

bool CDegrControlModel::IsEmpty() const {
	return firstChild->IsEmpty() && secondChild->IsEmpty();
}

bool CDegrControlModel::IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const {
	return model1 == secondChild.get();
}

// Высота выступающего над основанием показателя степени
int CDegrControlModel::getExponentHeight( int rectHeight )
{
	return rectHeight / 4 > CEditControlModel::MINIMAL_HEIGHT ? rectHeight / 4 : CEditControlModel::MINIMAL_HEIGHT;
}

void CDegrControlModel::UpdateSelection()
{
	if( firstChild->IsSelected() && secondChild->IsSelected() ) {
		params.isSelected = true;
	} else {
		params.isSelected = false;
	}
}