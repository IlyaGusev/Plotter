// Validator.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//

#include "stdafx.h"
#include "Validator.h"
#include "FormulaComputability.h"

namespace ValidatorDll {

	class XmlLoadException : public exception {
	public:
		XmlLoadException(const char* errInfo) : errorMsg(errInfo) {};

		const char* what() const noexcept {
			return errorMsg.c_str();
		}
	private:
		string errorMsg;
	};

	// ������ ���������������� ����������
	VALIDATOR_API int nValidator = 0;

	// ������ ���������������� �������.
	VALIDATOR_API bool validate(const wchar_t* path)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(path);
		if (!result) {
			throw XmlLoadException(result.description());
		}

		if (FormulaComputability::checkComputability(doc)) {
			return true;
		}
		else {
			return false;
		}
	}

	// ����������� ��� ����������������� ������.
	// ��. ����������� ������ � Validator.h
	CValidator::CValidator()
	{
		return;
	}
}