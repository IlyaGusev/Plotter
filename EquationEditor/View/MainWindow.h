#pragma once
#include "View\EquationEditorWindow.h"

class CMainWindow {
public:
  CMainWindow();

  // Зарегистрировать класс окна
  // Если true, то зарегистрировали успешно
  static bool RegisterClassW();

  HWND GetHandle()
  {
    return hwnd;
  }

  UINT32 getRibbonHeight();

  // Создать экземпляр окна
  // Если true, то создано успешно
  bool Create();
  // Показать окно
  void Show(int cmdShow);
  // Создание edit'а
  void OnCreate();
  // Реагировать на изменения размера окна
  void OnSize(int cxSize, int cySize);

  void OnChar(WPARAM wParam);

  void OnDraw();

  void Redraw();

  // void OnLButtonDown(int xMousePos, int yMousePos);

  void OnWmCommand(WPARAM wParam, LPARAM lParam);

  void OnKeyDown(WPARAM wParam);

  void OnMouseMove(WPARAM wParam, int x, int y);
protected:
  void OnDestroy();

private:
  HWND hwnd;		// хэндл окна
  CEquationEditorWindow editor;  // экземпляр ребенка-рисователя

  static LRESULT __stdcall mainWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};