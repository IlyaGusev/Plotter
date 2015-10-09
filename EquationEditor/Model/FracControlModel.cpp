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

void CFracControlModel::InitializeChildren()
{
	CRect childRect = CRect( 0, 0, 0, rect.GetHeight() );
	firstChild = std::make_shared<CExprControlModel>( childRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	firstChild->InitializeChildren();

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

void CFracControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode/*=false */ )
{
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

bool CFracControlModel::IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const {
	return model1 == firstChild.get();
}

void CFracControlModel::updatePolygons()
{
	params.polygon.front().Set( rect.Left(), rect.Top() + GetMiddle(), rect.Right(), rect.Top() + GetMiddle() );
}

void CFracControlModel::UpdateSelection()
{
	if( firstChild->IsSelected() && secondChild->IsSelected() ) {
		params.isSelected = true;
	} else {
		params.isSelected = false;
	}
}