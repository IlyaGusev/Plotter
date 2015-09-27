#pragma once
#include <utility>
#include <memory>
#include <Windows.h>

#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"
#include "Model/EditControlModel.h"
#include "Model/FracControlModel.h"
#include "Model/DegrControlModel.h"

// Интерфейс окна редактора
class IEditorView {
public:
	virtual ~IEditorView() = 0;

	// Отобразить текст в определенном прямоугольнике
	virtual void DrawText( HDC hdc, std::wstring text, CRect rect ) = 0;

	// Нарисовать ломаную
	virtual void DrawPolygon( HDC hdc, std::list<CLine> polygon ) = 0;

	// Установить положение каретки
	virtual void SetCaret( POINT caretPoint, int height ) = 0;

	// Запустить перерисовку окна
	virtual void Redraw() = 0;

	virtual int GetCharWidth( wchar_t symbol ) = 0;
};

inline IEditorView::~IEditorView() {}

struct CCaret {
	// Текущий edit control, на котором стоит каретка
	std::shared_ptr<CEditControlModel> curEdit;
	// Координаты каретки на экране
	POINT caretPoint;
	// Номер символа, за которым стоит каретка
	int offset;

	CCaret() : offset( 0 ) {
		caretPoint.x = 0;
		caretPoint.y = 0;
		curEdit = nullptr;
	}
};

// Класс, размещающий прямоугольники вьюшек на экране
class CEquationPresenter {
public:
	CEquationPresenter( IEditorView* view );
	~CEquationPresenter();

	void AddControlView( ViewType viewType );

	void InsertSymbol( wchar_t symbol );

	void DeleteSymbol();

	void Draw( HDC hdc );
	
	void SetCaret( int x, int y );
private:
    std::shared_ptr<CExprControlModel> root;
	CCaret caret;
	IEditorView* view;

	void addFrac( std::shared_ptr<CExprControlModel> parent );
	void setFracRects( CRect parentRect, std::shared_ptr<CFracControlModel> fracModel );

	void addDegr( std::shared_ptr<CExprControlModel> parent );
	void setDegrRects( CRect parentRect, std::shared_ptr<CDegrControlModel> degrModel );
	
	// Ищет позицию каретки с таким x
	// Возвращает пару <координата, номер буквы>
	std::pair<int, int> findCaretPos( std::shared_ptr<CEditControlModel> editControlModel, int x );

	void updateTreeAfterSizeChange(std::shared_ptr<IBaseExprModel> startVert);
};