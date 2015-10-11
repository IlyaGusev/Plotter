#include "Caret.h"
#include "Model/EditControlModel.h"
#include "Model/IBaseExprModel.h"

CCaret::CCaret() : offset( 0 )
{
}

int CCaret::GetPointX() const {
	return curEdit->GetSymbolPointByNumber( offset );
}

int CCaret::GetPointY() const {
	return curEdit->GetRect().Top();
}

int CCaret::GetHeight() const {
	return curEdit->GetRect().Bottom() - curEdit->GetRect().Top();
}

void CCaret::SetCurEdit( std::shared_ptr<IBaseExprModel> curEdit ) {
	this->curEdit = std::dynamic_pointer_cast<CEditControlModel>( curEdit );
}

std::shared_ptr<CEditControlModel> CCaret::GetCurEdit( ) const {
	return curEdit;
}

int& CCaret::Offset() {
	return offset;
}

bool CCaret::operator!=(const CCaret& other) const {
	return !(other == *this);
}

bool CCaret::operator==(const CCaret& other) const {
	return curEdit == other.curEdit && offset == other.offset;
}
