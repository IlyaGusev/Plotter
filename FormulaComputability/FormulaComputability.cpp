#include "FormulaComputability.h"


FormulaComputability::FormulaComputability()
{
}


FormulaComputability::~FormulaComputability()
{
}

bool FormulaComputability::checkComputability( const CNode& rootNode )
{
    CNode currentNode = rootNode.first_child();
    while ( !currentNode.empty() )
    {
        CTag& currentTag = CTagContainer::getTag(currentNode.name());
        CNode nextNode = currentTag.checkSignature( currentNode );
        currentTag(currentNode);
        currentNode = nextNode;
    }
    return true;
}