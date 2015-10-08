#include "Model/ExprControlModel.h"
#include "Model/EditControlModel.h"

CExprControlModel::CExprControlModel( CRect rect, const std::weak_ptr<IBaseExprModel> parent ) :
	IBaseExprModel( rect, parent )
{
	middle = rect.GetHeight() / 2;
}

void CExprControlModel::InitializeChildren() 
{
	children.push_back( std::make_shared<CEditControlModel>( rect, shared_from_this() ) );
	Resize();
	PlaceChildren();
}

void CExprControlModel::Resize()
{
	int width = 0;
	int aboveMiddle = 0;	// Расстояния от верха до середины
	int underMiddle = 0;	// Расстояние от низа до середины
	for( auto child : children ) {
		auto childRect = child->GetRect();
		width += childRect.GetWidth() + 2; // отступ 2 пикселя между соседними детьми внутри выражения

		aboveMiddle = MAX( aboveMiddle, child->GetMiddle() );
		underMiddle = MAX( underMiddle, childRect.GetHeight() - child->GetMiddle() );
	}
	width -= 2;
	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + aboveMiddle + underMiddle;
	middle = aboveMiddle;
}

void CExprControlModel::PlaceChildren()
{
	int currentX = rect.Left();

	CRect newRect;
	for (auto child : children) {
		CRect oldRect = child->GetRect();
		newRect.Left() = currentX;
		newRect.Right() = newRect.Left() + oldRect.GetWidth();
		newRect.Top() = rect.Top() + middle - child->GetMiddle();
		newRect.Bottom() = rect.Top() + middle + (oldRect.GetHeight() - child->GetMiddle());
		child->SetRect( newRect );

		currentX = newRect.Right() + 2; // отступ в 2 пикселя между дочерними моделями
	}
}

int CExprControlModel::GetMiddle( ) const
{
	return middle;
}

std::list<std::shared_ptr<IBaseExprModel>> CExprControlModel::GetChildren() const 
{
	return children;
}

void CExprControlModel::AddChildAfter( std::shared_ptr<IBaseExprModel> newChild, std::shared_ptr<IBaseExprModel> currentChild ) 
{
	auto currentChildIterator = std::find( children.begin(), children.end(), currentChild );
	if( currentChildIterator == children.end() ) {
		children.push_back( newChild );
	} else {
		++currentChildIterator;
		children.insert( currentChildIterator, newChild );
	}
}

ViewType CExprControlModel::GetType() const 
{
	return EXPR;
}

void CExprControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret ) const {
	// from может быть одним из детей, тогда вставляем каретку в ребенка перед ним
	// Если это был самый левый ребенок - поднимаемся наверх
	if( from == children.front().get() ) {
		if( !parent.expired() ) {
			parent.lock()->MoveCaretLeft( this, caret );
		}
		return;
	}
	// Если он где-то посередине - we need to go deeper
	for( auto it = children.begin(); it != children.end(); ++it ) {
		if( (*it).get() == from ) {
			(*--it)->MoveCaretLeft( this, caret );
			return;
		}
	}
	// Иначе - он пришел извне, ставим каретку в самый конец
	children.back()->MoveCaretLeft( this, caret );
}

void CExprControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret ) const {
	// from может быть одним из детей, тогда вставляем каретку в ребенка перед ним
	// Если это был самый правый ребенок - поднимаемся наверх
	if( from == children.back().get() ) {
		if( !parent.expired() ) {
			parent.lock()->MoveCaretRight( this, caret );
		}
		return;
	}
	// Если он где-то посередине - we need to go deeper
	for( auto it = children.begin(); it != children.end(); ++it ) {
		if( (*it).get() == from ) {
			(*++it)->MoveCaretRight( this, caret );
			return;
		}
	}
	// Иначе - он пришел извне, ставим каретку в самое начало
	children.front()->MoveCaretRight( this, caret );
}
