// Приведенный ниже блок ifdef - это стандартный метод создания макросов, упрощающий процедуру 
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа TRANSLATION_EXPORTS,
// в командной строке. Этот символ не должен быть определен в каком-либо проекте
// использующем данную DLL. Благодаря этому любой другой проект, чьи исходные файлы включают данный файл, видит 
// функции TRANSLATION_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
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
	// Этот класс экспортирован из Translation.dll
	class TRANSLATION_API CTranslation {
	public:
		CTranslation(void);
		// TODO: Добавьте здесь свои методы.
	};

	extern TRANSLATION_API int nTranslation;

	TRANSLATION_API wstring translateText(wstring input, string from, string to);
	TRANSLATION_API void translateFile(string input, string output, string from, string to);
}