#pragma once
#include <string>
#include <vector>

#include "Model/IBaseExprModel.h"

// Модель текстового поля
class CEditControlModel : public IBaseExprModel {
public:
	CEditControlModel( CRect rect, const std::weak_ptr<IBaseExprModel> parent, bool isHightlighted = true );
	~CEditControlModel() {}

	std::list< std::shared_ptr<IBaseExprModel> > GetChildren( ) const;
	void InitializeChildren() {}

	void Resize();

	void PlaceChildren();

	int GetMiddle() const;

	void InsertSymbol( wchar_t symbol, int offset, int symbolWidth );
	
	// Удаляет символ
	// Возвращает ширину удаленного символа
	int DeleteSymbol( int offset );

	// Разрезает edit control на два по offset
	// Возвращает второй edit control
	std::shared_ptr<CEditControlModel> SliceEditControl( int offset );

	std::vector<int> GetSymbolsWidths() const;

	ViewType GetType() const;

	// Возвращает позицию символа на экране по его номеру
	int GetSymbolPointByNumber( int number ) const;

	void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret ) const;
	void MoveCaretRight( const IBaseExprModel* from, CCaret& caret ) const;
private:
	// Ширина каждого символа
	std::vector<int> symbolsWidths;
	std::list< std::shared_ptr<IBaseExprModel> > children;

	static const int MINIMAL_WIDTH = 10;
};
