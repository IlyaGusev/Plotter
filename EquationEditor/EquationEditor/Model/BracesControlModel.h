#pragma once
#include "Model\IBracketsModel.h"

class CBracesControlModel : public IBracketsModel
{
public:
	CBracesControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent );

	ViewType GetType() const;

	std::shared_ptr<IBaseExprModel> CopySelected() const;
	std::wstring Serialize();

private:
	void updatePolygons();

};
