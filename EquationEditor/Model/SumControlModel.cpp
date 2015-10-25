#include "Model/SumControlModel.h"
#include "Model/EditControlModel.h"

CSumControlModel::CSumControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent ) :
IBaseExprModel( rect, parent )
{
	this->rect.Set( 0, 0, 0, rect.GetHeight() ); // нас интересует только высота, остальное исправится сразу же после инвалидации дерева
	
	depth = parent.lock()->GetDepth() + 1;
}

void CSumControlModel::Resize()
{
	int width = MAX( firstChild->GetRect().GetWidth(), secondChild->GetRect().GetWidth() );
	int height = firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight() + 30; // добавим третьего ребенка (что под суммой) -- высота будет зависеть от него

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CSumControlModel::PlaceChildren()
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

int CSumControlModel::GetMiddle() const
{
	return ( firstChild->GetRect().GetHeight() + ( rect.GetHeight() - secondChild->GetRect().GetHeight() ) ) / 2;
}

void CSumControlModel::InitializeChildren( std::shared_ptr<IBaseExprModel> initChild )
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

std::list<std::shared_ptr<IBaseExprModel>> CSumControlModel::GetChildren() const
{
	return std::list < std::shared_ptr<IBaseExprModel> > { firstChild, secondChild };
}

void CSumControlModel::SetRect( const CRect& rect )
{
	this->rect = rect;
	updatePolygons();
}

ViewType CSumControlModel::GetType() const
{
	return SUM;
}

void CSumControlModel::MoveBy( int dx, int dy )
{
	rect.MoveBy( dx, dy );
	updatePolygons();
}

void CSumControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
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

void CSumControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
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

bool CSumControlModel::IsEmpty() const
{
	return firstChild->IsEmpty() && secondChild->IsEmpty();
}

// высота индексов
int CSumControlModel::getIndexHeight( int rectHeight )
{
	return max( rectHeight * 3 / 4, CEditControlModel::MINIMAL_HEIGHT );
}


void CSumControlModel::updatePolygons()
{
	params.polygon.clear();
	
	int sigmaTop = rect.Top() + firstChild->GetRect().GetHeight() + 5;
	int sigmaBottom = rect.Bottom() - secondChild->GetRect().GetHeight() - 5;
	int sigmaLeft = rect.Left();
	int sigmaRight = rect.Right();
	int sigmaCenterX = (sigmaLeft + sigmaRight) / 2;
	int sigmaCenterY = (sigmaTop + sigmaBottom) / 2;

	params.polygon.push_back(CLine(sigmaLeft, sigmaTop, sigmaRight, sigmaTop)); // верхняя перекладина
	params.polygon.push_back(CLine(sigmaLeft, sigmaTop, sigmaCenterX, sigmaCenterY)); // диагональ сверху в центр
	params.polygon.push_back(CLine(sigmaCenterX, sigmaCenterY, sigmaLeft, sigmaBottom)); // диагональ из центра вниз
	params.polygon.push_back(CLine(sigmaLeft, sigmaBottom, sigmaRight, sigmaBottom)); // нижняя перекладина
}

void CSumControlModel::UpdateSelection()
{
	params.isSelected = firstChild->IsSelected() && secondChild->IsSelected();
}

std::shared_ptr<IBaseExprModel> CSumControlModel::CopySelected() const
{
	std::shared_ptr<CSumControlModel> sumModel( new CSumControlModel( rect, parent ) );
	std::shared_ptr<IBaseExprModel> firstModel = firstChild->CopySelected();
	std::shared_ptr<IBaseExprModel> secondModel = secondChild->CopySelected();

	sumModel->firstChild = firstModel;
	sumModel->firstChild->SetParent( sumModel );

	sumModel->secondChild = secondModel;
	sumModel->secondChild->SetParent( sumModel );
	return sumModel;
}
