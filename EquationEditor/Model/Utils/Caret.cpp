#include "Caret.h"

CCaret::CCaret() : offset( 0 ) {
	curEdit = nullptr;
}

int CCaret::GetPointX() {
	return curEdit->GetSymbolPointByNumber( offset );
}

int CCaret::GetPointY() {
	return curEdit->Rect().Top();
}

int CCaret::GetHeight() {
	return curEdit->Rect().Bottom() - curEdit->Rect().Top();
}

void CCaret::SetCurEdit( std::shared_ptr<CEditControlModel> curEdit ) {
	this->curEdit = curEdit;
}

std::shared_ptr<CEditControlModel> CCaret::GetCurEdit() {
	return curEdit;
}

int& CCaret::Offset() {
	return offset;
}
