#include "FracControlView.h"

CFracControlView::CFracControlView( CFracDrawable* _drawable, IControlView* _firstChild, IControlView* _secondChild ) :
	drawable( _drawable ), firstChild(_firstChild), secondChild(_secondChild) {}

void CFracControlView::SetRect( RECT newRect ) {
	drawable->OnDrawFrac( newRect );
}

HWND CFracControlView::GetHWND() {
	return NULL;
}

ViewType CFracControlView::GetType() {
	return FRAC_CONTROL;
}

IControlView* CFracControlView::GetFirstChild() {
	return firstChild;
}

IControlView* CFracControlView::GetSecondChild() {
	return secondChild;
}