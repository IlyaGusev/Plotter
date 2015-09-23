#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"

// Модель для дроби
class CFracControlModel : public IBaseExprModel {
public:
	CFracControlModel();
	~CFracControlModel() {}

	IBaseExprModel* GetParent();
	void SetParent( IBaseExprModel* parent );

	std::list<IBaseExprModel*> GetChildren();

	RECT GetRect();

	void SetRect( RECT rect );

	CDrawParams GetDrawParams();
private:
	RECT rect;

	IBaseExprModel* parent;
	// Верхний ребенок
	CExprControlModel* firstChild;
	// Нижний ребенок
	CExprControlModel* secondChild;
};