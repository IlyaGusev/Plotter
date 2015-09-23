#pragma once
#include "Model/IBaseExprModel.h"

// Модель холдера под вьюшки
class CExprControlModel : public IBaseExprModel {
public:
	CExprControlModel();
	~CExprControlModel() {}

	IBaseExprModel* GetParent() const;
	void SetParent( IBaseExprModel* parent );

	std::list<IBaseExprModel*> GetChildren() const;
	
	void AddChild( IBaseExprModel* child );

	RECT GetRect() const;
	void SetRect( RECT rect );

	void Resize();

	void PermutateChildren();

	CDrawParams GetDrawParams() const;
private:
	RECT rect;

	IBaseExprModel* parent;
	std::list<IBaseExprModel*> children;
};