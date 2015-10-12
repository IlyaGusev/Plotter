#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для корня n-й степени
class CRadicalControlModel : public IBaseExprModel {
public:
	CRadicalControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent );

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;
	void InitializeChildren( std::shared_ptr<IBaseExprModel> initChild = 0 );

	void Resize( );

	void PlaceChildren( );

	int GetMiddle() const;

	void SetRect(const CRect& rect);

	ViewType GetType() const;
	
	void MoveBy( int dx, int dy );

	void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );
	void MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );

	bool IsEmpty() const;

	bool IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const;

	void UpdateSelection();

	std::shared_ptr<IBaseExprModel> CopySelected() const;
private:
	// Верхний ребенок
	std::shared_ptr<IBaseExprModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<IBaseExprModel> secondChild;

	void updatePolygons();
	int getDegreeHeight( int rectHeight );
};