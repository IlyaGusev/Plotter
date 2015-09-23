#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"

// Модель для дроби
class CFracControlModel : public IBaseExprModel {
public:
	CFracControlModel();
	~CFracControlModel() {}

	IBaseExprModel* GetParent() const;
	void SetParent( IBaseExprModel* parent );

	std::list<IBaseExprModel*> GetChildren() const;

	RECT GetRect() const;
	void SetRect( RECT rect );

	void Resize();

	void PermutateChildren();

	CDrawParams GetDrawParams() const;
private:
	RECT rect;

	IBaseExprModel* parent;
	// Верхний ребенок
	CExprControlModel* firstChild;
	// Нижний ребенок
	CExprControlModel* secondChild;
};