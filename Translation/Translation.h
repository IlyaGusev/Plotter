// ����������� ���� ���� ifdef - ��� ����������� ����� �������� ��������, ���������� ��������� 
// �������� �� ��������� DLL. ��� ����� ������ DLL �������������� � �������������� ������� TRANSLATION_EXPORTS,
// � ��������� ������. ���� ������ �� ������ ���� ��������� � �����-���� �������
// ������������ ������ DLL. ��������� ����� ����� ������ ������, ��� �������� ����� �������� ������ ����, ����� 
// ������� TRANSLATION_API ��� ��������������� �� DLL, ����� ��� ������ DLL ����� �������,
// ������������ ������ ��������, ��� ����������������.
#ifdef TRANSLATION_EXPORTS
#define TRANSLATION_API __declspec(dllexport)
#else
#define TRANSLATION_API __declspec(dllimport)
#endif

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <stdexcept>
#include <fstream>
#include <string>
#include <streambuf>
using std::string;
using std::wstring;

namespace TranslationDll {
	// ���� ����� ������������� �� Translation.dll
	class TRANSLATION_API CTranslation {
	public:
		CTranslation(void);
		// TODO: �������� ����� ���� ������.
	};

	extern TRANSLATION_API int nTranslation;

	TRANSLATION_API wstring translateText(wstring input, string from, string to);
	TRANSLATION_API void translateFile(string input, string output, string from, string to);
}