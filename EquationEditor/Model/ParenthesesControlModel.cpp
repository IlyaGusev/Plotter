#include "Model/ParenthesesControlModel.h"

#include <string>

CParenthesesControlModel::CParenthesesControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent ) :
	IBaseExprModel( rect, parent )
{
	depth = parent.lock( )->GetDepth( ) + 1;
}

void CParenthesesControlModel::Resize()
{
	// мидл висит на середние. размеры подстраиваютс€ так, чтобы, вне зависимости от содержимого, снизу и свреху от центра было одинаковое рассто€ние
	int width = content->GetRect().GetWidth() + 10;
	int height = 2 * (MAX( content->GetMiddle(), content->GetRect().GetHeight() - content->GetMiddle() ) + 3);

	rect.Right() = rect.Left() + width;
	rect.Bottom() = rect.Top() + height;
}

void CParenthesesControlModel::PlaceChildren()
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

int CParenthesesControlModel::GetMiddle( ) const
{
	return rect.GetHeight() / 2;
}

void CParenthesesControlModel::InitializeChildren()
{
	CRect childRect = CRect( 0, 0, 0, rect.GetHeight() );
	content = std::make_shared<CExprControlModel>( childRect, std::weak_ptr<IBaseExprModel>( shared_from_this() ) );
	content->InitializeChildren();

	Resize();
	PlaceChildren();
}

std::list<std::shared_ptr<IBaseExprModel>> CParenthesesControlModel::GetChildren( ) const
{
	return std::list<std::shared_ptr<IBaseExprModel>> { content };
}

void CParenthesesControlModel::SetRect( const CRect& rect )
{
	this->rect = rect;
}

ViewType CParenthesesControlModel::GetType( ) const
{
	return PARENTHESES;
}

void CParenthesesControlModel::MoveBy( int dx, int dy )
{
	rect.MoveBy( dx, dy );
	updatePolygons();
}

void CParenthesesControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	// ≈сли пришли из родител€ - идем внутрь скобок
	if( from == parent.lock().get() ) {
		content->MoveCaretLeft( this, caret );
	}
	else {
		// »наче идем наверх
		parent.lock()->MoveCaretLeft( this, caret );
	}
}

void CParenthesesControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ )
{
	// ≈сли пришли из родител€ - идем внутрь скобок
	if( from == parent.lock().get() ) {
		content->MoveCaretRight( this, caret );
	}
	else {
		// »наче идем наверх
		parent.lock()->MoveCaretRight( this, caret );
	}
}

void CParenthesesControlModel::UpdateSelection()
{
	params.isSelected = content->IsSelected();
}

bool CParenthesesControlModel::IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const
{
	return model1 == content.get( );
}

bool CParenthesesControlModel::IsEmpty() const
{
	return content->IsEmpty();
}

void CParenthesesControlModel::updatePolygons()
{
	params.polygon.clear();
	// лева€ скобка
	params.polygon.push_back( CLine( rect.Left() + 2, rect.Top() + 5, rect.Left() + 2, rect.Bottom() - 5 ) ); // середина
	params.polygon.push_back( CLine( rect.Left() + 2, rect.Top() + 5, rect.Left() + 5, rect.Top() + 2 ) ); // верх
	params.polygon.push_back( CLine( rect.Left() + 2, rect.Bottom() - 5, rect.Left() + 5, rect.Bottom() - 2 ) ); // низ
	// права€ скобка
	params.polygon.push_back( CLine( rect.Right() - 2, rect.Top() + 5, rect.Right() - 2, rect.Bottom() - 5 ) ); // середина
	params.polygon.push_back( CLine( rect.Right() - 2, rect.Top() + 5, rect.Right() - 5, rect.Top() + 2 ) ); // верх
	params.polygon.push_back( CLine( rect.Right() - 2, rect.Bottom() - 5, rect.Right() - 5, rect.Bottom() - 2 ) ); // низ
}
