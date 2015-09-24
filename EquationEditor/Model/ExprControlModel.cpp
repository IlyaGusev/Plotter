#include "Model/ExprControlModel.h"

CExprControlModel::CExprControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = -1;
	parent = nullptr;
}

void CExprControlModel::Resize( )
{
	int width = 0;
	int height = 0;
	for (auto child : children)
	{
		auto childRect = child->GetRect();
		width += childRect.right - childRect.left;
		if (height < childRect.bottom - childRect.top)
		{
			height = childRect.bottom - childRect.top;
		}
	}
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
}

void CExprControlModel::PermutateChildren( )
{
	auto moveRect = []( RECT rect, int dx, int dy )
	{
		rect.right += dx;
		rect.left += dx;
		rect.top += dy;
		rect.bottom += dy;
	};

	int currentX = rect.left;
	int middleY = (rect.bottom + rect.top) / 2;

	RECT newRect;
	for (auto child : children)
	{
		RECT oldRect = child->GetRect();
		newRect.left = currentX;
		newRect.right = newRect.left + oldRect.right - oldRect.left;
		newRect.top = middleY - (oldRect.bottom - oldRect.top) / 2;
		newRect.bottom = middleY + (oldRect.bottom - oldRect.top) / 2;
		child->SetRect( newRect );

		currentX = newRect.right;
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
	rect.left = min( rect.left, newChild->GetRect().left );
	rect.top = min( rect.top, newChild->GetRect().top );
	rect.right = max( rect.right, newChild->GetRect().right );
	rect.bottom = max( rect.bottom, newChild->GetRect().bottom );
}

ViewType CExprControlModel::GetType() const {
	return EXPR;
}
