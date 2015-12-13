#pragma once

#include <utility>
#include <memory>
#include <Windows.h>

#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/Utils/Caret.h"
#include "Model/SystemControlModel.h"

#include "Presenter/Utils/TreeBfsProcessor.h"
#include "Presenter/Utils/TreeDfsProcessor.h"


// Интерфейс окна редактора
class IEditorView
{
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
class CEquationPresenter
{
public:
	CEquationPresenter( IEditorView& newView, int default_bottom = 50, std::shared_ptr<IBaseExprModel> old_presenter = nullptr );

	void SetDelta( int delta );

	void AddControlView( ViewType viewType );

	void InsertSymbol( wchar_t symbol );

	void DeleteSymbol( bool withCtrl = false );

	void DeleteNextSymbol( bool withCtrl = false );

	std::wstring Serialize( bool forValidation );

	// Отправляет во вьюшку всё, что нужно на ней нарисовать
	void OnDraw();

	void SetCaret( int x, int y );

    void SetCaret( CCaret c )
    {
        caret = c;
    }

	void SetSelection( int x, int y );

	// Обработка движений каретки стрелками
	void MoveCaretLeft();
	void MoveCaretRight();
	void MoveCaretTop();
	void MoveCaretBottom();

	void OnEnter(); // пока только создает нового ребенка, если мы находимся внутри системы уравнений

	CCaret GetCaret() const
	{
		return caret;
	}

	std::shared_ptr<CExprControlModel> GetRoot()
	{
		return root;
	}

private:
	std::shared_ptr<CExprControlModel> root;
	IEditorView& view;
	CCaret caret;
	CCaret beginSelectionCaret;
	bool isInSelectionMode;

	// processors
	CTreeBfsProcessor highlightingProcessor;
	CTreeBfsProcessor placeProcessor;
	CTreeDfsProcessor resizeProcessor;
	CTreeBfsProcessor drawProcessor;
	CTreeBfsProcessor deleteSelectionProcessor;
	CTreeBfsProcessor updateSelectionProcessor;

	void addFrac( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild );
	void addRadical( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild );
	void addDegr( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild );
	void addSubscript( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild );
	void addParentheses( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild );
	void addSum( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild );
	void addProduct( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild );
	void addSystem( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild );
	void addSquareBrackets( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild );
	void addBraces( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild );

	void deleteSelectedParts();

	// Говорит, правее ли model1 находится model2
	bool isRightDirection( const IBaseExprModel* model1, const IBaseExprModel* model2, int offset1, int offset2 );

	// Ищет позицию каретки с таким x
	// Возвращает пару <координата, номер буквы>
	std::pair<int, int> findCaretPos( std::shared_ptr<CEditControlModel> editControlModel, int x );

	void setCaretPos( int x, int y, CCaret& curCaret );
	void invalidateTree();

	void invalidateBranch( std::shared_ptr<IBaseExprModel> startingNode );

	int deltaY; // текущая величина сдвига прямоугольничков вверх (для обработки скролла)

	std::shared_ptr<CSystemControlModel> NearestSystem( std::shared_ptr<CEditControlModel> edit ); // находит ближайшую систему уравнений
};