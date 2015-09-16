#include "EditControlView.h"

CEditControlView::CEditControlView(HWND editControl) : hwndEditControl(editControl) {
}

void CEditControlView::SetRect(RECT newRect) {
	::SetWindowPos( hwndEditControl, HWND_TOP, newRect.left, newRect.bottom, newRect.right - newRect.left, newRect.top - newRect.bottom, NULL );
}

HWND CEditControlView::GetHWND() {
    return hwndEditControl;
}

ViewType CEditControlView::GetType() {
    return EDIT_CONTROL;
}