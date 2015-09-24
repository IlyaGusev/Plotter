#pragma once
#include "EditControlModel.h"
#include "ExprControlModel.h"

//проставляем родителя для дочернего CExprControlModel и связываем CExprControlModel с соответствующим CEditControlModel
void linkChildWithParent(std::shared_ptr<CExprControlModel> child, IBaseExprModel* parent);