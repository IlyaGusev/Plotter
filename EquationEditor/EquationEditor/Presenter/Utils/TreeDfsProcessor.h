#pragma once

#include <functional>
#include <memory>

class IBaseExprModel;

// класс, который обходит в глубину дерево и совершает над ним необходимые действия
// сделан отдельно метод для удобного поиска
class CTreeDfsProcessor {
public:
	typedef std::shared_ptr<IBaseExprModel> Node;

private:
	Node _startingNode;
	std::function<void( Node )> _afterEnter;
	std::function<void( Node )> _beforeEachChild;
	std::function<bool( Node )> _condition;
	std::function<void( Node )> _afterEachChild;
	std::function<void( Node )> _beforeExit;

	void dfsFunction( Node currentNode ) const;

public:
	// при вызове process процессор начинает обход в глубину с startingNode
	// при входе в вершину node вызывается afterEnter(node)
	// перед рекурсивным входом в ребёнка node, вершину child, выполняется beforeEachChild(child)
	// если condition(child) = false, то заход в child не происходит
	// после выхода из child выполняется afterEachChild(child)
	// перед выходом из вершины выполняется beforeExit(node)
	CTreeDfsProcessor(
		Node startingNode = nullptr, 
		const std::function<void( Node )>& afterEnter = std::function<void( Node )>( [](Node arg1){} ),
		const std::function<void( Node )>& beforeEachChild = std::function<void( Node )>( []( Node arg1 ){} ),
		const std::function<bool( Node )>& condition = std::function<bool( Node )>( []( Node arg1 ){return true;} ),
	const std::function<void( Node )>& afterEachChild = std::function<void( Node )>( []( Node arg1 ){} ),
	const std::function<void( Node )>& beforeExit = std::function<void( Node )>( []( Node arg1 ){} ) );

	void SetStartingNode( Node startingNode );
	void SetEnterProcessFunc( const std::function<void( Node )>& afterEnter );
	void SetBeforeChildProcessFunc( const std::function<void( Node )>& beforeEachChild );
	void SetCondition( const std::function<bool( Node )>& condition );
	void SetAfterChildProcessFunc( const std::function<void( Node )>& afterEachChild );
	void SetExitProcessFunc( const std::function<void( Node )>& beforeExit );

	// функция ищет самую верхнюю вершину в дереве, для которой predicate(node) = true
	// обход при этом не затрагивает вершины, для которых hint(child) = false
	Node Find(
		const std::function<bool( Node )>& predicate,
		const std::function<bool( Node )>& hint = std::function<bool( Node )>( []( Node arg1 ){return true;} ) ) const;
	// функция ищет самую верхнюю вершину в дереве, для которой predicate(node) = true
	// обход при этом не затрагивает вершины, для которых hint(child) = false
	std::list<Node> FindAll(
		const std::function<bool( Node )>& predicate,
		const std::function<bool( Node )>& hint = std::function<bool( Node )>( []( Node arg1 ){return true;} ) ) const;

	void Process() const;
};