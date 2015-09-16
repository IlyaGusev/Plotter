#pragma once
#include "IControlView.h"

class CEditControlView : public IControlView 
{
public:
    CEditControlView(HWND editControl);
	~CEditControlView() {}

    void SetRect(RECT rect);

    HWND GetHWND();

    ViewType GetType();
private:
    HWND hwndEditControl;
};