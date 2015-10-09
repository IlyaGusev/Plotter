#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для степени
class CSubscriptControlModel : public IBaseExprModel {
public:
	CSubscriptControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent );

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;
	void InitializeChildren( );

	void Resize( );

	void PlaceChildren( );

	int GetMiddle() const;

	ViewType GetType( ) const;
	
	void MoveBy(int dx, int dy);

	void MoveCaretLeft(const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false);

	void MoveCaretRight(const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false);

	bool HasInverseDirection() const;

	bool IsEmpty() const;
private:
	int getSubscriptHeight( int rectHeight );

	// Верхний ребенок
	std::shared_ptr<CExprControlModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<CExprControlModel> secondChild;
};