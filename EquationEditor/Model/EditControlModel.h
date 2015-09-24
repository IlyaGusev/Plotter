#pragma once
#include <string>
#include <vector>

#include "Model/IBaseExprModel.h"

// Модель текстового поля
class CEditControlModel : public IBaseExprModel {
public:
	CEditControlModel();
	~CEditControlModel() {}

	std::list< std::shared_ptr<IBaseExprModel> > GetChildren( ) const;

	void Resize();

	void PermutateChildren();

	void InsertSymbol( wchar_t symbol, int offset, int symbolWidth );
	
	// Удаляет символ
	// Возвращает ширину удаленного символа
	int DeleteSymbol( int offset );

	// Разрезает edit control на два по offset
	// Возвращает второй edit control
	std::shared_ptr<CEditControlModel> SliceEditControl( int offset );

	std::vector<int> GetSymbolsWidths();

	ViewType GetType() const;
private:
	// Ширина каждого символа
	std::vector<int> symbolsWidths;
	std::list< std::shared_ptr<IBaseExprModel> > children;
};