#pragma once
#include "IBaseExprModel.h"

// Модель для дроби
class CFracControlModel : public IBaseExprModel {
public:
	CFracControlModel();
	~CFracControlModel() {}

	IBaseExprModel* GetParent();
	void SetParent( IBaseExprModel* parent );

	std::list<IBaseExprModel*> GetChilds();

	void SetFirstChild( IBaseExprModel* child );
	void SetSecondChild( IBaseExprModel* child );

	RECT GetRect();

	void SetRect( RECT rect );
private:
	RECT rect;

	IBaseExprModel* parent;
	// Верхний ребенок
	IBaseExprModel* firstChild;
	// Нижний ребенок
	IBaseExprModel* secondChild;
};