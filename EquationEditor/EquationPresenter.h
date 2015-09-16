#pragma once
#include "IControlView.h"
#include "ExprControlView.h"
#include "EditControlView.h"
#include "FracControlView.h"

#include "IBaseExprModel.h"
#include "ExprControlModel.h"
#include "EditControlModel.h"
#include "FracControlModel.h"

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

	void ExtendControlView( HWND currViewHwnd, int deltaWidth );
private:
    CExprControlModel* root;

	std::unordered_map<IBaseExprModel*, IControlView*> views;

	IBaseExprModel* getModel( HWND hwnd );

	void addFrac( IControlView* view, CExprControlModel* parent, RECT focusedViewRect );

	void setRect( IBaseExprModel* model, RECT rect );

	// void updateGraph(IControlModel* startVert);
};