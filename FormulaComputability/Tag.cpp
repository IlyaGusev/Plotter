#include "Tag.h"
#include "TagCi.h"

CTag::CTag()
{
}

CTag::~CTag()
{
}

string CTag::deleteSpaces(const string& s)//delete white spaces in the begin and end of the string
{
	int begin = 0, end = s.length()-1;
	while (s[begin] == ' ')
		++begin;
	while (s[end] == ' ')
		--end;
	return s.substr(begin, end - begin + 1);
}

void CTag::enterToAllChilds(const CNode& node)
{
	
    auto child = node.first_child();
    CNode boundNode;
	while (! child.empty())
	{
        CTag& childTag = CTagContainer::getTag( child.name() );
        if ( childTag.getType() & BOUND ) {
            CTagCi::AddIdentifier(child.first_child(), BOUND);
            boundNode = child.first_child();
        } else {
           ( childTag )( child ); 
        }
		child = child.next_sibling();
	} 
    if ( !boundNode.empty() ) {
        CTagCi::deleteIdentifier( boundNode.text().as_string() );
    }
}


void CTag::hasNoAttributes(const CNode& node) const
{
	if ( node.attributes().begin() != node.attributes().end() ) {
        throwException(node, node.offset_debug(), INVALID_ATTRIBUTE);
    }
}

void CTag::checkAttributes(const CNode& node, const set<string>& attributes) const
{
	string errorMessage;
	for ( auto it = node.attributes_begin(); it != node.attributes_end(); ++it ) {
		if ( attributes.find(it->name()) == attributes.end() ) {
			errorMessage += string( it->name() ) + ' ';
        }
    }
	if ( !errorMessage.empty() ) {
        throwException(node, node.offset_debug(), UNKNOWN_ATTRIBUTE);
    }
}

void CTag::hasNoText(const CNode& node) const
{
    //this tag can't have any value
	if ( !node.text().empty() ) {
		throwException(node, node.offset_debug(), UNEXPECTED_VALUE );
    }
}

void CTag::hasNoChilds(const CNode& node)
{
	if (node.children().begin() != node.children().end()) {
		throwException(node, node.offset_debug(), UNEXPECTED_CHILD);
    }
}

void CTag::hasNChilds(const CNode& node, int N)const
{
	int i = 0;
	CNode child = node.first_child();
	while (!child.empty())
	{
		++i;
		child = child.next_sibling();
	}
	if (i != N)
		throwException(node, child.offset_debug(), UNEXPECTED_CHILD);
}

const CNode CTag::checkArgumentType(const CNode& node, int requiredType) const
{
    if ( node.empty() ) {
        throwException(node.parent(), node.offset_debug(), INVALID_ARGUMENT);
    }
    CTag& curArgTag = CTagContainer::getTag(node.name());
    //проверяем, что заявлена переменная требуемого типа
    if ( !(curArgTag.getType() & requiredType) ) {
        throwException(node.parent(), node.offset_debug(), INVALID_ARGUMENT);
    }
    return curArgTag.checkSignature(node);
}

