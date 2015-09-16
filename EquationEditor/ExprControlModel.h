#pragma once
#include "IBaseExprModel.h"

// Модель текстового поля
class CExprControlModel : public IBaseExprModel {
public:
	CExprControlModel();
	~CExprControlModel() {}

	IBaseExprModel* GetParent();
	void SetParent( IBaseExprModel* parent );

	std::list<IBaseExprModel*> GetChilds();
	void AddChild( IBaseExprModel* child );

	RECT GetRect();

	void SetRect( RECT rect );
private:
	RECT rect;

	IBaseExprModel* parent;
	std::list<IBaseExprModel*> childs;
};