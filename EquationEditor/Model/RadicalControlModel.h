#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"


// Модель для степени
class CRadicalControlModel : public IBaseExprModel {
public:
	CRadicalControlModel(CRect rect, std::weak_ptr<IBaseExprModel> parent);
	~CRadicalControlModel()
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

	void GoLeft(std::shared_ptr<const IBaseExprModel> from, CCaret& caret) const;

	void GoRight(std::shared_ptr<const IBaseExprModel> from, CCaret& caret) const;

private:
	// Верхний ребенок
	std::shared_ptr<CExprControlModel> firstChild;
	// Нижний ребенок
	std::shared_ptr<CExprControlModel> secondChild;
};