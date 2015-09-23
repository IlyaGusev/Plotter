#pragma once
#include <list>

#include <Windows.h>

struct CLine {
	LONG startX;
	LONG startY;
	LONG endX;
	LONG endY;

	CLine( LONG _startX, LONG _startY, LONG _endX, LONG _endY ) :
		startX( _startX ),
		startY( _startY ),
		endX( _endX ),
		endY( _endY )
	{
	}
};

// Что из этой модельки нужно отрисовать на экране
struct CDrawParams {
	std::wstring text;

	std::list<CLine> polygon;

	CDrawParams()
	{
	}

	CDrawParams( std::wstring _text, std::list<CLine> _polygon ) :
		text( _text ),
		polygon( _polygon )
	{
	}
};

// Модель элемента выражения
class IBaseExprModel {
protected:
	IBaseExprModel* parent;
	RECT rect;
	CDrawParams params;

public:
	virtual ~IBaseExprModel()
	{
	}

	virtual IBaseExprModel* GetParent( ) const = 0;
	virtual void SetParent( IBaseExprModel* parent ) = 0;

	virtual std::list<IBaseExprModel*> GetChildren() const = 0;

	virtual RECT GetRect() const = 0;
	virtual void SetRect( RECT rect ) = 0;

	// изменение размеров (только размеров) своего прямоугольника в соответствии с размерами прямоугольников непосредственных детей
	virtual void Resize() = 0;

	// расставить детей по своим местам
	virtual void PermutateChildren() = 0;

	virtual CDrawParams GetDrawParams() const = 0;
};