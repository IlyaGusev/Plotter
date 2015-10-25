#include "Model/ProductControlModel.h"
#include "Model/EditControlModel.h"

CProductControlModel::CProductControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent ) :
IBaseExprModel( rect, parent )
{
	this->rect.Set( 0, 0, 0, rect.GetHeight() ); // нас интересует только высота, остальное исправится сразу же после инвалидации дерева

	depth = parent.lock()->GetDepth() + 1;
}

int CProductControlModel::GetSymbolHeight() {
	return rect.GetHeight() - (firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight()); // -10 ?
}


void CProductControlModel::Resize()
{
	int width = MAX( firstChild->GetRect().GetWidth(), secondChild->GetRect().GetWidth() );
	int height = firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight() + MIN( firstChild->GetRect().GetHeight(), secondChild->GetRect().GetHeight() ) * 1.618;

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CProductControlModel::PlaceChildren()
{
	CRect newRect;
	int middle = ( rect.Right() + rect.Left() ) / 2;

	CRect oldRect = firstChild->GetRect();
	newRect.Top() = rect.Top();
	newRect.Bottom() = rect.Top() + oldRect.GetHeight();
	newRect.Left() = middle - oldRect.GetWidth() / 2;
	newRect.Right() = middle + oldRect.GetWidth() / 2;
	firstChild->SetRect( newRect );

	oldRect = secondChild->GetRect();
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = rect.Bottom() - oldRect.GetHeight();
	newRect.Left() = middle - oldRect.GetWidth() / 2;
	newRect.Right() = middle + oldRect.GetWidth() / 2;
	secondChild->SetRect( newRect );

	updatePolygons();
}

int CProductControlModel::GetMiddle() const
{
	return ( firstChild->GetRect().GetHeight() + ( rect.GetHeight() - secondChild->GetRect().GetHeight() ) ) / 2;
}

// новая высота индекса
int CProductControlModel::getIndexHeight( int rectHeight )
{
	return MAX( 3 * rectHeight / 4, CEditControlModel::MINIMAL_HEIGHT );
}

void CProductControlModel::InitializeChildren( std::shared_ptr<IBaseExprModel> initChild )
{
	CRect childRect = CRect( 0, 0, 0, getIndexHeight( rect.GetHeight() ) );
	if ( initChild ) {
		firstChild = initChild;
		firstChild->SetParent( shared_from_this() );
		firstChild->UpdateDepth();
	}
	else {
		firstChild = std::make_shared<CExprControlModel>( childRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
		firstChild->InitializeChildren();
	}

	secondChild = std::make_shared<CExprControlModel>( childRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	secondChild->InitializeChildren();

	Resize();
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CProductControlModel::GetChildren() const
{
	return std::list < std::shared_ptr<IBaseExprModel> > { firstChild, secondChild };
}

void CProductControlModel::SetRect( const CRect& rect )
{
	this->rect = rect;
	updatePolygons();
}

ViewType CProductControlModel::GetType() const
{
	return PRODUCT;
}

void CProductControlModel::MoveBy( int dx, int dy )
{
	rect.MoveBy( dx, dy );
	updatePolygons();
}

void CProductControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	// Если пришли из родителя - идем в нижнего ребенка
	if ( from == parent.lock().get() ) {
		secondChild->MoveCaretLeft( this, caret, isInSelectionMode );
	}
	else if ( from == secondChild.get() ) {
		firstChild->MoveCaretLeft( this, caret, isInSelectionMode );
	}
	else {
		// Иначе идем наверх
		parent.lock()->MoveCaretLeft( this, caret, isInSelectionMode );
	}
}

void CProductControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	// Если пришли из родителя - идем в верхнего ребенка
	if ( from == parent.lock().get() ) {
		firstChild->MoveCaretRight( this, caret, isInSelectionMode );
	}
	else if ( from == firstChild.get() ) {
		// Если пришли из верхнего - идем в нижнего
		secondChild->MoveCaretRight( this, caret, isInSelectionMode );
	}
	else {
		// Иначе идем наверх
		parent.lock()->MoveCaretRight( this, caret, isInSelectionMode );
	}
}

bool CProductControlModel::IsEmpty() const
{
	return firstChild->IsEmpty() && secondChild->IsEmpty();
}

void CProductControlModel::updatePolygons()
{
	params.polygon.clear();

	int piTop = rect.Top() + firstChild->GetRect().GetHeight() + 5;
	int piBottom = rect.Bottom() - secondChild->GetRect().GetHeight() - 5;
	int piLeft = rect.Left() + rect.GetWidth() / 5;
	int piRight = rect.Right() - rect.GetWidth() / 5;
	
	params.polygon.push_back( CLine( piLeft, piTop, piLeft, piBottom ) ); // левая палка
	params.polygon.push_back( CLine( piRight, piTop, piRight, piBottom ) ); // правая палка
	params.polygon.push_back( CLine( rect.Left(), piTop, rect.Right(), piTop ) ); // верхняя перекладина
}

void CProductControlModel::UpdateSelection()
{
	params.isSelected = firstChild->IsSelected() && secondChild->IsSelected();
}

std::shared_ptr<IBaseExprModel> CProductControlModel::CopySelected() const
{
	std::shared_ptr<CProductControlModel> productModel( new CProductControlModel( rect, parent ) );
	std::shared_ptr<IBaseExprModel> firstModel = firstChild->CopySelected();
	std::shared_ptr<IBaseExprModel> secondModel = secondChild->CopySelected();

	productModel->firstChild = firstModel;
	productModel->firstChild->SetParent( productModel );

	productModel->secondChild = secondModel;
	productModel->secondChild->SetParent( productModel );
	return productModel;
}
