#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для круглых скобок
class CParenthesesControlModel : public IBaseExprModel {
public:
	CParenthesesControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent );

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren() const;
	void InitializeChildren( std::shared_ptr<IBaseExprModel> initChild = 0 );

	void Resize();

	void PlaceChildren();

	int GetMiddle() const;

	void SetRect( const CRect& rect );

	ViewType GetType() const;

	void MoveBy( int dx, int dy );
	void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );
	void MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );

	void UpdateSelection();

	//bool IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const;

	bool IsEmpty() const;

	std::shared_ptr<IBaseExprModel> CopySelected() const;
private:
	void updatePolygons();

	// Содержимое скобок
	std::shared_ptr<IBaseExprModel> content;
};