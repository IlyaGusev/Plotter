#ifndef _TAGAPPLY_H
#define _TAGAPPLY_H
#include "CTag.h"
#include "CTree.h"
#include "CTagCi.h"
#include <typeinfo>

//both Apply (= CTagApplyReln<NUMBER>) and Reld (= CTagApplyReln<BOOL>)
template<CType TFunc>
class CTagApplyReln : public CTagAtomic {
public:
	CTagApplyReln();
	virtual void operator ()( const CNode& node ) const;
	virtual void operator ()( const CNode& node, CTreeNode& tree_node );
protected:
	static void enterToAllLimitableArgs( const CNode& node );//обход всех дочерних вершин для функций sum, product
	static void enterToAllLimitableArgs( const CNode& node, CTreeNode& tree_node );
};


template<CType TFunc>
CTagApplyReln< TFunc>::CTagApplyReln( )
{
	type = TFunc | SPECIAL;
}

template<CType TFunc>
void CTagApplyReln< TFunc>::operator ()( const CNode& node )const
{
	hasNoAttributes( node );
	hasNoText( node );

	auto child = node.first_child();
	const CTag& func = CTagContainer::getTag(child.name());
	if ( !(( func.type & CALCULATEBLE) && (func.type & TFunc )) ) {
		throwException( node, child.offset_debug(), INVALID_ARGUMENT );
	}
	child = func.checkSignature(child);
	if ( !(child.empty()) ) {
		throwException( node, child.offset_debug(), INVALID_ARGUMENT );
	}

	if (! (func.getType() & LIMITABLE )) {
		enterToAllChilds( node );
	}
	else {
		//limitable-функции требуют особого обхода
		enterToAllLimitableArgs( node );
	}
}

template<CType TFunc>
void CTagApplyReln< TFunc>::operator ()( const CNode& node, CTreeNode& tree_node)
{
	auto child = node.first_child();
	const CTag& func = CTagContainer::getTag(child.name());
	CTreeNode& next_node = tree_node.Step(child.name());
	func.checkTree(child, next_node);

	if (! (func.getType() & LIMITABLE )) {
		enterToAllChilds( node, next_node);
	}
	else {
		enterToAllLimitableArgs( node, next_node );
	}
}

template<CType TFunc>
void CTagApplyReln< TFunc>::enterToAllLimitableArgs( const CNode& node )
{
	//предполагаем, что все теги корректно заданы
	CNode child = node.first_child();

	//узел, содержащий информацию о переменной, по которой будет выполнено взятие суммы или произведения
	CNode bVar = child.next_sibling().first_child();
	while ( !child.empty() ) {
		CTag& childTag = CTagContainer::getTag( child.name() );
		//информация об итерируемой переменной должны быть доступна в теге <condition>
		if ( childTag.getType() & CONDITION ) {
			CTagCi::AddIdentifier( bVar, BOUND | VARIABLE | TFunc );
		}
		childTag(child);
		//информация о переменной интегрирования не должна быть доступна ранее обработки тега <uplimit>
		if ( childTag.getType() & LIMIT_UP ) {
			CTagCi::AddIdentifier( bVar, BOUND | VARIABLE | TFunc );
		}
		child = child.next_sibling();
	}
	//удаляем итерируемую переменную из области видимости
	CTagCi::deleteIdentifier( bVar.text().as_string() );
}

template<CType TFunc>
void CTagApplyReln< TFunc>::enterToAllLimitableArgs( const CNode& node, CTreeNode& tree_node )
{
	//предполагаем, что все теги корректно заданы
	CNode child = node.first_child();
	//узел, содержащий информацию о переменной, по которой будет выполнено взятие суммы или произведения
	CNode bVar = child.next_sibling().first_child();
	while ( !child.empty() ) {
		CTag& childTag = CTagContainer::getTag( child.name() );
		if ( childTag.getType() & CONDITION ) {
			CTagCi::AddIdentifier( bVar, BOUND | VARIABLE | TFunc );
		}
		childTag(child, tree_node);
		if ( childTag.getType() & LIMIT_UP ) {
			CTagCi::AddIdentifier( bVar, BOUND | VARIABLE | TFunc );
		}
		child = child.next_sibling();
	}
	//удаляем итерируемую переменную из области видимости
	CTagCi::deleteIdentifier( bVar.text().as_string() );
}


#endif
