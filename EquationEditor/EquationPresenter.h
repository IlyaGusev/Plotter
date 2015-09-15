#pragma once
#include "IControlView.h"
#include <vector>

// Класс, размещающий прямоугольники вьюшек на экране
class CEquationPresenter
{
public:
    CEquationPresenter();

    void AddControlView(IControlView* view, HWND parentViewHwnd);

    IControlView* FindControlView(HWND hwnd);
};