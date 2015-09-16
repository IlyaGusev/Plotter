#pragma once
#include "IControlView.h"
#include "FracDrawable.h"

class CFracControlView : public IControlView {
public:
	CFracControlView( CFracDrawable* drawable, IControlView* firstChild, IControlView* secondChild );
	~CFracControlView() {}

	void SetRect( RECT rect );

	HWND GetHWND();

	ViewType GetType();

	IControlView* GetFirstChild();
	IControlView* GetSecondChild();
private:
	CFracDrawable* drawable;

	IControlView* firstChild;
	IControlView* secondChild;
};