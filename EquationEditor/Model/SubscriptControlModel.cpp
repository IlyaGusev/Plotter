#include "Model/SubscriptControlModel.h"
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
	return firstChild->GetMiddle();

}

void CSubscriptControlModel::InitializeChildren()
{
	CRect childRect = CRect(0, 0, 0, rect.GetHeight());
	firstChild = std::make_shared<CExprControlModel>(CExprControlModel(childRect, std::weak_ptr<IBaseExprModel>(shared_from_this())));
	firstChild->InitializeChildren();

	secondChild = std::make_shared<CExprControlModel>(CExprControlModel(childRect, std::weak_ptr<IBaseExprModel>(shared_from_this())));
	secondChild->InitializeChildren();

	Resize();
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CSubscriptControlModel::GetChildren() const {
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CSubscriptControlModel::SetRect(CRect rect) {
	this->rect = rect;
}

ViewType CSubscriptControlModel::GetType() const {
	return SUBSCRIPT;
}

void CSubscriptControlModel::MoveBy(int dx, int dy) {
	rect.MoveBy(dx, dy);
	params.polygon.front().MoveBy(dx, dy);
}

void CSubscriptControlModel::GoLeft(std::shared_ptr<const IBaseExprModel> from, CCaret& caret) const {
	// Если пришли из индекса - идём в основание
	if (from == secondChild) {
		firstChild->GoLeft(shared_from_this(), caret);
	}
	//если пришли из родителя - идём в индекс
	else if (from == parent.lock()) {
		secondChild->GoLeft(shared_from_this(), caret);
	}
	else {
		// Иначе идем наверх
		parent.lock()->GoLeft(shared_from_this(), caret);
	}
}

void CSubscriptControlModel::GoRight(std::shared_ptr<const IBaseExprModel> from, CCaret& caret) const {
	// Если пришли из родителя - идем в основание
	if (from == parent.lock()) {
		firstChild->GoRight(shared_from_this(), caret);
	}
	//если из основания - в индекс
	else if (from == firstChild) {
		secondChild->GoRight(shared_from_this(), caret);
	}
	else {
		// Иначе идем наверх
		parent.lock()->GoRight(shared_from_this(), caret);
	}
}

