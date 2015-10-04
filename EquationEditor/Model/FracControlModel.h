#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"

// Модель для дроби
class CFracControlModel : public IBaseExprModel {
public:
	CFracControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent );
	~CFracControlModel() {}

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren() const;

	// Выставляем размеры вьюшек
	// Ширина дроби - 15 пикселей
	// Высота дроби - две высоты соседнего текстового поля + 5
	void InitializeChildren();

	virtual void SetRect( CRect rect );

	void Resize();

	void PlaceChildren();

	int GetMiddle() const;

	ViewType GetType() const;

	void MoveBy( int dx, int dy );

	void GoLeft( std::shared_ptr<const IBaseExprModel> from, CCaret& caret ) const;
	void GoRight( std::shared_ptr<const IBaseExprModel> from, CCaret& caret ) const;
private:
	// Верхний ребенок
	std::shared_ptr<CExprControlModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<CExprControlModel> secondChild;
};