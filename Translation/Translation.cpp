// Translation.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <direct.h>
#include "Translation.h"
using namespace std;
extern int NOTATION;
extern ofstream FOUT;
extern int mlparse();
extern FILE *mlin;
extern int texparse();
extern FILE *texin;
extern int omparse();
extern FILE *omin;

namespace TranslationDll {
	// Пример экспортированной переменной
	TRANSLATION_API int nTranslation = 0;

	TRANSLATION_API void translateFile(string input, string output, string from, string to){
		int to_notation = 0;
		if (to == "mathml")
			to_notation = 0;
		else if (to == "openmath")
			to_notation = 1;
		else if (to == "tex")
			to_notation = 2;

		NOTATION = to_notation;
		if (from == "mathml") {
			mlin = fopen(input.c_str(), "r");
			if (mlin == NULL)
				throw new invalid_argument("File not found");
			FOUT.open(output.c_str(), std::ofstream::out);
			mlparse();
			fclose(mlin);
			FOUT.close();

			ifstream ofile;
			ofile.open(input.c_str());
			string result((istreambuf_iterator<char>(ofile)), istreambuf_iterator<char>());
			ofile.close();
			ifstream ifile;
			ifile.open(output.c_str());
			string result1((istreambuf_iterator<char>(ifile)), istreambuf_iterator<char>());
			ifile.close();
		}
		else if (from == "tex") {
			texin = fopen(input.c_str(), "r");
			if (texin == NULL)
				throw new invalid_argument("File not found");
			FOUT.open(output.c_str(), std::ofstream::out);
			texparse();
			fclose(texin);
			FOUT.close();
		}
		else if (from == "openmath") {
			omin = fopen(input.c_str(), "r");
			if (omin == NULL)
				throw new invalid_argument("File not found");
			FOUT.open(output.c_str(), std::ofstream::out);
			omparse();
			fclose(omin);
			FOUT.close();
		}
		
	}

	TRANSLATION_API wstring translateText(wstring input, string from, string to) {
		char NPath[MAX_PATH];
		_getcwd(NPath, MAX_PATH);
		string path(NPath);
		string iname = path + "translation_input.txt";
		string oname = path + "translation_output.txt";
		wofstream ifile;
		ifile.open(iname);
		ifile << input;
		ifile.close();

		translateFile(iname, oname, from, to);

		wifstream ofile;
		ofile.open(oname);
		wstring result((istreambuf_iterator<wchar_t>(ofile)), istreambuf_iterator<wchar_t>());
		ofile.close();

		remove(iname.c_str());
		remove(oname.c_str());
		return result;
	}

	// Конструктор для экспортированного класса.
	// см. определение класса в Translation.h
	CTranslation::CTranslation()
	{
		return;
	}
}