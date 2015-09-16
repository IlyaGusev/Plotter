#include "EditControlView.h"

CEditControlView::CEditControlView(HWND editControl) : hwndEditControl(editControl) {
}

void CEditControlView::SetRect(RECT newRect) {
	::SetWindowPos( hwndEditControl, HWND_TOP, newRect.left, newRect.top, newRect.right - newRect.left, newRect.bottom - newRect.top, NULL );
}

HWND CEditControlView::GetHWND() {
    return hwndEditControl;
}

ViewType CEditControlView::GetType() {
    return EDIT_CONTROL;
}