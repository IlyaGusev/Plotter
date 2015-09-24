#include <queue>

#include "Presenter/Utils/TreeBfsProcessor.h"
#include "Model/IBaseExprModel.h"

CTreeBfsProcessor::CTreeBfsProcessor(
	Node startingNode,
	const std::function<void( Node )>& afterEnter,
	const std::function<void( Node, Node )>& beforeEachChild,
	const std::function<bool( Node, Node )>& condition,
	const std::function<void( Node, Node )>& afterEachChild,
	const std::function<void( Node )>& beforeExit ) :
	_startingNode( startingNode ),
	_afterEnter( afterEnter ),
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
	if( _startingNode == nullptr ) {
		throw std::exception( "starting node is not initialized" );
	}
	
	std::queue<Node> nodeQueue;
	nodeQueue.push( _startingNode );
	
	while( nodeQueue.size() > 0 ) {
		auto currentNode = nodeQueue.front( );
		nodeQueue.pop( );
		if( predicate( currentNode ) ) {
			return currentNode;
		}
		for( auto child : currentNode->GetChildren( ) ) {
			if( hint( currentNode, child ) ) {
				nodeQueue.push( child );
			}
		}
	}

	return nullptr;
}

std::list<std::shared_ptr<IBaseExprModel>> CTreeBfsProcessor::FindAll( const std::function<bool(Node)>& predicate, const std::function<bool(Node, Node)>& hint ) const
{
	if( _startingNode == nullptr ) {
		throw std::exception( "starting node is not initialized" );
	}
	
	std::list<Node> result;
	std::queue<Node> nodeQueue;
	nodeQueue.push( _startingNode );

	while( nodeQueue.size( ) > 0 ) {
		auto currentNode = nodeQueue.front( );
		nodeQueue.pop( );
		if( predicate( currentNode ) ) {
			result.push_back(currentNode);
		}
		for( auto child : currentNode->GetChildren( ) ) {
			if( hint( currentNode, child ) ) {
				nodeQueue.push( child );
			}
		}
	}

	return result;
}

void CTreeBfsProcessor::Process( ) const
{
	if( _startingNode == nullptr ) {
		throw std::exception( "starting node is not initialized" );
	}

	std::queue<Node> nodeQueue;
	nodeQueue.push( _startingNode );
	while( nodeQueue.size( ) > 0 ) {
		auto currentNode = nodeQueue.front( );
		nodeQueue.pop( );
		_afterEnter( currentNode );
		for( auto child : currentNode->GetChildren( ) ) {
			_beforeEachChild( currentNode, child );
			if( _condition( currentNode, child ) ) {
				nodeQueue.push( child );
			}
			_afterEachChild( currentNode, child );
		}
		_beforeExit( currentNode );
	}
}