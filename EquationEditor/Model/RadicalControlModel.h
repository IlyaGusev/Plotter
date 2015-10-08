#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для корня n-й степени
class CRadicalControlModel : public IBaseExprModel {
public:
	CRadicalControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent );

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;
	void InitializeChildren( );

	void Resize( );

	void PlaceChildren( );

	int GetMiddle() const;

	void SetRect(const CRect& rect);

	ViewType GetType( ) const;
	
	void MoveBy(int dx, int dy);

	void UpdateSelection( const CRect& selectionRect ) {}

	void MoveCaretLeft(const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false);

	void MoveCaretRight(const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode=false);

	bool IsEmpty() const;
private:
	// Верхний ребенок
	std::shared_ptr<CExprControlModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<CExprControlModel> secondChild;

	void updatePolygons();
	int getDegreeHeight( int rectHeight );
};