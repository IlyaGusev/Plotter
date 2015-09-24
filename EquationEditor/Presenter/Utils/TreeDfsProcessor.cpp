#include "Model/IBaseExprModel.h"
#include "Presenter/Utils/TreeDfsProcessor.h"

CTreeDfsProcessor::CTreeDfsProcessor(
	Node startingNode,
	const std::function<void( Node )>& afterEnter,
	const std::function<void( Node, Node )>& beforeEachChild,
	const std::function<bool( Node, Node )>& condition,
	const std::function<void( Node, Node )>& afterEachChild,
	const std::function<void( Node )>& beforeExit) :
	_startingNode( startingNode ),
	_afterEnter( afterEnter ),
	_beforeEachChild( beforeEachChild ),
	_condition( condition ),
	_afterEachChild( afterEachChild ),
	_beforeExit( beforeExit )
{
}

void CTreeDfsProcessor::SetStartingNode( Node startingNode )
{
	_startingNode = startingNode;
}

void CTreeDfsProcessor::SetEnterProcessFunc( const std::function<void( Node )>& afterEnter )
{
	_afterEnter = afterEnter;
}

void CTreeDfsProcessor::SetBeforeChildProcessFunc( const std::function<void( Node, Node )>& beforeEachChild )
{
	_beforeEachChild = beforeEachChild;
}

void CTreeDfsProcessor::SetCondition( const std::function<bool(Node, Node)>& condition )
{
	_condition = condition;
}

void CTreeDfsProcessor::SetAfterChildProcessFunc( const std::function<void( Node, Node )>& afterEachChild )
{
	_afterEachChild = afterEachChild;
}

void CTreeDfsProcessor::SetExitProcessFunc( const std::function<void( Node )>& beforeExit )
{
	_beforeExit = beforeExit;
}

void CTreeDfsProcessor::dfsFunction( Node currentNode ) const
{
	_afterEnter(currentNode);
	for (auto child : currentNode->GetChildren())
	{
		_beforeEachChild(currentNode, child);
		if (_condition( currentNode, child ))
		{
			dfsFunction( child );
		}
		_afterEachChild(currentNode, child);
	}
	_beforeExit(currentNode);
}

std::shared_ptr<IBaseExprModel> CTreeDfsProcessor::Find(
	const std::function<bool(Node)>& predicate,
	const std::function<bool(Node, Node)>& hint ) const
{
	if( _startingNode == nullptr ) {
		throw std::exception( "starting node is not initialized" );
	}
	std::function<Node(Node)> findFunction = [&]( Node currentNode ) -> Node
	{
		if( predicate( currentNode ) ) {
			return currentNode;
		}
		for( auto child : currentNode->GetChildren( ) ) {
			if( hint( currentNode, child ) ) {
				auto result = findFunction( child );
				if( result != nullptr ) {
					return child;
				}
			}
		}
		return nullptr;
	};
	
	return findFunction( _startingNode );
}

std::list<CTreeDfsProcessor::Node> CTreeDfsProcessor::FindAll(
	const std::function<bool(Node)>& predicate,
	const std::function<bool(Node, Node)>& hint ) const
{
	if( _startingNode == nullptr ) {
		throw std::exception( "starting node is not initialized" );
	}

	std::list<Node> result;
	std::function<void(Node)> findFunction = [&]( Node currentNode )
	{
		if( predicate( currentNode ) ) {
			result.push_back( currentNode );
		}
		for( auto child : currentNode->GetChildren( ) ) {
			if( hint( currentNode, child ) ) {
				findFunction( child );
			}
		}
	};

	return result;
}

void CTreeDfsProcessor::Process( ) const
{
	if (_startingNode == nullptr)
	{
		throw std::exception( "starting node is not initialized" );
	}
	dfsFunction( _startingNode );
}