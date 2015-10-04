﻿#include "Model/SubscriptControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/Utils/GeneralFunct.h"


#include <string>

CSubscriptControlModel::CSubscriptControlModel(CRect rect, std::weak_ptr<IBaseExprModel> parent) {
	this->parent = parent;
	this->rect = rect;
}

void CSubscriptControlModel::Resize()
{
	int width = firstChild->GetRect().GetWidth() + secondChild->GetRect().GetWidth() + 5;
	int height = firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight() - 15; // -15 для пересечения по высоте основания и показателя

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CSubscriptControlModel::PlaceChildren()
{
	CRect newRect;
	int middle = (rect.Right() + rect.Left()) / 2;

	CRect oldRect = firstChild->GetRect();
	newRect.Top() = rect.Top();
	newRect.Bottom() = rect.Top() + oldRect.GetHeight();
	newRect.Left() = rect.Left();
	newRect.Right() = rect.Left() + oldRect.GetWidth();
	firstChild->SetRect(newRect);
	
	oldRect = secondChild->GetRect();
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = rect.Bottom() - oldRect.GetHeight();
	newRect.Left() = firstChild->GetRect().Right();
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	secondChild->SetRect(newRect);

}

int CSubscriptControlModel::GetMiddle() const
{
	return (firstChild->GetRect().Bottom() + firstChild->GetRect().Top()) / 2 - rect.Top();

}

void CSubscriptControlModel::InitializeChildren()
{
	CRect childRect = CRect(0, 0, 5, rect.GetHeight());
	firstChild = std::make_shared<CExprControlModel>(CExprControlModel(childRect, std::weak_ptr<IBaseExprModel>(shared_from_this())));
	firstChild->InitializeChildren();

	secondChild = std::make_shared<CExprControlModel>(CExprControlModel(childRect, std::weak_ptr<IBaseExprModel>(shared_from_this())));
	secondChild->InitializeChildren();

	CRect newRect = CRect(rect.Left(), rect.Top(), rect.Left()+ 2*childRect.GetWidth() + 5, rect.Top() + 2 * childRect.GetHeight() - 15);
	SetRect(newRect);
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CSubscriptControlModel::GetChildren() const {
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CSubscriptControlModel::SetRect(CRect rect) {
	this->rect = rect;
}

ViewType CSubscriptControlModel::GetType() const {
	return DEGR;
}

void CSubscriptControlModel::MoveBy(int dx, int dy) {
	rect.MoveBy(dx, dy);
	params.polygon.front().MoveBy(dx, dy);
}

