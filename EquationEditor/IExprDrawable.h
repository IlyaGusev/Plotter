#pragma once
#include <Windows.h>

// Отвечает за отрисовку знаков в выражении, например, дроби или корня 
class IExprDrawable {
    virtual ~IExprDrawable() = 0;

    virtual void ResizeView(RECT rect) = 0;
};