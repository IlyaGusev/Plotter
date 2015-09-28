#include "Model/EditControlModel.h"
#include <memory>

class CCaret {
public:
	CCaret();

	int GetPointX();

	int GetPointY();

	int GetHeight();
	
	void SetCurEdit( std::shared_ptr<CEditControlModel> curEdit );

	std::shared_ptr<CEditControlModel> GetCurEdit();

	int& Offset();
private:
	// Текущий edit control, на котором стоит каретка
	std::shared_ptr<CEditControlModel> curEdit;
	// Номер символа, за которым стоит каретка
	int offset;
};