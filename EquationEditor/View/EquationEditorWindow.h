#pragma once
#include "Presenter/EquationPresenter.h"
#include <unordered_map>

class CEquationEditorWindow : public IEditorView {
public:
    CEquationEditorWindow();
	~CEquationEditorWindow();

    // Зарегистрировать класс окна
    // Если true, то зарегистрировали успешно
    static bool RegisterClassW();

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

	void DrawString( const std::wstring& text, const CRect& rect );

	void DrawPolygon( const std::list<CLine>& polygon );

	void DrawHightlightedRect( CRect& rect );

	void SetCaret( int caretPointX, int caretPointY, int height );

	void Redraw();

	int GetSymbolWidth( wchar_t symbol, int symbolHeight );

	void OnLButtonDown( int xMousePos, int yMousePos );

	void OnWmCommand( WPARAM wParam, LPARAM lParam );

	void OnKeyDown( WPARAM wParam );
protected:
    void OnDestroy();

private:
    HWND hwnd; // хэндл окна
	HDC hdc;
	std::unordered_map<int, HFONT> fonts; // Отображение из высоты шрифта в шрифт

    static const wchar_t* const className;

    static LRESULT __stdcall equationEditorWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	WNDPROC originEditControlProc;

    std::shared_ptr<CEquationPresenter> presenter;
};