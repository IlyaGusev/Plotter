#pragma once
#include "IControlView.h"
#include "ExprControlView.h"
#include "EditControlView.h"

#include "IBaseExprModel.h"
#include "ExprControlModel.h"
#include "EditControlModel.h"

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>

// Класс, размещающий прямоугольники вьюшек на экране
class CEquationPresenter
{
public:
    CEquationPresenter();

	void AddControlView( IControlView* newView, HWND currViewHwnd );

private:
    IBaseExprModel* root;

	std::unordered_map<IBaseExprModel*, IControlView*> views;

	IBaseExprModel* getControlView( HWND hwnd );
};