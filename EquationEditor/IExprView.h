#pragma once
#include <Windows.h>

// Отвечает за отрисовку знаков в выражении, например, дроби или корня 
class IExprView {
    virtual void ResizeView(RECT rect) = 0;
};