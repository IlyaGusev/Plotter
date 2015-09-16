#pragma once
#include <Windows.h>

enum ViewType { EDIT_CONTROL, EXPR };

// Вьюшка, появляющаяся на экране, например, edit control или дробь
class IControlView {
public:
	virtual ~IControlView() = 0;

    // Производит изменение размера и перемещение вьюшки
    // Шрифт в edit control выставляется по высоте rect'а
    virtual void SetRect(RECT rect) = 0;

    // Возвращает связанный с вьюшкой HWND
    virtual HWND GetHWND() = 0;

    // Возвращает тип вьюшки
    virtual ViewType GetType() = 0;
};

inline IControlView::~IControlView() {}