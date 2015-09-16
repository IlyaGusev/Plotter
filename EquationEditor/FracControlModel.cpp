#include "FracControlModel.h"

CFracControlModel::CFracControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = 0;
	parent = firstChild = secondChild = nullptr;
}

IBaseExprModel* CFracControlModel::GetParent() {
	return parent;
}

void CFracControlModel::SetParent( IBaseExprModel* newParent ) {
	parent = newParent;
}

std::list<IBaseExprModel*> CFracControlModel::GetChilds() {
	return std::list<IBaseExprModel*> { firstChild, secondChild };
}

void CFracControlModel::SetFirstChild( IBaseExprModel* child ) {
	firstChild = child;
}

void CFracControlModel::SetSecondChild( IBaseExprModel* child ) {
	secondChild = child;
}

RECT CFracControlModel::GetRect() {
	return rect;
}

void CFracControlModel::SetRect( RECT newRect ) {
	rect = newRect;
}