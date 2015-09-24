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
	
	void AddChild( std::shared_ptr<IBaseExprModel> child );

	ViewType GetType() const;

private:
	std::list<std::shared_ptr<IBaseExprModel>> children;
};