#include "EditControlView.h"

CEditControlView::CEditControlView(HWND editControl) : hwndEditControl(editControl) {
}

void CEditControlView::SetRect(RECT newRect) {
    ::SetWindowPos(hwndEditControl, 0, newRect.left, newRect.bottom, newRect.right, newRect.top, SWP_NOZORDER);
}

HWND CEditControlView::GetHWND() {
    return hwndEditControl;
}

ViewType CEditControlView::GetType() {
    return EDIT_CONTROL;
}