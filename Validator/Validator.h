// ����������� ���� ���� ifdef - ��� ����������� ����� �������� ��������, ���������� ��������� 
// �������� �� ��������� DLL. ��� ����� ������ DLL �������������� � �������������� ������� VALIDATOR_EXPORTS,
// � ��������� ������. ���� ������ �� ������ ���� ��������� � �����-���� �������
// ������������ ������ DLL. ��������� ����� ����� ������ ������, ��� �������� ����� �������� ������ ����, ����� 
// ������� VALIDATOR_API ��� ��������������� �� DLL, ����� ��� ������ DLL ����� �������,
// ������������ ������ ��������, ��� ����������������.
#ifdef VALIDATOR_EXPORTS
#define VALIDATOR_API __declspec(dllexport)
#else
#define VALIDATOR_API __declspec(dllimport)
#endif

#include "common.h"

namespace ValidatorDll {
	// ���� ����� ������������� �� Validator.dll
	class VALIDATOR_API CValidator {
	public:
		CValidator(void);
		// TODO: �������� ����� ���� ������.
	};

	extern VALIDATOR_API int nValidator;

	VALIDATOR_API bool validate(const wchar_t* path);
}