#include "Model/ExprControlModel.h"

CExprControlModel::CExprControlModel()
{
	middle = 10; // костыль. нужно сделать по-нормальному, а то чёт не очень :(
	parent = nullptr;
}

void CExprControlModel::Resize( )
{
	int width = 0;
	int aboveMiddle = 0;
	int underMiddle = 0;
	for( auto child : children ) {
		auto childRect = child->Rect();
		width += childRect.GetWidth();

		aboveMiddle = MAX( aboveMiddle, child->GetMiddle() );
		underMiddle = MAX( underMiddle, childRect.GetHeight() - child->GetMiddle() );
	}
	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + aboveMiddle + underMiddle;
	middle = aboveMiddle;
}

void CExprControlModel::PermutateChildren()
{
	int currentX = rect.Left();

	CRect newRect;
	for (auto child : children)
	{
		CRect oldRect = child->Rect();
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

std::list<std::shared_ptr<IBaseExprModel>> CExprControlModel::GetChildren() const {
	return children;
}

void CExprControlModel::AddChildAfter( std::shared_ptr<IBaseExprModel> newChild, std::shared_ptr<IBaseExprModel> curChild ) {
	auto curChildIt = std::find( children.begin(), children.end(), curChild );
	if( curChildIt == children.end() ) {
		children.push_back( newChild );
	} else {
		++curChildIt;
		children.insert( curChildIt, newChild );
	}
	rect.Left() = MIN( rect.Left(), newChild->Rect().Left() );
	rect.Top() = MIN( rect.Top(), newChild->Rect().Top() );
	rect.Right() = MAX( rect.Right(), newChild->Rect().Right() );
	rect.Bottom() = MAX( rect.Bottom(), newChild->Rect().Bottom() );
}

ViewType CExprControlModel::GetType() const {
	return EXPR;
}
