#include "Model/ExprControlModel.h"
#include "Model/EditControlModel.h"

CExprControlModel::CExprControlModel( const CRect& rect, const std::weak_ptr<IBaseExprModel> parent ) :
	IBaseExprModel( rect, parent )
{
	middle = rect.GetHeight() / 2;
	if( !parent.expired() ) {
		depth = parent.lock()->GetDepth() + 1;
	}
}

void CExprControlModel::InitializeChildren( std::shared_ptr<IBaseExprModel> initChild /*= 0 */ )
{
	children.push_back( std::make_shared<CEditControlModel>( rect, shared_from_this() ) );
	if( initChild ) {
		children.push_back( initChild );
		children.back()->SetParent( shared_from_this() );
		children.back()->UpdateDepth();
		children.push_back( std::make_shared<CEditControlModel>( rect, shared_from_this() ) );
	}
	Resize();
	PlaceChildren();
}

void CExprControlModel::Resize()
{
	int width = 0;
	int aboveMiddle = 0;	// Расстояния от верха до середины
	int underMiddle = 0;	// Расстояние от низа до середины
	for( auto child : children ) {
		auto childRect = child->GetRect();
		width += childRect.GetWidth() + 2; // отступ 2 пикселя между соседними детьми внутри выражения

		aboveMiddle = MAX( aboveMiddle, child->GetMiddle() );
		underMiddle = MAX( underMiddle, childRect.GetHeight() - child->GetMiddle() );
	}
	width -= 2;
	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + aboveMiddle + underMiddle;
	middle = aboveMiddle;
}

void CExprControlModel::PlaceChildren()
{
	int currentX = rect.Left();

	CRect newRect;
	for (auto child : children) {
		CRect oldRect = child->GetRect();
		newRect.Left() = currentX;
		newRect.Right() = newRect.Left() + oldRect.GetWidth();
		newRect.Top() = rect.Top() + middle - child->GetMiddle();
		newRect.Bottom() = rect.Top() + middle + (oldRect.GetHeight() - child->GetMiddle());
		child->SetRect( newRect );

		currentX = newRect.Right() + 2; // отступ в 2 пикселя между дочерними моделями
	}
}

int CExprControlModel::GetMiddle( ) const
{
	return middle;
}

std::list<std::shared_ptr<IBaseExprModel>> CExprControlModel::GetChildren() const 
{
	return children;
}

void CExprControlModel::AddChildAfter( std::shared_ptr<IBaseExprModel> newChild, std::shared_ptr<IBaseExprModel> currentChild ) 
{
	auto currentChildIterator = std::find( children.begin(), children.end(), currentChild );
	if( currentChildIterator == children.end() ) {
		children.push_back( newChild );
	} else {
		++currentChildIterator;
		children.insert( currentChildIterator, newChild );
	}
}

void CExprControlModel::AddChildBefore( std::shared_ptr<IBaseExprModel> newChild, std::shared_ptr<IBaseExprModel> currentChild ) 
{
	auto currentChildIterator = std::find( children.begin(), children.end(), currentChild );
	if( currentChildIterator == children.begin() ) {
		children.push_front( newChild );
	} else {
		--currentChildIterator;
		children.insert( currentChildIterator, newChild );
	}
}

ViewType CExprControlModel::GetType() const
{
	return EXPR;
}

void CExprControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ ) 
{
	// from может быть одним из детей, тогда вставляем каретку в ребенка перед ним
	// Если это был самый левый ребенок - поднимаемся наверх
	if( from == children.front().get() ) {
		if( !parent.expired() ) {
			parent.lock()->MoveCaretLeft( this, caret );
		}
		return;
	}
	// Если он где-то посередине - we need to go deeper
	for( auto it = children.begin(); it != children.end(); ++it ) {
		if( (*it).get() == from ) {
			(*--it)->MoveCaretLeft( this, caret );
			return;
		}
	}
	// Иначе - он пришел извне, ставим каретку в самый конец
	children.back()->MoveCaretLeft( this, caret );
}

void CExprControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /* =false */ ) 
{
	// from может быть одним из детей, тогда вставляем каретку в ребенка перед ним
	// Если это был самый правый ребенок - поднимаемся наверх
	if( from == children.back().get() ) {
		if( !parent.expired() ) {
			parent.lock()->MoveCaretRight( this, caret, isInSelectionMode );
		}
		return;
	}
	// Если он где-то посередине - we need to go deeper
	for( auto it = children.begin(); it != children.end(); ++it ) {
		if( (*it).get() == from ) {
			(*++it)->MoveCaretRight( this, caret, isInSelectionMode );
			return;
		}
	}
	// Иначе - он пришел извне, ставим каретку в самое начало
	children.front()->MoveCaretRight( this, caret, isInSelectionMode );
}

bool CExprControlModel::IsEmpty() const 
{
	return children.empty() || children.size() == 1 && children.front()->IsEmpty();
}

bool CExprControlModel::IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const 
{
	int first, second;
	int i = 0;
	for( auto it = children.begin(); it != children.end(); ++it, ++i ) {
		if( (*it).get() == model1 ) {
			first = i;
		}
		if( (*it).get() == model2 ) {
			second = i;
		}
	}
	return first < second;
}

void CExprControlModel::UpdateSelection()
{
	for( std::shared_ptr<IBaseExprModel> child : children ) {
		if( !child->IsSelected() ) {
			params.isSelected = false;
			return;
		}
	}
	params.isSelected = true;
}

bool CExprControlModel::DeleteSelectedPart() 
{
	// Если уже пустой - ничего не трогаем
	if( IsEmpty() ) {
		return true;
	}

	for( auto it = children.begin(); it != children.end(); ++it ) {
		// Если нужно дополнительное вмешательство
		// Первым идет EditControl, его оставляем
		if( !(*it)->DeleteSelectedPart() && it != children.begin() && it != --children.end() ) {
			// Если пустой - удаляем
			if( (*it)->IsEmpty() ) {
				it = --children.erase( it );
			}
			//if( !(*it)->IsEmpty() ) {
			//	// Иначе переносим наверх все непустые контролы
			//	for( auto childExpr : (*it)->GetChildren() ) {
			//		if( !childExpr->IsEmpty() ) {
			//			for( auto child : childExpr->GetChildren() ) {
			//				children.insert( it, child );
			//				child->SetParent( shared_from_this() );
			//				child->UpdateDepth();
			//			}
			//		}
			//	}
			//}
			//it = --children.erase( it );
		}
	}
	// Если остался один пустой EditControl
	return !IsEmpty();
}

std::shared_ptr<IBaseExprModel> CExprControlModel::CopySelected() const
{
	std::shared_ptr<CExprControlModel> exprModel( new CExprControlModel( rect, parent ) );
	
	for( auto child : children ) {
		std::shared_ptr<IBaseExprModel> copy = child->CopySelected();
		if( copy != 0 && !copy->IsEmpty() ) {
			if( copy->GetType() == EXPR ) {
				for( std::shared_ptr<IBaseExprModel> child : copy->GetChildren() ) {
					exprModel->children.push_back( child );
					child->SetParent( exprModel );
				}
			} else {
				exprModel->children.push_back( copy );
				copy->SetParent( exprModel );
			}
		}
	}

	return exprModel;
}
