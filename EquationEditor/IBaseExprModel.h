#pragma once
#include <Windows.h>
#include <list>
#include <memory>

// Модель элемента выражения
class IBaseExprModel {
public:
    virtual ~IBaseExprModel() = 0;

	virtual IBaseExprModel* GetParent( ) = 0;
	virtual void SetParent( IBaseExprModel* parent ) = 0;

	virtual std::list<IBaseExprModel*> GetChilds() = 0;
	virtual void AddChild( IBaseExprModel* child ) = 0;

    virtual RECT GetRect() = 0;
    virtual void SetRect(RECT rect) = 0;
};

inline IBaseExprModel::~IBaseExprModel() {}