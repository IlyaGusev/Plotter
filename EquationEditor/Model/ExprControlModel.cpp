#include "Model/ExprControlModel.h"
#include "Model/EditControlModel.h"

CExprControlModel::CExprControlModel( CRect rect, const std::weak_ptr<IBaseExprModel> parent )
{
	this->rect = rect;
	this->parent = parent;
	middle = rect.GetHeight() / 2;
}

void CExprControlModel::InitializeChildren() 
{
	children.push_back( std::shared_ptr<CEditControlModel>( new CEditControlModel( rect, shared_from_this() ) ) );
}

void CExprControlModel::Resize()
{
	int width = 0;
	int aboveMiddle = 0;	// Расстояния от верха до середины
	int underMiddle = 0;	// Расстояние от низа до середины
	for( auto child : children ) {
		auto childRect = child->GetRect();
		width += childRect.GetWidth();

		aboveMiddle = MAX( aboveMiddle, child->GetMiddle() );
		underMiddle = MAX( underMiddle, childRect.GetHeight() - child->GetMiddle() );
	}
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

		currentX = newRect.Right();
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

void CExprControlModel::AddChildAfter( std::shared_ptr<IBaseExprModel> newChild, std::shared_ptr<IBaseExprModel> curChild ) 
{
	auto curChildIt = std::find( children.begin(), children.end(), curChild );
	if( curChildIt == children.end() ) {
		children.push_back( newChild );
	} else {
		++curChildIt;
		children.insert( curChildIt, newChild );
	}
	rect.Left() = MIN( rect.Left(), newChild->GetRect().Left() );
	rect.Top() = MIN( rect.Top(), newChild->GetRect().Top() );
	rect.Right() = MAX( rect.Right(), newChild->GetRect().Right() );
	rect.Bottom() = MAX( rect.Bottom(), newChild->GetRect().Bottom() );
}

ViewType CExprControlModel::GetType() const 
{
	return EXPR;
}
