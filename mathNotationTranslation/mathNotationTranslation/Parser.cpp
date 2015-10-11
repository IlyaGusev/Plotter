#include "Parser.h"
#include "arithmeticNode.h"
#include "variableNode.h"
#include "lex.yy.cc"

Parser::Parser() { prev.reset(); }
Parser::~Parser() {}
void Parser::addNode(){}

void Parser::arithmeticOperation(istream& is, int oper, unique_ptr<Node>& position)
{
	position = unique_ptr<arithmeticNode>(new arithmeticNode());
	unique_ptr<arithmeticNode> pDerived(static_cast<arithmeticNode*>(position.release()));
	pDerived->operation = oper;
	position = move(pDerived);
	position.get()->left = move(prev);
	processText(is, position->right);
	position.get()->right = move(prev);
	prev = move(position);
}

bool Parser::processText(istream& is, unique_ptr <Node>& position) {
	int tok = lexer->yylex(&is, &cout);
	switch (tok) {
	case ID:
	case NUMBER:
		position.reset(new variableNode(value));
		prev = move(position);
		return false;
	case LOPER:
	case ROPER:
	case LNUM:
	case RNUM:
	case LID:
	case RID:
		processText(is, position);
		return false;
	case 0:
		return true;
	default:
		arithmeticOperation(is, tok, position);
		return false;
	}
	return true;
}
