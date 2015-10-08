#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для степени
class CDegrControlModel : public IBaseExprModel {
public:
	CDegrControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent ) :
		IBaseExprModel( rect, parent ) 
	{
	}

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;
	void InitializeChildren( );

	void Resize( );

	void PlaceChildren( );

	int GetMiddle() const;

	void SetRect( const CRect& rect );

	ViewType GetType( ) const;

	void MoveBy(int dx, int dy);
	void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret ) const;
	void MoveCaretRight( const IBaseExprModel* from, CCaret& caret ) const;

private:
	int getExponentHeight(int rectHeight);

	// Верхний ребенок
	std::shared_ptr<CExprControlModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<CExprControlModel> secondChild;
};