#include "Model/FracControlModel.h"
#include "Model/EditControlModel.h"

CFracControlModel::CFracControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent ) :
	IBaseExprModel(rect, parent)
{
	this->rect.Set( 0, 0, 0, rect.GetHeight() ); // нас интересует только высота, остальное исправится сразу же после инвалидации дерева
	this->params.polygon.push_back( CLine( rect.Left( ), rect.GetHeight( ) / 2, rect.Right( ), rect.GetHeight( ) / 2 ) );

	depth = parent.lock()->GetDepth() + 1;
}

void CFracControlModel::Resize()
{
	int width = MAX( firstChild->GetRect().GetWidth(), secondChild->GetRect().GetWidth());
	int height = firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight() + 5; // +5 для промежутка между числителем и знаменателем

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

std::wstring CFracControlModel::Serialize() {
	std::wstring result = L"<apply><csymbol cd=\"arith1\">divide</csymbol>";

	if (!firstChild->IsEmpty()) {
		result += firstChild->Serialize();
	}
	result += params.text;
	if (!secondChild->IsEmpty()) {
		result += secondChild->Serialize() + L"</apply>";
	}

	return result;
}

void CFracControlModel::PlaceChildren()
{
	CRect newRect;
	int middle = (rect.Right() + rect.Left()) / 2;
	
	CRect oldRect = firstChild->GetRect( );
	newRect.Top() = rect.Top();
	newRect.Bottom() = rect.Top() + oldRect.GetHeight();
	newRect.Left() = middle - oldRect.GetWidth() / 2;
	newRect.Right() = middle + oldRect.GetWidth() / 2;
	firstChild->SetRect( newRect );
	
	oldRect = secondChild->GetRect( );
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = rect.Bottom() - oldRect.GetHeight();
	newRect.Left() = middle - oldRect.GetWidth() / 2;
	newRect.Right() = middle + oldRect.GetWidth() / 2;
	secondChild->SetRect( newRect );

	updatePolygons();
}

int CFracControlModel::GetMiddle( ) const
{
	return (firstChild->GetRect().GetHeight() + (rect.GetHeight() - secondChild->GetRect().GetHeight())) / 2;
}

void CFracControlModel::InitializeChildren( std::shared_ptr<IBaseExprModel> initChild /*= 0 */ )
{
	CRect childRect = CRect( 0, 0, 0, rect.GetHeight( ) );
	if( initChild ) {
		firstChild = initChild;
		firstChild->SetParent( shared_from_this() );
		firstChild->UpdateDepth();
	} else {
		firstChild = std::make_shared<CExprControlModel>( childRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
		firstChild->InitializeChildren();
	}

	secondChild = std::make_shared<CExprControlModel>( childRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	secondChild->InitializeChildren();

	Resize();
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CFracControlModel::GetChildren() const 
{
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CFracControlModel::SetRect( const CRect& rect ) 
{
	this->rect = rect;
	updatePolygons();
}

ViewType CFracControlModel::GetType() const 
{
	return FRAC;
}

void CFracControlModel::MoveBy( int dx, int dy ) 
{
	rect.MoveBy( dx, dy );
	updatePolygons();
}

void CFracControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ ) 
{
	if (isInSelectionMode)
		params.isSelected = true;
	// Если пришли из родителя - идем в нижнего ребенка
	if( from == parent.lock().get() ) {
		secondChild->MoveCaretLeft( this, caret, isInSelectionMode );
	} else if( from == secondChild.get() ) {
		firstChild->MoveCaretLeft( this, caret, isInSelectionMode );
	} else {
		// Иначе идем наверх
		parent.lock()->MoveCaretLeft( this, caret, isInSelectionMode );
	}
}

void CFracControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	if (isInSelectionMode)
		params.isSelected = true;
	// Если пришли из родителя - идем в верхнего ребенка
	if( from == parent.lock().get() ) {
		firstChild->MoveCaretRight( this, caret, isInSelectionMode );
	} else if( from == firstChild.get() ) {
		// Если пришли из верхнего - идем в нижнего
		secondChild->MoveCaretRight( this, caret, isInSelectionMode );
	} else {
		// Иначе идем наверх
		parent.lock()->MoveCaretRight( this, caret, isInSelectionMode );
	}
}

bool CFracControlModel::IsEmpty() const 
{
	return firstChild->IsEmpty( ) && secondChild->IsEmpty( );
}

void CFracControlModel::updatePolygons()
{
	params.polygon.front().Set( rect.Left(), rect.Top() + GetMiddle(), rect.Right(), rect.Top() + GetMiddle() );
}

void CFracControlModel::UpdateSelection()
{
	if (!(firstChild->IsSelected()) || !(secondChild->IsSelected()))
		params.isSelected = false;
}

std::shared_ptr<IBaseExprModel> CFracControlModel::CopySelected() const
{
	std::shared_ptr<CFracControlModel> fracModel( new CFracControlModel( rect, parent ) );
	std::shared_ptr<IBaseExprModel> firstModel = firstChild->CopySelected();
	std::shared_ptr<IBaseExprModel> secondModel = secondChild->CopySelected();

	// Если один из выбранных кусков пуст - возвращаем второй кусок (т.е. ExprControl, а не FracControl)
	if( firstModel == 0 || firstModel->IsEmpty() || secondModel == 0 || secondModel->IsEmpty() ) {
		return (firstModel != 0 && !firstModel->IsEmpty() || firstModel->IsSelected()) ? firstModel : secondModel;
	}
	// Иначе возвращаем целую дробь
	fracModel->firstChild = firstModel;
	fracModel->firstChild->SetParent( fracModel );

	fracModel->secondChild = secondModel;
	fracModel->secondChild->SetParent( fracModel );
	return fracModel;
}
