#include "Model/ExprControlModel.h"

CExprControlModel::CExprControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = -1;
	parent = nullptr;
}

IBaseExprModel* CExprControlModel::GetParent() const {
	return parent;
}

void CExprControlModel::SetParent( IBaseExprModel* newParent ) {
	parent = newParent;
}

std::list<IBaseExprModel*> CExprControlModel::GetChildren() const {
	return children;
}

RECT CExprControlModel::GetRect() const {
	return rect;
}

void CExprControlModel::SetRect( RECT newRect ) {
	rect = newRect;
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

CDrawParams CExprControlModel::GetDrawParams() const
{
	return CDrawParams( );
}

void CExprControlModel::AddChild( IBaseExprModel* child ) {
	children.push_back( child );
	if( rect.left != -1 ) {
		rect.left = min( rect.left, child->GetRect().left );
		rect.top = min( rect.top, child->GetRect().top );
		rect.right = max( rect.right, child->GetRect().right );
		rect.bottom = max( rect.bottom, child->GetRect().bottom );
	} else {
		rect.left = child->GetRect().left;
		rect.top = child->GetRect().top;
		rect.right = child->GetRect().right;
		rect.bottom = child->GetRect().bottom;
	}
}