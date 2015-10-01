#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для степени
class CDegrControlModel : public IBaseExprModel {
public:
	CDegrControlModel(CRect rect, std::weak_ptr<IBaseExprModel> parent);
	~CDegrControlModel()
	{
	}

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const;
	void InitializeChildren( );

	void Resize( );

	void PlaceChildren( );

	int GetMiddle() const;

	void SetRect(CRect rect);

	ViewType GetType( ) const;
	
	void MoveBy(int dx, int dy);

private:
	// Показатель
	std::shared_ptr<CExprControlModel> child;
};