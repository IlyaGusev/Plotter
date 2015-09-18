#pragma once
#include <Windows.h>

#include "EquationPresenter.h"

class CEquationEditorWindow : public IEditorView {
public:
    CEquationEditorWindow();

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

	void DrawText( HDC hdc, std::wstring text, RECT rect );

	void DrawPolygon( HDC hdc, std::list<CLine> polygon );

	void SetCaret( POINT caretPoint, int height );

	void Redraw();

	int GetCharWidth( wchar_t symbol );

	void OnLButtonDown( int xMousePos, int yMousePos );
protected:
    void OnDestroy();

private:
    HWND hwnd; // хэндл окна
	std::list<CLine> paintedLines; // Список всех нарисованных линий

    static const wchar_t* const className;

    static LRESULT __stdcall equationEditorWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	WNDPROC originEditControlProc;

    CEquationPresenter* presenter;
};