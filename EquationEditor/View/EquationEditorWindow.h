#pragma once
#include "Presenter/EquationPresenter.h"

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

	void DrawText( HDC hdc, std::wstring text, CRect rect );

	void DrawPolygon( HDC hdc, std::list<CLine> polygon );

	void SetCaret( int caretPointX, int caretPointY, int height );

	void Redraw();

	int GetCharWidth( wchar_t symbol );

	void OnLButtonDown( int xMousePos, int yMousePos );

	void OnWmCommand( WPARAM wParam, LPARAM lParam );
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