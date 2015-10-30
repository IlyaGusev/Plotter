#pragma once
#include <string>
#include <vector>

#include "Model/IBaseExprModel.h"

// Модель текстового поля
class CEditControlModel : public IBaseExprModel {
public:
	CEditControlModel( const CRect& rect, const std::weak_ptr<IBaseExprModel> parent, bool isHightlighted = true );

	std::list< std::shared_ptr<IBaseExprModel> > GetChildren( ) const;
	void InitializeChildren( std::shared_ptr<IBaseExprModel> initChild = 0 ) {}

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

	void DeleteSelection();

	void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );
	void MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );

	bool IsEmpty() const;

	std::list<std::pair<std::wstring, CRect>> GetSelectedText() const;
	std::list<std::pair<std::wstring, CRect>> GetUnselectedText() const;

	void UpdateSelection();

	bool DeleteSelectedPart();

	void MergeWith( const CEditControlModel& edit );

	std::shared_ptr<IBaseExprModel> CopySelected() const;
	std::wstring Serialize();

	static const int MINIMAL_WIDTH = 10;
	static const int MINIMAL_HEIGHT = 5;
private:
	std::vector<int> symbolsWidths;	// Ширина каждого символа
	std::list< std::shared_ptr<IBaseExprModel> > children;	// Заглушка-пустой список
};
