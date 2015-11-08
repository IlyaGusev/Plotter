#include "Model/SystemControlModel.h"
#include "Model/EditControlModel.h"
#include <memory>

CSystemControlModel::CSystemControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent ) :
	IBaseExprModel(rect, parent)
{
	this->rect.Set( 0, 0, 0, rect.GetHeight() ); // нас интересует только высота, остальное исправится сразу же после инвалидации дерева
	depth = parent.lock()->GetDepth() + 1;
}

int CSystemControlModel::CalcHeight() const
{
  int height = 10;
  for (auto child : children) {
    height += child->GetRect().GetHeight();
  }
  height += MAX(0, (children.size() - 1)) * 5; // для промежутка между rect детей
  
  return height;
}

void CSystemControlModel::AddChild(int num, std::shared_ptr<IBaseExprModel> initChild)
{
  if (initChild != nullptr) {
    children.insert(children.begin() + num, initChild); // ахтунг, не испытывалось на практике!
    children[num]->SetParent(shared_from_this());
    children[num]->UpdateDepth();
  } else {
    CRect childRect = CRect(0, 0, 0, 20);
    children.insert(children.begin() + num, std::make_shared<CExprControlModel>(childRect, std::weak_ptr<IBaseExprModel>(shared_from_this())));
    children[num]->InitializeChildren();
  }
}

bool CSystemControlModel::CanRemoveChild(int line)
{
  return children[line]->IsEmpty();
}

std::shared_ptr<IBaseExprModel> CSystemControlModel::TryRemoveChild(int line)
{
  if (CanRemoveChild(line)) {
    children.erase(children.begin() + line);
    if (children.size() == 1) { // остался последний контрол -- убиваем систему, оставляем пустой контрол
      // перевешиваем ребенка
      auto exprParent = std::shared_ptr<CExprControlModel>(std::dynamic_pointer_cast<CExprControlModel>(GetParent().lock()));
      children[0]->SetParent(exprParent); 
      children[0]->UpdateDepth();
      exprParent->AddChildBefore(children[0], shared_from_this());
      
      // удаляем систему из детей своего родителя
      exprParent->RemoveChild(shared_from_this());
      return children[0];
    }
    return children[MAX(0, line - 1)];
  }

  return nullptr;
}

void CSystemControlModel::Resize()
{
  int width = 0;  
  for (auto child : GetChildren()) {
    width = MAX(child->GetRect().GetWidth(), width);
  }
  width += 15;

  int height = CalcHeight();

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

std::wstring CSystemControlModel::Serialize() {
	std::wstring result = L"<set>";
	for (auto child : GetChildren()) {
		result += L"<mrow>" + child->Serialize() + L"</mrow>";
	}

	return result + L"</set>";
}

int CSystemControlModel::FindLineNum(std::shared_ptr<CEditControlModel> edit)
{
  std::weak_ptr<IBaseExprModel> current = edit;
  while (current.lock() != nullptr && current.lock().get()->GetParent().lock().get()->GetType() != SYSTEM) {
    current = current.lock().get()->GetParent();
  }

  if (current.lock().get() != nullptr) { // если мы в системе, а это должно быть так
    return std::find(children.begin(), children.end(), current.lock()) - children.begin();
  }
}

void CSystemControlModel::PlaceChildren()
{
	CRect newRect;
  int currentTop = rect.Top() + 5;

  for (int i = 0; i < children.size(); ++i) {
    CRect oldRect = children[i]->GetRect();
    newRect.Top() = currentTop;
    currentTop += oldRect.GetHeight() + 5;
    newRect.Bottom() = newRect.Top() + oldRect.GetHeight();
    newRect.Left() = rect.Left() + 15;
    newRect.Right() = newRect.Left() + oldRect.GetWidth();
    children[i]->SetRect(newRect);
  }

	updatePolygons();
}

int CSystemControlModel::GetMiddle( ) const
{
	return CalcHeight() / 2;
}

void CSystemControlModel::InitializeChildren( std::shared_ptr<IBaseExprModel> initChild )
{
	CRect childRect = CRect( 0, 0, 0, rect.GetHeight( ) );
	if( initChild ) {
		children.push_back( initChild );
		children[0]->SetParent( shared_from_this() );
    children[0]->UpdateDepth();
	} else {
		children.push_back( std::make_shared<CExprControlModel>( childRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) ) );
    children[0]->InitializeChildren();
	}

  children.push_back( std::make_shared<CExprControlModel>( childRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) ) );
  children[1]->InitializeChildren();

	Resize();
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CSystemControlModel::GetChildren() const 
{
	return std::list<std::shared_ptr<IBaseExprModel>> ( children.begin(), children.end() );
}

