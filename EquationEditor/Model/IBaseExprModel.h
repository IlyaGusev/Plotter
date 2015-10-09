#pragma once
#include <list>
#include <memory>
#include <Windows.h>

#include "Model/Utils/Rect.h"
#include "Model/Utils/Line.h"
#include "Model/Utils/Caret.h"

enum ViewType { TEXT, EXPR, FRAC, DEGR, SUBSCRIPT, RADICAL };

// Что из этой модельки нужно отрисовать на экране
struct CDrawParams {
	std::wstring text;
	std::pair<int, int> selectedPositions;		// Номера первой и следующей за последней выбранной буквы

	std::list<CLine> polygon;
	bool isHighlighted;	// Есть ли подсветка (должна быть над созданным контролом до того, как в него что-то введут)
	bool isSelected;	// Выделена ли вьюшка

	CDrawParams() :
		isHighlighted( false ),
		isSelected( false )
	{
		selectedPositions.first = selectedPositions.second = 0;
	}
};

// Модель элемента выражения
class IBaseExprModel : public std::enable_shared_from_this<IBaseExprModel> {
protected:
	std::weak_ptr<IBaseExprModel> parent;
	CRect rect;
	CDrawParams params;
	int depth;		// Расстояние до корня в дереве

	IBaseExprModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent, int depth = 0 ) :
		parent( parent ),
		rect( rect ),
		depth( depth )
	{
	}
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
	virtual void SetRect( const CRect& rect );
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
	virtual std::list<std::pair<std::wstring, CRect>> GetSelectedText() const;
	virtual std::list<std::pair<std::wstring, CRect>> GetUnselectedText() const;

	// Возвращает набор линий, которые нужно провести на вьюшке, относящейся к этой модели
	virtual std::list<CLine> GetLines() const;

	// Говорит, подсвечен ли прямоугольник этого контрола
	virtual bool IsHighlighted() const;
	virtual void HighlightingOff();
	virtual void HighlightingOn();
	
	// Говорит, выделен ли прямоугольник
	virtual bool IsSelected() const;
	virtual void DeleteSelection();

	// Возвращает тип модели
	virtual ViewType GetType() const = 0;

	// Сдвигает каретку в нужную сторону относительно from
	virtual void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false ) = 0;
	virtual void MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false ) = 0;

	// Говорит, нужно ли двигаться вправо, чтобы попасть от первой модели ко второй
	virtual bool IsSecondModelFarther( const IBaseExprModel* model1, const IBaseExprModel* model2 ) const = 0;

	virtual bool IsEmpty() const = 0;

	virtual int GetDepth() const;

	virtual void UpdateSelection() = 0;
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

inline void IBaseExprModel::SetRect( const CRect& rect )
{
	this->rect = rect;
}

inline void IBaseExprModel::MoveBy( int dx, int dy ) 
{
	rect.MoveBy( dx, dy );
}

inline std::list<CLine> IBaseExprModel::GetLines() const 
{
	return params.polygon;
}

inline bool IBaseExprModel::IsHighlighted() const
{
	return params.isHighlighted;
}

inline void IBaseExprModel::HighlightingOff()
{
	params.isHighlighted = false;
}

inline void IBaseExprModel::HighlightingOn()
{
	params.isHighlighted = true;
}

inline bool IBaseExprModel::IsSelected() const {
	return params.isSelected;
}

inline std::list<std::pair<std::wstring, CRect>> IBaseExprModel::GetSelectedText( ) const 
{
	return std::list<std::pair<std::wstring, CRect>>();
}

inline std::list<std::pair<std::wstring, CRect>> IBaseExprModel::GetUnselectedText( ) const 
{
	return std::list<std::pair<std::wstring, CRect>>();
}

inline void IBaseExprModel::DeleteSelection() {
	params.isSelected = false;
}

inline int IBaseExprModel::GetDepth() const {
	return depth;
}
