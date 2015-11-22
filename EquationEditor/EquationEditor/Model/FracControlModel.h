#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"

// Модель для дроби
// Держит на себе пару ExprControl'ов - числитель и знаменатель
class CFracControlModel : public IBaseExprModel {
public:
	CFracControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent );

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren() const;

	// Выставляем размеры вьюшек
	// Ширина дроби - 15 пикселей
	// Высота дроби - две высоты соседнего текстового поля + 5
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

	void UpdateSelection();

	std::shared_ptr<IBaseExprModel> CopySelected() const;
	std::wstring Serialize();
private:
	// Верхний ребенок
	std::shared_ptr<IBaseExprModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<IBaseExprModel> secondChild;

	void updatePolygons();
};