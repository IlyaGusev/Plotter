#pragma once
#include "Model/IBaseExprModel.h"

// Модель холдера под вьюшки
class CExprControlModel : public IBaseExprModel {
public:
	CExprControlModel();
	~CExprControlModel() {}

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( );
	
	void AddChild( std::shared_ptr<IBaseExprModel> child );

	ViewType GetType();
private:
	std::list<std::shared_ptr<IBaseExprModel>> children;
};