#pragma once
#include "IBaseExprModel.h"

// Модель текстового поля
class CEditControlModel : public IBaseExprModel {
public:
	CEditControlModel();
	~CEditControlModel() {}

	IBaseExprModel* GetParent( );
	void SetParent( IBaseExprModel* parent );

	std::list< IBaseExprModel* > GetChilds( );

	RECT GetRect();
	void SetRect( RECT rect );
private:
	RECT rect;

	IBaseExprModel* parent;
};