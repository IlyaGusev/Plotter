#pragma once
#include <Windows.h>

class CFracDrawable {
public:
	virtual void OnDrawFrac( RECT rect ) = 0;
};