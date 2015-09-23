#pragma once
#include <string>
#include <vector>

#include "Model/IBaseExprModel.h"

// Модель текстового поля
class CEditControlModel : public IBaseExprModel {
public:
	CEditControlModel();
	~CEditControlModel() {}

	IBaseExprModel* GetParent() const;
	void SetParent( IBaseExprModel* parent );

	std::list< IBaseExprModel* > GetChildren() const;

	RECT GetRect() const;
	void SetRect( RECT rect );
	
	void Resize();

	void PermutateChildren();

	CDrawParams GetDrawParams() const;

	void InsertSymbol( wchar_t symbol, int offset, int symbolWidth );
	
	// Удаляет символ
	// Возвращает ширину удаленного символа
	int DeleteSymbol( int offset );

	// Разрезает edit control на два по offset
	// Возвращает второй edit control
	CEditControlModel* SliceEditControl( int offset );

	std::vector<int> GetSymbolsWidths();
private:
	// Ширина каждого символа
	std::vector<int> symbolsWidths;
};