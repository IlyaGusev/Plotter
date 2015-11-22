#pragma once
#include "View\EquationEditorWindow.h"

class CMainWindow {
public:
  CMainWindow();

  // ���������������� ����� ����
  // ���� true, �� ���������������� �������
  static bool RegisterClassW();

  HWND GetHandle()
  {
    return hwnd;
  }

  UINT32 getRibbonHeight();
  RECT getChildRect();

  // ������� ��������� ����
  // ���� true, �� ������� �������
  bool Create();
  // �������� ����
  void Show(int cmdShow);
  // �������� edit'�
  void OnCreate();
  // ����������� �� ��������� ������� ����
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
  HWND hwnd;		// ����� ����
  CEquationEditorWindow editor;  // ��������� �������-����������

  static LRESULT __stdcall mainWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};