#include "Model/GeneralFunct.h"

void linkChildWithParent(std::shared_ptr<CExprControlModel> child, IBaseExprModel* parent) {
	child->SetParent(std::shared_ptr<IBaseExprModel>(parent));
	std::shared_ptr<CEditControlModel> childEdit(new CEditControlModel());

	child->AddChildAfter(childEdit, nullptr);
	childEdit->SetParent(child);
}