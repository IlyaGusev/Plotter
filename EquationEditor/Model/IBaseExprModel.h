#pragma once
#include <list>
#include <memory>

#include <Windows.h>

enum ViewType { TEXT, EXPR, FRAC, DEGR };

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

	void Set( LONG startX, LONG startY, LONG endX, LONG endY ) {
		this->startX = startX;
		this->startY = startY;
		this->endX = endX;
		this->endY = endY;
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
	std::shared_ptr<IBaseExprModel> parent;
	RECT rect;
	CDrawParams params;

public:
	virtual ~IBaseExprModel()
	{
	}

	virtual std::shared_ptr<IBaseExprModel> GetParent( ) const;
	virtual void SetParent( std::shared_ptr<IBaseExprModel> parent );

	virtual std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const = 0;

	virtual RECT GetRect() const;
	virtual void SetRect( RECT rect );

	// изменение размеров (только размеров) своего прямоугольника в соответствии с размерами прямоугольников непосредственных детей
	virtual void Resize() = 0;

	// расставить детей по своим местам
	virtual void PermutateChildren() = 0;

	virtual CDrawParams GetDrawParams() const;

	virtual ViewType GetType() const = 0;
};

inline std::shared_ptr<IBaseExprModel> IBaseExprModel::GetParent( ) const {
	return parent;
}

inline void IBaseExprModel::SetParent( std::shared_ptr<IBaseExprModel> parent ) {
	this->parent = parent;
}

inline RECT IBaseExprModel::GetRect() const {
	return rect;
}

inline void IBaseExprModel::SetRect( RECT rect ) {
	this->rect = rect;
}

inline CDrawParams IBaseExprModel::GetDrawParams() const {
	return params;
}