#pragma once
#include "IBaseExprModel.h"
#include <string>
#include <vector>

// Модель текстового поля
class CEditControlModel : public IBaseExprModel {
public:
	CEditControlModel();
	~CEditControlModel() {}

	IBaseExprModel* GetParent();
	void SetParent( IBaseExprModel* parent );

	std::list< IBaseExprModel* > GetChildren();

	RECT GetRect();
	void SetRect( RECT rect );

	CDrawParams GetDrawParams();

	void InsertSymbol( wchar_t symbol, int offset, int symbolWidth );
	
	// Удаляет символ
	// Возвращает ширину удаленного символа
	int DeleteSymbol( int offset );

	// Разрезает edit control на два по offset
	// Возвращает второй edit control
	CEditControlModel* SliceEditControl( int offset );

	std::vector<int> GetSymbolsWidths();
private:
	RECT rect;
	CDrawParams params;

	IBaseExprModel* parent;
	// Ширина каждого символа
	std::vector<int> symbolsWidths;
};