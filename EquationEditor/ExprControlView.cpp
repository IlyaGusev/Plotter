#include "ExprControlView.h"

CExprControlView::CExprControlView() {
}

void CExprControlView::SetRect( RECT newRect ) {
	rect = newRect;
}

HWND CExprControlView::GetHWND() {
	return NULL;
}

ViewType CExprControlView::GetType() {
	return EXPR;
}

