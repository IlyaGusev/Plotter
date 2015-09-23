#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"

// Модель для степени
class CDegrControlModel : public IBaseExprModel {
public:
	CDegrControlModel();
	~CDegrControlModel() {}

	IBaseExprModel* GetParent();
	void SetParent(IBaseExprModel* parent);

	std::list<IBaseExprModel*> GetChildren();

	RECT GetRect();

	void SetRect(RECT rect);

	CDrawParams GetDrawParams();
private:
	RECT rect;

	IBaseExprModel* parent;

	// Показатель
	CExprControlModel* сhild;
};