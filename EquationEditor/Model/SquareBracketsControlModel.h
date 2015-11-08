#pragma once
#include "Model\IBracketsModel.h"

class CSquareBracketsControlModel : public IBracketsModel
{
public:
	CSquareBracketsControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent );
	void Resize();

	ViewType GetType() const;

	std::shared_ptr<IBaseExprModel> CopySelected() const;
	std::wstring Serialize();

private:
	void updatePolygons();

};

