#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для степени
class CDegrControlModel : public IBaseExprModel {
public:
	CDegrControlModel( CRect rect );
	~CDegrControlModel( )
	{
	}

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;
	void InitializeChildren() {}

	void Resize( );

	void PlaceChildren( );

	int GetMiddle() const;

	ViewType GetType( ) const;
private:
	// Показатель
	std::shared_ptr<CExprControlModel> child;
};