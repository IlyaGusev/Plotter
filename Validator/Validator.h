// Приведенный ниже блок ifdef - это стандартный метод создания макросов, упрощающий процедуру 
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа VALIDATOR_EXPORTS,
// в командной строке. Этот символ не должен быть определен в каком-либо проекте
// использующем данную DLL. Благодаря этому любой другой проект, чьи исходные файлы включают данный файл, видит 
// функции VALIDATOR_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#ifdef VALIDATOR_EXPORTS
#define VALIDATOR_API __declspec(dllexport)
#else
#define VALIDATOR_API __declspec(dllimport)
#endif

#include "common.h"

namespace ValidatorDll {
	// Этот класс экспортирован из Validator.dll
	class VALIDATOR_API CValidator {
	public:
		CValidator(void);
		// TODO: Добавьте здесь свои методы.
	};

	extern VALIDATOR_API int nValidator;

	VALIDATOR_API bool validate(const wchar_t* path);
}