#include "Model\IBracketsModel.h"

IBracketsModel::IBracketsModel( CRect rect, std::weak_ptr<IBaseExprModel> parent ) :
	IBaseExprModel( rect, parent )
{
	depth = parent.lock()->GetDepth() + 1;
}


void IBracketsModel::PlaceChildren()
{
	CRect newRect;

	CRect oldRect = content->GetRect();
	newRect.Top() = rect.Top() + GetMiddle() - content->GetMiddle();
	newRect.Bottom() = newRect.Top() + oldRect.GetHeight();
	newRect.Left() = rect.Left() + 5;
	newRect.Right() = newRect.Left() + oldRect.GetWidth();
	content->SetRect( newRect );

	updatePolygons();
}

int IBracketsModel::GetMiddle() const
{
	return rect.GetHeight() / 2;
}

void IBracketsModel::InitializeChildren( std::shared_ptr<IBaseExprModel> initChild /*= 0 */ )
{
	if( initChild ) {
		content = initChild;
		content->SetParent( shared_from_this() );
		content->UpdateDepth();
	} else {
		CRect childRect = CRect( 0, 0, 0, rect.GetHeight() );
		content = std::make_shared<CExprControlModel>( childRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
		content->InitializeChildren( 0 );
	}

	Resize();
	PlaceChildren();
}

void IBracketsModel::Resize()
{
	// мидл висит на середине. размеры подстраиваются так, чтобы, вне зависимости от содержимого, снизу и свреху от центра было одинаковое расстояние
	int width = content->GetRect().GetWidth() + 10;
	int height = 2 * (MAX( content->GetMiddle(), content->GetRect().GetHeight() - content->GetMiddle() ) + 3);

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

std::list<std::shared_ptr<IBaseExprModel>> IBracketsModel::GetChildren() const
{
	return std::list<std::shared_ptr<IBaseExprModel>> { content };
}

void IBracketsModel::SetRect( const CRect& rect )
{
	this->rect = rect;
}

void IBracketsModel::MoveBy( int dx, int dy )
{
	rect.MoveBy( dx, dy );
	updatePolygons();
}

void IBracketsModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	if( isInSelectionMode )
		params.isSelected = true;
	// Если пришли из родителя - идем внутрь скобок
	if( from == parent.lock().get() ) {
		content->MoveCaretLeft( this, caret );
	} else {
		// Иначе идем наверх
		parent.lock()->MoveCaretLeft( this, caret );
	}
}

void IBracketsModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	if( isInSelectionMode )
		params.isSelected = true;
	// Если пришли из родителя - идем внутрь скобок
	if( from == parent.lock().get() ) {
		content->MoveCaretRight( this, caret );
	} else {
		// Иначе идем наверх
		parent.lock()->MoveCaretRight( this, caret );
	}
}

void IBracketsModel::UpdateSelection()
{
	if( !content->IsSelected() )
		params.isSelected = false;
}

bool IBracketsModel::IsEmpty() const
{
	return content->IsEmpty();
}
