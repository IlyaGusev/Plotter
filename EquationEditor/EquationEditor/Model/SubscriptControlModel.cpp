#include "Model/SubscriptControlModel.h"
#include "Model/EditControlModel.h"

CSubscriptControlModel::CSubscriptControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent ) :
	IBaseExprModel(rect, parent)
{
	depth = parent.lock()->GetDepth() + 1;
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

std::wstring CSubscriptControlModel::Serialize() {
	std::wstring result = L"";

	if (!firstChild->IsEmpty()) {
		result += L"<apply><selector/>" + firstChild->Serialize();
	}
	result += params.text;
	if (!secondChild->IsEmpty()) {
		result += secondChild->Serialize() + L"</apply>";
	}

	return result;
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

void CSubscriptControlModel::InitializeChildren( std::shared_ptr<IBaseExprModel> initChild /*= 0 */ )
{
	if( initChild ) {
		firstChild = initChild;
		firstChild->SetParent( shared_from_this( ) );
		firstChild->UpdateDepth();
	} else {
		CRect firstChildRect = CRect( 0, 0, 0, rect.GetHeight() );
		firstChild = std::make_shared<CExprControlModel>( firstChildRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
		firstChild->InitializeChildren();
	}

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

ViewType CSubscriptControlModel::GetType() const 
{
	return SUBSCRIPT;
}

void CSubscriptControlModel::MoveBy(int dx, int dy) 
{
	rect.MoveBy(dx, dy);
	params.polygon.front().MoveBy(dx, dy);
}

void CSubscriptControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	if (isInSelectionMode)
		params.isSelected = true;
	// Если пришли из индекса - идём в основание
	if( from == secondChild.get() ) {
		firstChild->MoveCaretLeft( this, caret, isInSelectionMode );
	}
	//если пришли из родителя - идём в индекс
	else if( from == parent.lock().get() ) {
		secondChild->MoveCaretLeft( this, caret, isInSelectionMode );
	}
	else {
		// Иначе идем наверх
		parent.lock()->MoveCaretLeft( this, caret, isInSelectionMode );
	}
}

void CSubscriptControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	if (isInSelectionMode)
		params.isSelected = true;
	// Если пришли из родителя - идем в основание
	if( from == parent.lock().get() ) {
		firstChild->MoveCaretRight( this, caret, isInSelectionMode );
	}
	//если из основания - в индекс
	else if( from == firstChild.get() ) {
		secondChild->MoveCaretRight( this, caret, isInSelectionMode );
	}
	else {
		// Иначе идем наверх
		parent.lock()->MoveCaretRight( this, caret, isInSelectionMode );
	}
}

bool CSubscriptControlModel::IsEmpty() const {
	return firstChild->IsEmpty() && secondChild->IsEmpty();
}

//bool CSubscriptControlModel::IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const 
//{
//	// Если первая - действительно первая
//	return model1 == firstChild.get();
//}

// Высота выступающего снизу индекса
int CSubscriptControlModel::getSubscriptHeight( int rectHeight ) {
	return rectHeight / 4 > CEditControlModel::MINIMAL_HEIGHT ? rectHeight / 4 : CEditControlModel::MINIMAL_HEIGHT;
}

void CSubscriptControlModel::UpdateSelection()
{
	if (!(firstChild->IsSelected()) || !(secondChild->IsSelected()))
		params.isSelected = false;
}

std::shared_ptr<IBaseExprModel> CSubscriptControlModel::CopySelected() const
{
	std::shared_ptr<CSubscriptControlModel> newSubscriptionModel( new CSubscriptControlModel( rect, parent ) );
	std::shared_ptr<IBaseExprModel> firstModel = firstChild->CopySelected();
	std::shared_ptr<IBaseExprModel> secondModel = secondChild->CopySelected();

	// Если один из выбранных кусков пуст - возвращаем второй кусок (т.е. ExprControl, а не SubscriptControl)
	if( firstModel == 0 || firstModel->IsEmpty() || secondModel == 0 || secondModel->IsEmpty() ) {
		return (firstModel != 0 && !firstModel->IsEmpty()) ? firstModel : secondModel;
	}

	// Иначе возвращаем целую модельку
	newSubscriptionModel->firstChild = firstModel;
	newSubscriptionModel->firstChild->SetParent( newSubscriptionModel );

	newSubscriptionModel->secondChild = secondChild->CopySelected();
	newSubscriptionModel->secondChild->SetParent( newSubscriptionModel );
	return newSubscriptionModel;
}
