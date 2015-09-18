#pragma once
#include <Windows.h>
#include <list>

struct CLine {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;

	CLine( LONG _left, LONG _top, LONG _right, LONG _bottom ) : left( _left ), top( _top ), right( _right ), bottom( _bottom ) {}
};

// Что из этой модельки нужно отрисовать на экране
struct CDrawParams {
	std::wstring text;

	std::list<CLine> polygon;

	CDrawParams() {}
	CDrawParams( std::wstring _text, std::list<CLine> _polygon ) : text( text ), polygon( _polygon ) {}
};

// Модель элемента выражения
class IBaseExprModel {
public:
    virtual ~IBaseExprModel() = 0;

	virtual IBaseExprModel* GetParent( ) = 0;
	virtual void SetParent( IBaseExprModel* parent ) = 0;

	virtual std::list<IBaseExprModel*> GetChilds() = 0;

    virtual RECT GetRect() = 0;
    virtual void SetRect(RECT rect) = 0;

	virtual CDrawParams GetDrawParams() = 0;
};

inline IBaseExprModel::~IBaseExprModel() {}