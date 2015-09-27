#include <functional>
#include <memory>
#include <list>

class IBaseExprModel;

// класс, который обходит в ширину дерево и совершает над ним необходимые действия
// сделан отдельно метод для удобного поиска
class CTreeBfsProcessor {
public:
	typedef std::shared_ptr<IBaseExprModel> Node;

private:
	Node _startingNode;
	std::function<void( Node )> _afterEnter;
	std::function<void( Node, Node )> _beforeEachChild;
	std::function<bool( Node, Node )> _condition;
	std::function<void( Node, Node )> _afterEachChild;
	std::function<void( Node )> _beforeExit;

public:
	// при вызове process процессор начинает обход в ширину с startingNode вниз
	// при входе в вершину node вызывается afterEnter(node)
	// перед тем как положить в очередь ребёнка node, вершину child, выполняется beforeEachChild(node, child)
	// если condition(node, child) = false, то child не попадает в очередь
	// после того как child положили в очередь, выполняется afterEachChild(node, child)
	// перед выходом из вершины выполняется beforeExit(node)
	CTreeBfsProcessor(
		Node startingNode = nullptr,
		const std::function<void( Node )>& afterEnter = std::function<void( Node )>( []( Node arg1 ){} ),
		const std::function<void( Node, Node )>& beforeEachChild = std::function<void( Node, Node )>( []( Node arg1, Node arg2 ){} ),
		const std::function<bool( Node, Node )>& condition = std::function<bool( Node, Node )>( []( Node arg1, Node arg2 ){return true;} ),
		const std::function<void( Node, Node )>& afterEachChild = std::function<void( Node, Node )>( []( Node arg1, Node arg2 ){} ),
		const std::function<void( Node )>& beforeExit = std::function<void( Node )>( []( Node arg1 ){} ) );

	void SetStartingNode( Node startingNode );
	void SetEnterProcessFunc( const std::function<void( Node )>& afterEnter );
	void SetBeforeChildProcessFunc( const std::function<void( Node, Node )>& beforeEachChild );
	void SetCondition( const std::function<bool( Node, Node )>& condition );
	void SetAfterChildProcessFunc( const std::function<void( Node, Node )>& afterEachChild );
	void SetExitProcessFunc( const std::function<void( Node )>& beforeExit );

	// функция ищет самую первую в порядке обхода в ширину вершину в дереве, для которой predicate(node) = true
	// обход при этом не затрагивает вершины, для которых hint(node, child) = false (т.е. и их потомков тоже)
	Node Find(
		const std::function<bool( Node )>& predicate,
		const std::function<bool( Node, Node )>& hint = std::function<bool( Node, Node )>( []( Node arg1, Node arg2 ){return true;} ) ) const;
	// функция ищет обходом в ширину все вершины в дереве, для которых predicate(node) = true
	// обход при этом не затрагивает вершины, для которых hint(node, child) = false (т.е. и их потомков тоже)
	std::list<Node> FindAll(
		const std::function<bool( Node )>& predicate,
		const std::function<bool( Node, Node )>& hint = std::function<bool( Node, Node )>( []( Node arg1, Node arg2 ){return true;} ) ) const;

	void Process( ) const;
};