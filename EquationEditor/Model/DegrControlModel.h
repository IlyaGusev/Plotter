#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"

// попытка индент. dasha
// Модель для степени
class CDegrControlModel : public IBaseExprModel {
public:
	CDegrControlModel();
	~CDegrControlModel() {}

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren();

	ViewType GetType();
private:
	RECT rect;

	std::shared_ptr<IBaseExprModel> parent;

	// Показатель
	std::shared_ptr<CExprControlModel> сhild;
};