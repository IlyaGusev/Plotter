#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для круглых скобок
class CParenthesesControlModel : public IBaseExprModel {
public:
	CParenthesesControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent ) :
		IBaseExprModel( rect, parent )
	{
	}

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren() const;
	void InitializeChildren();

	void Resize();

	void PlaceChildren();

	int GetMiddle() const;

	void SetRect( const CRect& rect );

	ViewType GetType() const;

	void MoveBy( int dx, int dy );
	void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret ) const;
	void MoveCaretRight( const IBaseExprModel* from, CCaret& caret ) const;

private:
	void updatePolygons();

	// Содержимое скобок
	std::shared_ptr<CExprControlModel> content;
};