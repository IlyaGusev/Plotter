#include "Model/ProductControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/SumControlModel.h"	


CProductControlModel::CProductControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent ) :
IBaseExprModel( rect, parent )
{
	this->rect.Set( 0, 0, 0, rect.GetHeight() ); // нас интересует только высота, остальное исправится сразу же после инвалидации дерева

	depth = parent.lock()->GetDepth() + 1;
}

int CProductControlModel::GetSymbolHeight() const {
	return symbolRect.GetHeight();
}

void CProductControlModel::setRealChildPresentSumOrProduct()
{
  realChildPresentSum = nullptr;
  realChildPresentProduct = nullptr;
  auto childList = productChild->GetChildren();
  if (!childList.empty()) {
    auto child = std::next(childList.begin());
    if (child != childList.end()) {
      auto type = (*child)->GetType();
      if (type == SUM) {
        realChildPresentSum = std::dynamic_pointer_cast<CSumControlModel>(*child);
      } else if (type == PRODUCT) {
        realChildPresentProduct = std::dynamic_pointer_cast<CProductControlModel>(*child);
      }
    }
  }
}

std::wstring CProductControlModel::Serialize() {
	std::wstring result = L"";

	if (!firstChild->IsEmpty()) {
		result += L"<apply><product/><uplimit>" + firstChild->Serialize() + L"</uplimit>";
	}

	if (!secondChild->IsEmpty()) {
		result += L"<lowlimit>" + secondChild->Serialize() + L"</lowlimit>";
	}

	if (!productChild->IsEmpty()) {
		result += productChild->Serialize() + L"</apply>";
	}

	return result;
}

int CProductControlModel::getSumChildHeight() const
{
	if( realChildPresentSum != nullptr ) {
		return realChildPresentSum->GetSymbolHeight(); // возвращаем высоту знака суммы
	} else if( realChildPresentProduct != nullptr ) {
		return realChildPresentProduct->GetSymbolHeight();
	}
	return productChild->GetRect().GetHeight(); // иначе возвращаем высоту выражения
}

void CProductControlModel::updateSymbolRect()
{
	symbolRect.Top() = GetSymbolTop();
	symbolRect.Bottom() = symbolRect.Top() + getSumChildHeight();
	int indexMaxWidth = MAX( firstChild->GetRect().GetWidth(), secondChild->GetRect().GetWidth() );
	int piWidth = MAX( MIN( indexMaxWidth, symbolRect.GetHeight() ), symbolRect.GetHeight() / 2 );
	symbolRect.Left() = MAX( rect.Left() + indexMaxWidth / 2 - piWidth / 2, rect.Left() );
	symbolRect.Right() = symbolRect.Left() + piWidth;
}


