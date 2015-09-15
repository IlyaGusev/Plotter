#pragma once
#include <Windows.h>

// Вьюшка, появляющаяся на экране, например, edit control или дробь
class IControlView {
public:
    // Производит изменение размера и перемещение вьюшки
    // Шрифт в edit control выставляется по высоте rect'а
    virtual void SetRect(RECT rect) = 0;

    // Возвращает связанный с вьюшкой HWND
    virtual HWND GetHWND() = 0;
};