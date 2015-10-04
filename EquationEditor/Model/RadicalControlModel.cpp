#include "Model/RadicalControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/Utils/GeneralFunct.h"


#include <string>

CRadicalControlModel::CRadicalControlModel(CRect rect, std::weak_ptr<IBaseExprModel> parent) {
	this->parent = parent;
	this->rect = rect;
}

void CRadicalControlModel::Resize()
{
	int width = firstChild->GetRect().GetWidth() + secondChild->GetRect().GetWidth() + 5; //+5 для знака дроби
	int height = MAX(secondChild->GetRect().GetHeight(), firstChild->GetRect().GetHeight() + 5); // +5 для знака корня

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CRadicalControlModel::PlaceChildren()
{
	CRect newRect;

	CRect oldRect = firstChild->GetRect();
	newRect.Top() = rect.Top();
	newRect.Bottom() = rect.Bottom() - 5;
	newRect.Left() = rect.Left();
	newRect.Right() = rect.Left() + oldRect.GetWidth();
	firstChild->SetRect(newRect);
	
	oldRect = secondChild->GetRect();
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = rect.Bottom() - oldRect.GetHeight();
	newRect.Left() = firstChild->GetRect().Right() + 5;
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	secondChild->SetRect(newRect);

}

int CRadicalControlModel::GetMiddle() const
{
	return (secondChild->GetRect().Bottom() + secondChild->GetRect().Top()) / 2 - rect.Top();

}

void CRadicalControlModel::InitializeChildren()
{
	CRect childRect = CRect(0, 0, 5, rect.GetHeight());
	firstChild = std::make_shared<CExprControlModel>(CExprControlModel(childRect, std::weak_ptr<IBaseExprModel>(shared_from_this())));
	firstChild->InitializeChildren();

	secondChild = std::make_shared<CExprControlModel>(CExprControlModel(childRect, std::weak_ptr<IBaseExprModel>(shared_from_this())));
	secondChild->InitializeChildren();

	CRect newRect = CRect(rect.Left(), rect.Top(), rect.Left()+ 2*childRect.GetWidth() + 5, rect.Top() + childRect.GetHeight() + 5);
	SetRect(newRect);
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CRadicalControlModel::GetChildren() const {
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CRadicalControlModel::SetRect(CRect rect) {
	this->rect = rect;
}

ViewType CRadicalControlModel::GetType() const {
	return SUBSCRIPT;
}

void CRadicalControlModel::MoveBy(int dx, int dy) {
	rect.MoveBy(dx, dy);
	params.polygon.front().MoveBy(dx, dy);
}

void CRadicalControlModel::GoLeft(std::shared_ptr<const IBaseExprModel> from, CCaret& caret) const {
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

void CRadicalControlModel::GoRight(std::shared_ptr<const IBaseExprModel> from, CCaret& caret) const {
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

