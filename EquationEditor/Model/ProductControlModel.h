#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"

class CSumControlModel;

// Модель для произведения
// Держит на себе пару ExprControl'ов - верхний и нижний индексы
class CProductControlModel : public IBaseExprModel {
public:
	CProductControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent );

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren() const;

	// Выставляем размеры вьюшек
	void InitializeChildren( std::shared_ptr<IBaseExprModel> initChild = 0 );

	void SetRect( const CRect& rect );

	void Resize();

	void PlaceChildren();

	int GetMiddle() const;

	ViewType GetType() const;

	void MoveBy( int dx, int dy );

	void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );
	void MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );

	bool IsEmpty() const;
	int GetSymbolHeight() const;
	int GetSymbolTop() const;

	void UpdateSelection();

	std::shared_ptr<IBaseExprModel> CopySelected() const;
	std::wstring Serialize();
private:
	// Верхний ребенок
	std::shared_ptr<IBaseExprModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<IBaseExprModel> secondChild;
	// Ребенок под произведением
	std::shared_ptr<IBaseExprModel> productChild;

	CRect symbolRect;

	std::shared_ptr<CSumControlModel> realChildPresentSum;
	std::shared_ptr<CProductControlModel> realChildPresentProduct;

	void updatePolygons();
	int getIndexHeight( int rectHeight );
	int getSumChildRectTop() const;
	void updateSymbolRect();
	int getSumChildHeight() const;
	void setRealChildPresentSumOrProduct();
};