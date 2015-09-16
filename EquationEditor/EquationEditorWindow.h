#pragma once
#include <Windows.h>
#include "EquationPresenter.h"
#include "EditControlView.h"

class CEquationEditorWindow {
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

	void OnChar();
protected:
    void OnDestroy();

private:
    HWND hwnd; // хэндл окна

    static const wchar_t* const className;

    static LRESULT __stdcall equationEditorWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK editControlProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	WNDPROC originEditControlProc;

    CEquationPresenter presenter;
};