void CTag::throwException(const CNode& errorTag, int position, ErrorType errType)
{   
	static_assert(ATTRIBUTE_REQUiRED == 9, "add new enum value to throwException");
    string errorMsg;
    CNode rootErrorTag = errorTag;//родитель тега, где обнаружена ошибка
    //по умолчанию это errorTag, специальные случаи обрабатываются в switch
    switch (errType) {
        case INVALID_ARGUMENT:
            errorMsg += "invalid argument";
            break;
        case NO_ARGUMENT:
            errorMsg += "arguments are required after operation-tag " + string( errorTag.name() );
            rootErrorTag = errorTag.parent();
            break;
        case UNKNOWN_ATTRIBUTE:
            errorMsg += "unknown attribute(s): ";
            break;
        case INCORRECT_VALUE:
            errorMsg += "incorrect value ";
            break;
        case INVALID_ATTRIBUTE:
            errorMsg += "invalid attribute";
            break;
        case UNEXPECTED_VALUE:
            errorMsg += "this tag can't have any value";
            break;
        case UNEXPECTED_CHILD:
            errorMsg += "this tag can't have any child";
            break;
		case INVALID_ATTRIBUTE_ARGUMENT:
			errorMsg += "invalid attribute value";
			break;			
		case ATTRIBUTE_REQUiRED:
			errorMsg += "attribute requered";
			break;
		case IDENTIFIER_ALREADY_EXIST:
			errorMsg += "identifier has already exist";
			break;
		case UNKNOWN_IDENTIFIER:
			errorMsg += "unknown identifier";
			break;
        default:
#ifdef _DEBUG
			throw invalid_argument("invalid enum value in throwException");
#else
            break;
#endif
    }
    errorMsg += " in tag " + string( rootErrorTag.name() ) + "\nposition: " + to_string(position);

	throw invalid_argument( errorMsg );
};

const string& CTag::getName() const 
{
    return name;
}
CType CTag::getType() const 
{
    return type;
}

const CNode CTagAtamar::checkSignature(const CNode& node) const
{
	return node.next_sibling();
}

CTagBVar::CTagBVar()
{
    type = BOUND;
}

void CTagBVar::operator()(const CNode& node) const 
{
    //hasNoText(node);
    CNode ident = node.first_child();
    CTag& identTag = CTagContainer::getTag( ident.name() );
    //проверяем, что первый дочерний тэг - переменная
    if ( !( identTag.getType() & VARIABLE ) ) {
        throwException( node, ident.offset_debug(), INVALID_ARGUMENT );
    }
    //проверка дочерних тэгов
    string identName(deleteSpaces(ident.text().as_string()));
    if (identName.empty()) {
        throwException(node, node.offset_debug(), INCORRECT_VALUE);
    }
    ident = identTag.checkSignature(ident);
    //проверям, что если существует второй дочерний тэг, то им может быть только degree
    if ( !ident.empty() ) {
        CTag& argTag = CTagContainer::getTag( ident.name() );
        if ( !( argTag.getType() & DEGREE ) ) {
            throwException( node, ident.offset_debug(), INVALID_ARGUMENT );
        }
        //инициируем проверку дочерних тэгов
        argTag(ident); 
        //проверяем, что больше дочерних тэгов нет
        ident = argTag.checkSignature(ident);
        if ( !ident.empty() ) {
            throwException(node, ident.offset_debug(), INVALID_ARGUMENT );
        }
    }

}

CTagCondition::CTagCondition() 
{
    type = CONDITION;
}

void CTagCondition::operator()(const CNode& node) const
{
    hasNoText(node);
    auto lastChild = checkArgumentType(node.first_child(), SPECIAL);
    if ( !lastChild.empty() ) {
        throwException(node, lastChild.offset_debug(), INVALID_ARGUMENT);
    }
    enterToAllChilds(node);
}

CTagLimitable::CTagLimitable()
{
    type = LIMITABLE | CALCULATEBLE | NUMBER;
}

const CNode CTagLimitable::checkSignature(const CNode& node) const
{

    CNode nextArg = checkArgumentType(node.next_sibling(), BOUND);
    CTag& nextArgTag = CTagContainer::getTag(nextArg.name());
    if ( !( nextArgTag.getType() & CONDITION ) ) {
        nextArg = checkArgumentType( checkArgumentType( nextArg, LIMIT_LO ), LIMIT_UP );
    } else {
        nextArg = nextArg.next_sibling();
    }
    nextArg = checkArgumentType(nextArg, SPECIAL | NUMBER | VARIABLE);
    return nextArg;
}

void CTagLimitable::operator()(const CNode& node) const
{
    hasNoAttributes(node);
    hasNoChilds(node);
    hasNoText(node);
}
