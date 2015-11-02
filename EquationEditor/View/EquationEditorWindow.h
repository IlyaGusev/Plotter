#pragma once
#include "Presenter/EquationPresenter.h"
#include <unordered_map>

class CEquationEditorWindow : public IEditorView {
public:
    CEquationEditorWindow();

    // Зарегистрировать класс окна
    // Если true, то зарегистрировали успешно
    static bool RegisterClassW();

	HWND GetHandle()
	{
		return hwnd;
	}

    // Создать экземпляр окна
    // Если true, то создано успешно
    bool Create();
    // Показать окно
    void Show(int cmdShow);
    // Создание edit'а
    void OnCreate();
    // Реагировать на изменения размера окна
    void OnSize(int cxSize, int cySize);

	void OnChar( WPARAM wParam );

	void OnDraw();

	void DrawSelectedRect( const CRect& rect );

	void DrawString( const std::wstring& text, const CRect& rect, bool isSelected );

	void DrawPolygon( const std::list<CLine>& polygon, bool isSelected );

	void DrawHighlightedRect( const CRect& rect, bool isSelected );

	void SetCaret( int caretPointX, int caretPointY, int height );

	void Redraw();

	int GetSymbolWidth( wchar_t symbol, int symbolHeight );

	void OnLButtonDown( int xMousePos, int yMousePos );

	void OnWmCommand( WPARAM wParam, LPARAM lParam );

	void OnKeyDown( WPARAM wParam );

	void OnMouseMove( WPARAM wParam, int x, int y );
protected:
    void OnDestroy();

private:
    HWND hwnd;		// хэндл окна
	HDC hdc;		// канва для рисования
	std::unordered_map<int, HFONT> fonts; // Отображение из высоты шрифта в шрифт
	bool isPressedShift;

	COLORREF symbolSelectedColorref;
	COLORREF symbolUnselectedColorref;
	COLORREF bkSelectedColorref;
	COLORREF bkUnselectedColorref;
	COLORREF bkSelectedHighlightColorref;
	COLORREF bkUnselectedHightlightColorref;

    static const wchar_t* const className;
	
	std::shared_ptr<CEquationPresenter> presenter;

    static LRESULT __stdcall equationEditorWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	HFONT getFont( int height );
};