void CProductControlModel::Resize()
{
	setRealChildPresentSumOrProduct();
	int width = MAX( firstChild->GetRect().GetWidth(), secondChild->GetRect().GetWidth() ) + 5 + productChild->GetRect().GetWidth();
	updateSymbolRect();
	int height = MAX( firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight() + 10 + symbolRect.GetHeight(),
		productChild->GetRect().GetHeight() );
	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

int CProductControlModel::getSumChildRectTop() const
{
	if( realChildPresentSum != nullptr ) {
		return symbolRect.Top() - realChildPresentSum->GetSymbolTop() + realChildPresentSum->GetRect().Top();
	} else if( realChildPresentProduct != nullptr ) {
		return symbolRect.Top() - realChildPresentProduct->GetSymbolTop() + realChildPresentProduct->GetRect().Top();
	}
	return symbolRect.Top();
}

int CProductControlModel::GetSymbolTop() const
{
	int top = rect.Top() + firstChild->GetRect().GetHeight() + 5;
	if( realChildPresentSum != nullptr ) {
		return MAX( top, rect.Top() + realChildPresentSum->GetSymbolTop() - realChildPresentSum->GetRect().Top() );
	} else if( realChildPresentProduct != nullptr ) {
		return MAX( top, rect.Top() + realChildPresentProduct->GetSymbolTop() - realChildPresentProduct->GetRect().Top() );
	}
	return top;
}

void CProductControlModel::PlaceChildren()
{
	CRect newRect;
	updateSymbolRect();
	int middleWidth = symbolRect.Left() + symbolRect.GetWidth() / 2;
	int middleHeight = rect.GetHeight() / 2;

	CRect oldRect = firstChild->GetRect();
	newRect.Bottom() = symbolRect.Top() - 5;
	newRect.Top() = newRect.Bottom() - oldRect.GetHeight();
	newRect.Left() = middleWidth - oldRect.GetWidth() / 2;
	newRect.Right() = middleWidth + oldRect.GetWidth() / 2;
	firstChild->SetRect( newRect );

	oldRect = secondChild->GetRect();
	newRect.Top() = symbolRect.Top() + symbolRect.GetHeight() + 5;
	newRect.Bottom() = newRect.Top() + oldRect.GetHeight();
	newRect.Left() = middleWidth - oldRect.GetWidth() / 2;
	newRect.Right() = middleWidth + oldRect.GetWidth() / 2;
	secondChild->SetRect( newRect );

	oldRect = productChild->GetRect();
	newRect.Top() = getSumChildRectTop();
	newRect.Bottom() = newRect.Top() + oldRect.GetHeight();
	newRect.Left() = MAX( symbolRect.Right(), MAX( firstChild->GetRect().Right(), secondChild->GetRect().Right() ) ) + 5;
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	productChild->SetRect( newRect );

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
	CRect indexRect = CRect( 0, 0, 0, getIndexHeight( rect.GetHeight() ) );
	CRect sumRect = CRect( 0, 0, 0, rect.GetHeight() );

	if ( initChild ) {
		productChild = initChild;
		productChild->SetParent( shared_from_this() );
		productChild->UpdateDepth();
	} else {
		productChild = std::make_shared<CExprControlModel>( sumRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
		productChild->InitializeChildren();
	}

	firstChild = std::make_shared<CExprControlModel>( indexRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	firstChild->InitializeChildren();

	secondChild = std::make_shared<CExprControlModel>( indexRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	secondChild->InitializeChildren();

	Resize();
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CProductControlModel::GetChildren() const
{
	return std::list < std::shared_ptr<IBaseExprModel> > { firstChild, secondChild, productChild };
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
	if (isInSelectionMode)
		params.isSelected = true;
	// Если пришли из родителя - идем в нижнего ребенка
	if ( from == parent.lock().get() ) {
		secondChild->MoveCaretLeft( this, caret, isInSelectionMode );
	} else if ( from == secondChild.get() ) {
		firstChild->MoveCaretLeft( this, caret, isInSelectionMode );
	} else if ( from == firstChild.get() ) {
		productChild->MoveCaretLeft( this, caret, isInSelectionMode );
	} else {
		// Иначе идем наверх
		parent.lock()->MoveCaretLeft( this, caret, isInSelectionMode );
	}
}

void CProductControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	if (isInSelectionMode)
		params.isSelected = true;
	// Если пришли из родителя - идем в верхнего ребенка
	if ( from == parent.lock().get() ) {
		firstChild->MoveCaretRight( this, caret, isInSelectionMode );
	} else if ( from == firstChild.get() ) {
		// Если пришли из верхнего - идем в нижнего
		secondChild->MoveCaretRight( this, caret, isInSelectionMode );
	} else if ( from == secondChild.get() ) {
		productChild->MoveCaretRight( this, caret, isInSelectionMode );
	} else {
		// Иначе идем наверх
		parent.lock()->MoveCaretRight( this, caret, isInSelectionMode );
	}
}

bool CProductControlModel::IsEmpty() const
{
	return firstChild->IsEmpty() && secondChild->IsEmpty() && productChild->IsEmpty();
}

void CProductControlModel::updatePolygons()
{
	params.polygon.clear();

	int piWidth = symbolRect.GetWidth();

	int piTop = symbolRect.Top();
	int piBottom = symbolRect.Bottom();
	int piLeft = symbolRect.Left() + piWidth / 5;
	int piRight = symbolRect.Right() - piWidth / 5;
	
	params.polygon.push_back( CLine( piLeft, piTop, piLeft, piBottom ) ); // левая палка
	params.polygon.push_back( CLine( piRight, piTop, piRight, piBottom ) ); // правая палка
	params.polygon.push_back( CLine( symbolRect.Left(), piTop, symbolRect.Left() + piWidth, piTop ) ); // верхняя перекладина
}

void CProductControlModel::UpdateSelection()
{
	if (!(firstChild->IsSelected()) || !(secondChild->IsSelected()) || !(productChild->IsSelected()))
		params.isSelected = false;
}

std::shared_ptr<IBaseExprModel> CProductControlModel::CopySelected() const
{
	std::shared_ptr<CProductControlModel> productModel( new CProductControlModel( rect, parent ) );
	std::shared_ptr<IBaseExprModel> firstModel = firstChild->CopySelected();
	std::shared_ptr<IBaseExprModel> secondModel = secondChild->CopySelected();
	std::shared_ptr<IBaseExprModel> productChildModel = productChild->CopySelected();

	productModel->firstChild = firstModel;
	productModel->firstChild->SetParent( productModel );

	productModel->secondChild = secondModel;
	productModel->secondChild->SetParent( productModel );

	productModel->productChild = productChildModel;
	productModel->productChild->SetParent( productModel );
	return productModel;
}
