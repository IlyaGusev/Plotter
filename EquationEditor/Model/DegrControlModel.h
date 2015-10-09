#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для степени
class CDegrControlModel : public IBaseExprModel {
public:
	CDegrControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent );

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;
	void InitializeChildren( );

	void Resize( );

	void PlaceChildren( );

	int GetMiddle() const;

	ViewType GetType( ) const;

	void MoveBy(int dx, int dy);
	void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );
	void MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );

	bool IsEmpty() const;

	bool IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const;
private:
	static const int MARGIN = 1;		// Отступ между показателем и основанием

	int getExponentHeight(int rectHeight);

	// Верхний ребенок
	std::shared_ptr<CExprControlModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<CExprControlModel> secondChild;
};