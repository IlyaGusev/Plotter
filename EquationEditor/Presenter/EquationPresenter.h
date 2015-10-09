﻿#pragma once

#include <utility>
#include <memory>
#include <Windows.h>

#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/FracControlModel.h"
#include "Model/DegrControlModel.h"
#include "Model/SubscriptControlModel.h"
#include "Model/RadicalControlModel.h"
#include "Model/Utils/Caret.h"

#include "Presenter/Utils/TreeBfsProcessor.h"
#include "Presenter/Utils/TreeDfsProcessor.h"

// Интерфейс окна редактора
class IEditorView {
public:
	virtual ~IEditorView() {}

	// Отображает прямоугольник выделения вокруг вьюшки
	virtual void DrawSelectedRect( const CRect& rect ) = 0;

	// Отобразить текст в определенном прямоугольнике
	virtual void DrawString( const std::wstring& text, const CRect& rect, bool isSelected ) = 0;

	// Нарисовать ломаную
	virtual void DrawPolygon( const std::list<CLine>& polygon, bool isSelected ) = 0;

	// Нарисовать подсветку вокруг прямоугольника
	virtual void DrawHighlightedRect( const CRect& rect, bool isSelected ) = 0;

	// Установить положение каретки
	virtual void SetCaret( int caretPointX, int caretPointY, int height ) = 0;

	// Запустить перерисовку окна
	virtual void Redraw() = 0;

	virtual int GetSymbolWidth( wchar_t symbol, int symbolHeight ) = 0;
};


// Класс, размещающий прямоугольники вьюшек на экране
class CEquationPresenter {
public:
	CEquationPresenter( IEditorView& newView );
	~CEquationPresenter();

	void AddControlView( ViewType viewType );

	void InsertSymbol( wchar_t symbol );

	void DeleteSymbol();

	// Отправляет во вьюшку всё, что нужно на ней нарисовать
	void OnDraw();
	
	void SetCaret( int x, int y );

	void SetSelection( int x, int y );

	// Обработка движений каретки стрелками
	void MoveCaretLeft();
	void MoveCaretRight();
	void MoveCaretTop();
	void MoveCaretBottom();
private:
    std::shared_ptr<CExprControlModel> root;
	IEditorView& view;
	CCaret caret;

	// processors
	CTreeBfsProcessor highlightingProcessor;
	CTreeBfsProcessor placeProcessor;
	CTreeDfsProcessor resizeProcessor;
	CTreeBfsProcessor drawProcessor;
	CTreeBfsProcessor deleteSelectionProcessor;

	void addFrac( std::shared_ptr<CExprControlModel> parent );
	void addRadical(std::shared_ptr<CExprControlModel> parent);

	void addDegr( std::shared_ptr<CExprControlModel> parent );

	void addSubscript(std::shared_ptr<CExprControlModel> parent);
	
	// Ищет позицию каретки с таким x
	// Возвращает пару <координата, номер буквы>
	std::pair<int, int> findCaretPos( std::shared_ptr<CEditControlModel> editControlModel, int x );

	void setCaretPos( int x, int y, CCaret& curCaret );
	void invalidateTree();

	void invalidateBranch(std::shared_ptr<IBaseExprModel> startingNode);
};