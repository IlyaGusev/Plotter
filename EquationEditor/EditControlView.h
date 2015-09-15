#include "IControlView.h"

class CEditControlView : public IControlView 
{
public:
    CEditControlView(HWND editControl);

    void SetRect(RECT rect);

    HWND GetHWND();
private:
    HWND hwndEditControl;
};