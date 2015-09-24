#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"

// Модель для дроби
class CFracControlModel : public IBaseExprModel {
public:
	CFracControlModel();
	~CFracControlModel() {}

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;

	virtual void SetRect( RECT rect );

	void Resize();

	void PermutateChildren();

	ViewType GetType() const;

private:
	// Верхний ребенок
	std::shared_ptr<CExprControlModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<CExprControlModel> secondChild;
};