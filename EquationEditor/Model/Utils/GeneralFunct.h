#pragma once
#include "Model/EditControlModel.h"
#include "Model/ExprControlModel.h"

//проставляем родителя для дочернего CExprControlModel и связываем CExprControlModel с соответствующим CEditControlModel
void linkChildWithParent(std::shared_ptr<CExprControlModel> child, IBaseExprModel* parent);