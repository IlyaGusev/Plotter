#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"
#include <string>

class IBracketsModel : public IBaseExprModel
{
public:
	IBracketsModel( CRect rect, std::weak_ptr<IBaseExprModel> parent );

	virtual std::list<std::shared_ptr<IBaseExprModel>> GetChildren() const;
	virtual void InitializeChildren( std::shared_ptr<IBaseExprModel> initChild = 0 );

	virtual void Resize();

	virtual void PlaceChildren(); // virtual??

	virtual int GetMiddle() const;

	virtual void SetRect( const CRect& rect );

	virtual ViewType GetType() const = 0; 

	virtual void MoveBy( int dx, int dy );
	virtual void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );
	virtual void MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );

	virtual void UpdateSelection();

	virtual bool IsEmpty() const;

	virtual std::shared_ptr<IBaseExprModel> CopySelected() const = 0;
	virtual std::wstring Serialize() = 0;
protected:
	virtual void updatePolygons() = 0;

	// Содержимое скобок
	std::shared_ptr<IBaseExprModel> content;
};