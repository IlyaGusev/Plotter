#include "Model/SubscriptControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/Utils/GeneralFunct.h"

#include <string>

CSubscriptControlModel::CSubscriptControlModel(CRect rect, std::weak_ptr<IBaseExprModel> parent) :
	IBaseExprModel(rect, parent)
{
}

void CSubscriptControlModel::Resize()
{
	// стараемся держать мидл индекса на высоте низа индексируемого выражения.
	// если индекс слишком большой (больше основания), то верх показателя упирается в мидл основания
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

void CSubscriptControlModel::PlaceChildren()
{
	CRect newRect;

	CRect oldRect = firstChild->GetRect();
	newRect.Top() = rect.Top();
	newRect.Bottom() = newRect.Top() + oldRect.GetHeight();
	newRect.Left() = rect.Left();
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	firstChild->SetRect(newRect);
	
	oldRect = secondChild->GetRect();
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = newRect.Bottom() - oldRect.GetHeight();
	newRect.Left() = firstChild->GetRect().Right();
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	secondChild->SetRect(newRect);
}

int CSubscriptControlModel::GetMiddle() const
{
	return firstChild->GetMiddle();

}

void CSubscriptControlModel::InitializeChildren()
{
	CRect firstChildRect = CRect( 0, 0, 0, rect.GetHeight() );
	firstChild = std::make_shared<CExprControlModel>( firstChildRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	firstChild->InitializeChildren();

	CRect secondChildRect = CRect( 0, 0, 0, 3 * getSubscriptHeight( rect.GetHeight() ) );
	secondChild = std::make_shared<CExprControlModel>( secondChildRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	secondChild->InitializeChildren();

	Resize();
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CSubscriptControlModel::GetChildren() const 
{
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CSubscriptControlModel::SetRect(const CRect& rect) 
{
	this->rect = rect;
}

ViewType CSubscriptControlModel::GetType() const 
{
	return SUBSCRIPT;
}

void CSubscriptControlModel::MoveBy(int dx, int dy) 
{
	rect.MoveBy(dx, dy);
	params.polygon.front().MoveBy(dx, dy);
}

void CSubscriptControlModel::MoveCaretLeft(const IBaseExprModel* from, CCaret& caret) const 
{
	// Если пришли из индекса - идём в основание
	if( from == secondChild.get() ) {
		firstChild->MoveCaretLeft( this, caret );
	}
	//если пришли из родителя - идём в индекс
	else if( from == parent.lock().get() ) {
		secondChild->MoveCaretLeft( this, caret );
	}
	else {
		// Иначе идем наверх
		parent.lock()->MoveCaretLeft( this, caret );
	}
}

void CSubscriptControlModel::MoveCaretRight(const IBaseExprModel* from, CCaret& caret) const {
	// Если пришли из родителя - идем в основание
	if( from == parent.lock().get() ) {
		firstChild->MoveCaretRight( this, caret );
	}
	//если из основания - в индекс
	else if( from == firstChild.get() ) {
		secondChild->MoveCaretRight( this, caret );
	}
	else {
		// Иначе идем наверх
		parent.lock()->MoveCaretRight( this, caret );
	}
}

// Высота выступающего снизу индекса
int CSubscriptControlModel::getSubscriptHeight( int rectHeight ) {
	return rectHeight / 4 > CEditControlModel::MINIMAL_HEIGHT ? rectHeight / 4 : CEditControlModel::MINIMAL_HEIGHT;
}