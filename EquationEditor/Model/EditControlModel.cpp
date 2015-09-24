#include "Model/EditControlModel.h"

CEditControlModel::CEditControlModel() {
	rect.bottom = rect.top = rect.left = rect.right = 0;
	parent = nullptr;
}

IBaseExprModel* CEditControlModel::GetParent() const {
	return parent;
}

void CEditControlModel::SetParent( IBaseExprModel* newParent ) {
	parent = newParent;
}

RECT CEditControlModel::GetRect() const {
	return rect;
}

void CEditControlModel::SetRect( RECT newRect ) {
	rect = newRect;
}

void CEditControlModel::Resize( )
{
}

void CEditControlModel::PermutateChildren( )
{
}

std::list< IBaseExprModel* > CEditControlModel::GetChildren() const {
	return std::list< IBaseExprModel* >();
}

CDrawParams CEditControlModel::GetDrawParams() const {
	return params;
}

void CEditControlModel::InsertSymbol( wchar_t symbol, int offset, int symbolWidth ) {
	params.text.insert( offset, 1, symbol );
	rect.right += symbolWidth;
	symbolsWidths.push_back( symbolWidth );
}

int CEditControlModel::DeleteSymbol( int offset ) {
	params.text.erase( offset, 1 );
	int symbolsWidth = symbolsWidths[offset];
	rect.right -= symbolsWidth;
	symbolsWidths.erase( symbolsWidths.begin() + offset );
	return symbolsWidth;
}

CEditControlModel* CEditControlModel::SliceEditControl( int offset ) {
	CEditControlModel* newEditControl = new CEditControlModel();

	// Вставляем всё, начиная с offset, в новый edit control
	int newEditControlWidth = 0;
	for( int i = offset; i < symbolsWidths.size(); ++i ) {
		newEditControlWidth += symbolsWidths[i];
		newEditControl->symbolsWidths.push_back( symbolsWidths[i] );
		newEditControl->params.text.push_back( params.text[i] );
	}
	newEditControl->parent = parent;
	newEditControl->rect = rect;
	newEditControl->rect.left = rect.right - newEditControlWidth;

	// Удаляем всё до offset из старого
	rect.right = newEditControl->rect.left;
	params.text.resize( offset );
	symbolsWidths.resize( offset );
	return newEditControl;
}

std::vector<int> CEditControlModel::GetSymbolsWidths() {
	return symbolsWidths;
}