void CSystemControlModel::SetRect( const CRect& rect ) 
{
	this->rect = rect;
	updatePolygons();
}

ViewType CSystemControlModel::GetType() const 
{
	return SYSTEM;
}

void CSystemControlModel::MoveBy( int dx, int dy ) 
{
	rect.MoveBy( dx, dy );
	updatePolygons();
}

void CSystemControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode ) 
{
	if (isInSelectionMode)
		params.isSelected = true;
	// Если пришли из родителя - идем в нижнего ребенка
  if (from == parent.lock().get()) {
    children[children.size() - 1]->MoveCaretLeft(this, caret, isInSelectionMode);
  } else {
    int childNum = -1;
    for (int i = 0; i < children.size(); ++i) {
      if (children[i].get() == from) {
        childNum = i;
      }
    }
    if (childNum > 0) {
      children[childNum - 1]->MoveCaretLeft(this, caret, isInSelectionMode);
    } else {
      // Иначе идем наверх
      parent.lock()->MoveCaretLeft(this, caret, isInSelectionMode);
    }
  }
}

void CSystemControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	if (isInSelectionMode)
		params.isSelected = true;
	// Если пришли из родителя - идем в верхнего ребенка
	if( from == parent.lock().get() ) {
    children[0]->MoveCaretRight( this, caret, isInSelectionMode );
  } else {
    int childNum = -1;
    for (int i = 0; i < children.size(); ++i) {
      if (children[i].get() == from) {
        childNum = i;
      }
    }
    if (childNum < children.size() - 1) {
      children[childNum + 1]->MoveCaretRight(this, caret, isInSelectionMode);
    } else {
      // Иначе идем наверх
      parent.lock()->MoveCaretRight(this, caret, isInSelectionMode);
    }
  }
}

bool CSystemControlModel::IsEmpty() const 
{
  for (auto child : children) {
    if (!child->IsEmpty()) {
      return false;
    }
  }
  return true;
}

void CSystemControlModel::updatePolygons()
{
  params.polygon.clear();

  int middle = rect.Top() + rect.GetHeight() / 2;
  params.polygon.push_back(CLine(rect.Left() + 7, rect.Top() + 5, rect.Left() + 7, middle - 3 )); // середина 1
  params.polygon.push_back(CLine(rect.Left() + 7, middle + 3, rect.Left() + 7, rect.Bottom() - 5)); // середина 2
  params.polygon.push_back(CLine(rect.Left() + 7, middle + 3, rect.Left() + 5, middle - 1)); // "носик" нижний
  params.polygon.push_back(CLine(rect.Left() + 7, middle - 3, rect.Left() + 5, middle + 1)); // "носик" верхний
  params.polygon.push_back(CLine(rect.Left() + 7, rect.Top() + 5, rect.Left() + 10, rect.Top() + 2)); // верх
  params.polygon.push_back(CLine(rect.Left() + 7, rect.Bottom() - 5, rect.Left() + 10, rect.Bottom() - 2)); // низ
}

void CSystemControlModel::UpdateSelection()
{
	params.isSelected = false;
  for (auto child : children) {
    if (!child->IsSelected()) {
      return;
    }
  }
  params.isSelected = true;
}

std::shared_ptr<IBaseExprModel> CSystemControlModel::CopySelected() const
{
	std::shared_ptr<CSystemControlModel> systemModel( new CSystemControlModel( rect, parent ) );

  for ( auto child : children ) {
    std::shared_ptr<IBaseExprModel> childModel = child->CopySelected();
    childModel->SetParent(systemModel);
    systemModel->children.push_back(childModel);
  }

	return systemModel;
}
