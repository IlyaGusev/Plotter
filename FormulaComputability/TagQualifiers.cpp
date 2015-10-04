#include "TagQualifiers.h"


CTagQualifiers::CTagQualifiers(int _type)
{
    type = QUALIFIER | _type;
}

void CTagQualifiers::operator()( const CNode& node ) const
{
    auto arg = node.first_child();
    auto argTag = CTagContainer::getTag( arg.name() );
    int argType = argTag->getType();
    if ( !( argType & NUMBER | argType & VARIABLE ) ) {
        throwException( node.name(), arg.offset_debug(), INVALID_ARGUMENT );
    }
    //инициируем проверку дочерних тэгов
    (*argTag)(arg);
    //проверяем следующие аргументы
    arg = argTag->checkSignature(arg);
    if ( !arg.empty() ) {
        throwException( node.name(), arg.offset_debug(), INVALID_ARGUMENT );
    }
}