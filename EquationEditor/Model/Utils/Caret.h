#pragma once
#include <memory>
#include "Model/Utils/Rect.h"

class CEditControlModel;
class IBaseExprModel;

class CCaret {
public:
	CCaret();

	int GetPointX() const;

	int GetPointY() const;

	int GetHeight() const;
	
	void SetCurEdit( std::shared_ptr<IBaseExprModel> curEdit );

	std::shared_ptr<CEditControlModel> GetCurEdit( ) const;

	int& Offset();
  void SetOffset(int off);

	bool operator==(const CCaret& other) const;

	bool operator!=(const CCaret& other) const;
private:
	// Текущий edit control, на котором стоит каретка
	std::shared_ptr<CEditControlModel> curEdit;
	// Номер символа, за которым стоит каретка
	int offset;
};