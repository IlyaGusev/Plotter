#pragma once
#include "Model/IBaseExprModel.h"

// Модель холдера под вьюшки
class CExprControlModel : public IBaseExprModel {
public:
	CExprControlModel();
	~CExprControlModel() {}

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;

	void Resize( );

	void PermutateChildren( );
	
	// Вставляет нового ребенка после curChild
	// Если curChild == nullptr, вставляет в конец списка
	void AddChildAfter( std::shared_ptr<IBaseExprModel> newChild, std::shared_ptr<IBaseExprModel> curChild );

	ViewType GetType() const;

private:
	std::list<std::shared_ptr<IBaseExprModel>> children;
};