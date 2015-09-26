#include "Model/ExprControlModel.h"

CExprControlModel::CExprControlModel() {
	parent = nullptr;
}

void CExprControlModel::Resize( )
{
	int width = 0;
	int height = 0;
	for (auto child : children)
	{
		auto childRect = child->Rect();
		width += childRect.Right() - childRect.Left();
		if (height < childRect.Bottom() - childRect.Top())
		{
			height = childRect.Bottom() - childRect.Top();
		}
	}
	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CExprControlModel::PermutateChildren()
{
	int currentX = rect.Left();
	int middleY = (rect.Bottom() + rect.Top()) / 2;

	CRect newRect;
	for (auto child : children)
	{
		CRect oldRect = child->Rect();
		newRect.Left() = currentX;
		newRect.Right() = newRect.Left() + oldRect.Right() - oldRect.Left();
		newRect.Top() = middleY - (oldRect.Bottom() - oldRect.Top()) / 2;
		newRect.Bottom() = middleY + (oldRect.Bottom() - oldRect.Top()) / 2;
		child->SetRect( newRect );

		currentX = newRect.Right();
	}
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
