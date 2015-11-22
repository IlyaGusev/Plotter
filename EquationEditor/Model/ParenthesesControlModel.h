#pragma once
#include "Model/IBracketsModel.h"
//#include "Model/ExprControlModel.h"


// Модель для круглых скобок
class CParenthesesControlModel : public IBracketsModel {
public:
	CParenthesesControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent );

	ViewType GetType() const;

	std::shared_ptr<IBaseExprModel> CopySelected() const;
	std::wstring Serialize();

private:
	void updatePolygons();

};