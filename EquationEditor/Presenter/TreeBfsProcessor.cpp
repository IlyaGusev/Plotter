#include "Presenter/TreeBfsProcessor.h"

CTreeBfsProcessor::CTreeBfsProcessor(
	Node startingNode,
	const std::function<void( Node )>& afterEnter,
	const std::function<void( Node, Node )>& beforeEachChild,
	const std::function<bool( Node, Node )>& condition,
	const std::function<void( Node, Node )>& afterEachChild,
	const std::function<void( Node )>& beforeExit ) :
	_startingNode( startingNode ),
	_beforeEachChild( beforeEachChild ),
	_condition( condition ),
	_afterEachChild( afterEachChild ),
	_beforeExit( beforeExit )
{
}

void CTreeBfsProcessor::SetStartingNode( Node startingNode )
{
	_startingNode = startingNode;
}

void CTreeBfsProcessor::SetEnterProcessFunc( const std::function<void(Node)>& afterEnter )
{
	_afterEnter = afterEnter;
}

void CTreeBfsProcessor::SetBeforeChildProcessFunc( const std::function<void(Node, Node)>& beforeEachChild )
{
	_beforeEachChild = beforeEachChild;
}

void CTreeBfsProcessor::SetCondition( const std::function<bool(Node, Node)>& condition )
{
	_condition = condition;
}

void CTreeBfsProcessor::SetAfterChildProcessFunc( const std::function<void(Node, Node)>& afterEachChild )
{
	_afterEachChild = afterEachChild;
}

void CTreeBfsProcessor::SetExitProcessFunc( const std::function<void(Node)>& beforeExit )
{
	_beforeExit = beforeExit;
}

std::shared_ptr<IBaseExprModel> CTreeBfsProcessor::Find( const std::function<bool( Node )>& predicate, const std::function<bool( Node, Node )>& hint ) const
{
	return nullptr;
}

void CTreeBfsProcessor::Process( ) const
{
	if (_startingNode == nullptr)
	{
		throw std::exception( "starting node is not initialized" );
	}
	
	std::queue<Node> nodeQueue;
	nodeQueue.push( _startingNode );
	while (nodeQueue.size())
	{
		auto currentNode = nodeQueue.front();
		nodeQueue.pop();
		_afterEnter( currentNode );
		for (auto child : currentNode->GetChildren())
		{
			_beforeEachChild( currentNode, child );
			if (_condition( currentNode, child ))
			{
				nodeQueue.push( child );
			}
			_afterEachChild( currentNode, child );
		}
		_beforeExit( currentNode );
	}
}