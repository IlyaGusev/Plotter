#include "Model/DegrControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/Utils/GeneralFunct.h"


#include <string>

CDegrControlModel::CDegrControlModel(CRect rect, std::weak_ptr<IBaseExprModel> parent) {
	this->parent = parent;
	this->rect = rect;
}

void CDegrControlModel::Resize( )
{
	int width = firstChild->GetRect().GetWidth() + secondChild->GetRect().GetWidth() + 5;
	int height = firstChild->GetRect().GetHeight() + secondChild->GetRect().GetHeight() - 15; // -15 для пересечения по высоте основания и показателя

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CDegrControlModel::PlaceChildren( )
{
	CRect newRect;
	int middle = (rect.Right() + rect.Left()) / 2;

	CRect oldRect = secondChild->GetRect();
	newRect.Bottom() = rect.Bottom();
	newRect.Top() = rect.Bottom() - oldRect.GetHeight();
	newRect.Left() = rect.Left();
	newRect.Right() = rect.Left() + oldRect.GetWidth();
	secondChild->SetRect(newRect);
	
	oldRect = firstChild->GetRect();
	newRect.Top() = rect.Top();
	newRect.Bottom() = rect.Top() + oldRect.GetHeight();
	newRect.Left() = secondChild->GetRect().Right();
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	firstChild->SetRect(newRect);

}

int CDegrControlModel::GetMiddle( ) const
{
	return (secondChild->GetRect().Bottom() + secondChild->GetRect().Top()) / 2 - rect.Top();

}

void CDegrControlModel::InitializeChildren()
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

std::list<std::shared_ptr<IBaseExprModel>> CDegrControlModel::GetChildren() const {
	return std::list<std::shared_ptr<IBaseExprModel>> { firstChild, secondChild };
}

void CDegrControlModel::SetRect(CRect rect) {
	this->rect = rect;
}

ViewType CDegrControlModel::GetType() const {
	return DEGR;
}

void CDegrControlModel::MoveBy(int dx, int dy) {
	rect.MoveBy(dx, dy);
	params.polygon.front().MoveBy(dx, dy);
}


//
//CDegrControlModel::CDegrControlModel(CRect rect, std::weak_ptr<IBaseExprModel> parent) {
//	this->parent = parent;
//	this->rect = rect;
//	//this->rect.Bottom() += rect.GetHeight();
//	//this->rect.Top() += rect.GetHeight();
//	//this->params.polygon.push_back(CLine(rect.Left(), rect.GetHeight(), rect.Right(), rect.GetHeight()));
//}
//
//void CDegrControlModel::Resize()
//{
//	int width = child->GetRect().GetWidth();
//	rect.Right() = rect.Left() + width;
//
//	int height = child->GetRect().GetHeight();
//	rect.Bottom() = MAX(rect.Bottom(), rect.Top() + height * 2);
//	
//}
//
//void CDegrControlModel::PlaceChildren()
//{
//	CRect newRect;
//	CRect oldRect = child->GetRect();
//	newRect.Top() = rect.Top();
//	newRect.Bottom() = rect.Top() + child->GetRect().GetHeight();
//	newRect.Left() = rect.Left();
//	newRect.Right() = rect.Right() + 15;
//	child->SetRect(newRect);
//
//}
//
//int CDegrControlModel::GetMiddle() const
//{
//	return child->GetRect().Bottom();
//}
//
//void CDegrControlModel::InitializeChildren() {
//	CRect childRect = CRect(0, 0, 15, rect.GetHeight());
//	child = std::make_shared<CExprControlModel>(CExprControlModel(childRect, std::weak_ptr<IBaseExprModel>(shared_from_this())));
//	child->InitializeChildren();
//	PlaceChildren();
//}
//
//std::list<std::shared_ptr<IBaseExprModel>> CDegrControlModel::GetChildren() const {
//	return std::list<std::shared_ptr<IBaseExprModel>> { child };
//}
//
//void CDegrControlModel::SetRect(CRect rect) {
//	this->rect = rect;
//	//params.polygon.front().Set(rect.Left(), rect.Top(), rect.Right(), rect.Top());
//	::OutputDebugString(std::to_wstring(GetRect().Top() + GetMiddle()).c_str());
//	::OutputDebugString((LPCWSTR) " ");
//	::OutputDebugString(std::to_wstring(child->GetRect().Bottom()).c_str());
//	::OutputDebugString((LPCWSTR) " ");
//	::OutputDebugString(std::to_wstring(child->GetRect().Top()).c_str());
//	::OutputDebugString((LPCWSTR) " ");
//	::OutputDebugString(std::to_wstring(rect.Top()).c_str());
//	::OutputDebugString((LPCWSTR) "\n");
//}
//
//ViewType CDegrControlModel::GetType() const {
//	return DEGR;
//}
//
//void CDegrControlModel::MoveBy(int dx, int dy) {
//	rect.MoveBy(dx, dy);
//	//params.polygon.front().MoveBy(dx, dy);
//}

void CDegrControlModel::GoLeft( std::shared_ptr<const IBaseExprModel> from, CCaret& caret ) const {

}

void CDegrControlModel::GoRight( std::shared_ptr<const IBaseExprModel> from, CCaret& caret ) const {

}
