#pragma once
#include <Windows.h>
#include <list>

// Модель элемента выражения
class IBaseExprModel {
public:
    virtual IBaseExprModel* GetParent() = 0;
    virtual std::list<IBaseExprModel*> GetChilds() = 0;
    virtual RECT GetRect() = 0;
    virtual void SetRect(RECT rect) = 0;
};