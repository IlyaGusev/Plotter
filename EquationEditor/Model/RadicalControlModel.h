#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для корня n-й степени
class CRadicalControlModel : public IBaseExprModel {
public:
	CRadicalControlModel(CRect rect, std::weak_ptr<IBaseExprModel> parent); 

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;
	void InitializeChildren( );

	void Resize( );

	void PlaceChildren( );

	int GetMiddle() const;

	void SetRect(const CRect& rect);

	ViewType GetType( ) const;
	
	void MoveBy(int dx, int dy);

	void MoveCaretLeft(const IBaseExprModel* from, CCaret& caret) const;

	void MoveCaretRight(const IBaseExprModel* from, CCaret& caret) const;

	int getDegreeHeight( int rectHeight );
private:
	// Верхний ребенок
	std::shared_ptr<CExprControlModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<CExprControlModel> secondChild;

	void updatePolygons();
};