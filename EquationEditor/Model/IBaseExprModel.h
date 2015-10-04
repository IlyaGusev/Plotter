#pragma once
#include <list>
#include <memory>
#include <Windows.h>

#include "Model/Utils/Rect.h"
#include "Model/Utils/Line.h"
#include "Model/Utils/Caret.h"

#define MIN(x, y) x < y ? x : y;
#define MAX(x, y) x > y ? x : y;

enum ViewType { TEXT, EXPR, FRAC, DEGR, SUBSCRIPT, RADICAL };

// Что из этой модельки нужно отрисовать на экране
struct CDrawParams {
	std::wstring text;
	std::list<CLine> polygon;
	bool isHightlighted;	// Есть ли подсветка (должна быть над созданным контролом до того, как в него что-то введут)

	CDrawParams() :
		isHightlighted( false )
	{
	}

	CDrawParams( std::wstring _text, std::list<CLine> _polygon, bool _isHightlighted ) :
		text( _text ),
		polygon( _polygon ),
		isHightlighted( _isHightlighted )
	{
	}
};

// Модель элемента выражения
class IBaseExprModel : public std::enable_shared_from_this<IBaseExprModel> {
protected:
	std::weak_ptr<IBaseExprModel> parent;
	CRect rect;
	CDrawParams params;

public:
	virtual ~IBaseExprModel()
	{
	}

	virtual std::weak_ptr<IBaseExprModel> GetParent() const;
	virtual void SetParent( std::weak_ptr<IBaseExprModel> parent );

	virtual std::list<std::shared_ptr<IBaseExprModel>> GetChildren() const = 0;

	// Прикрепляет детей к свежесозданной модели. Имеет смысл вызывать эту функцию после каждого конструктора
	virtual void InitializeChildren() = 0;

	// Нужно вызывать при создании модели для корректного определения начального положения дочерних элементов
	virtual void SetRect( CRect rect );
	// Просто выставляет размеры прямоугольника
	virtual CRect GetRect();
	// Двигает прямоугольник со всеми относящимися к нему дочерними элементами 
	virtual void MoveBy( int dx, int dy );

	// изменение размеров (только размеров) своего прямоугольника в соответствии с размерами прямоугольников непосредственных детей
	virtual void Resize() = 0;

	// расставить детей по своим местам
	virtual void PlaceChildren() = 0;

	// выдаёт середину модели, по которой будет выполняться выравнивание
	virtual int GetMiddle() const = 0;

	// Возвращает текст, хранящийся в этой модели
	virtual std::wstring GetText() const;

	// Возвращает набор линий, которые нужно провести на вьюшке, относящейся к этой модели
	virtual std::list<CLine> GetLines() const;

	// Говорит, подсвечен ли прямоугольник этого контрола
	virtual bool IsHightlighted() const;
	
	// Возвращает тип модели
	virtual ViewType GetType() const = 0;

	// Сдвигает каретку в нужную сторону относительно from
	virtual void GoLeft( std::shared_ptr<const IBaseExprModel> from, CCaret& caret ) const = 0;
	virtual void GoRight( std::shared_ptr<const IBaseExprModel> from, CCaret& caret ) const = 0;
};

inline std::weak_ptr<IBaseExprModel> IBaseExprModel::GetParent( ) const
{
	return parent;
}

inline void IBaseExprModel::SetParent( std::weak_ptr<IBaseExprModel> parent )
{
	this->parent = parent;
}

inline CRect IBaseExprModel::GetRect()
{
	return rect;
}

inline void IBaseExprModel::SetRect( CRect rect )
{
	this->rect = rect;
}

inline void IBaseExprModel::MoveBy( int dx, int dy ) 
{
	rect.MoveBy( dx, dy );
}

inline std::wstring IBaseExprModel::GetText() const 
{
	return params.text;
}

inline std::list<CLine> IBaseExprModel::GetLines() const 
{
	return params.polygon;
}

inline bool IBaseExprModel::IsHightlighted() const {
	return params.isHightlighted;
}