#include "TagQualifiers.h"


CTagQualifiers::CTagQualifiers(int _type)
{
    type = QUALIFIER | _type;
}


void CTagQualifiers::operator()( const CNode& node ) const
{
    auto arg = node.first_child();
    if (arg.empty()) {
        throwException(arg, arg.offset_debug(), MISSED_ARGUMENT);
    }
    CTag& argTag = CTagContainer::getTag( arg.name() );
    int argType = argTag.getType();
    //проверяем тип первого тега
    if ( !( argType & (NUMBER | VARIABLE |SPECIAL) ) ) {
        throwException( arg, arg.offset_debug(), INVALID_ARGUMENT );
    }
    //инициируем проверку дочерних тэгов
    argTag(arg);
    //проверяем, что больше детей нет
    arg = argTag.checkSignature(arg);
    if ( !arg.empty() ) {
        throwException( arg, arg.offset_debug(), INVALID_ARGUMENT );
    }
}
