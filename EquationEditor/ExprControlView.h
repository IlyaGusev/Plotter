#include "IControlView.h"

class CExprControlView : public IControlView {
public:
	CExprControlView();
	~CExprControlView() {}

	void SetRect( RECT rect );

	HWND GetHWND();

	ViewType GetType();
private:
	RECT rect;
};