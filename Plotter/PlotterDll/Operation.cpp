#include "operation.h"
#include <cmath>

double PlusOperation::build(const pugi::xml_node& currentNode) const {
	pugi::xml_node curArg = currentNode.next_sibling();
	if (curArg.empty()) {
		//error
	}
	double result = 0;
	while ( !curArg.empty() ) {
		result += OperationHandler::getOperation(curArg.name()).build(curArg);
		curArg = curArg.next_sibling();
	}
	return result;
}

double MinusOperation::build(const pugi::xml_node& currentNode) const {
	pugi::xml_node minuend = currentNode.next_sibling();
	if (minuend.empty()) {
		//error
	}
	pugi::xml_node subtrahend = minuend.next_sibling();
	if (subtrahend.empty()) {
		//error
	}
	return OperationHandler::getOperation( minuend.name() ).build( minuend ) -
		OperationHandler::getOperation( subtrahend.name() ).build( subtrahend );
}

double TimesOperation::build(const pugi::xml_node& currentNode) const {
	pugi::xml_node curArg = currentNode.next_sibling();
	if (curArg.empty()) {
		//error
	}
	double result = 1;
	while (!curArg.empty()) {
		result *= OperationHandler::getOperation(curArg.name()).build(curArg);
		curArg = curArg.next_sibling();
	}
	return result;
}

double DivideOperation::build( const pugi::xml_node& currentNode ) const
{
	pugi::xml_node firstArg = currentNode.next_sibling();
	if( firstArg.empty() ) {
		//error
	}
	pugi::xml_node secondArg = firstArg.next_sibling();
	if( secondArg.empty() ) {
		//error
	}
	return OperationHandler::getOperation( firstArg.name() ).build( firstArg ) /
		OperationHandler::getOperation( secondArg.name() ).build( secondArg );
}

double PowerOperation::build( const pugi::xml_node& currentNode ) const
{
	pugi::xml_node firstArg = currentNode.next_sibling();
	if( firstArg.empty() ) {
		//error
	}
	pugi::xml_node secondArg = firstArg.next_sibling();
	if( secondArg.empty() ) {
		//error
	}
	return pow( OperationHandler::getOperation( firstArg.name() ).build( firstArg ),
		OperationHandler::getOperation( secondArg.name() ).build( secondArg ) );
}

double RootOperation::build( const pugi::xml_node& currentNode ) const
{
	pugi::xml_node firstArg = currentNode.next_sibling();
	if( firstArg.empty() ) {
		//error
	}
	double degree, argument;
	if( !firstArg.next_sibling().empty() ) {
		pugi::xml_node secondArg = firstArg.next_sibling( );
		if( secondArg.empty() ) {
			//error
		}
		argument = OperationHandler::getOperation( secondArg.name() ).build( secondArg );
		degree = OperationHandler::getOperation( firstArg.name() ).build( firstArg );
	} else {
		argument = OperationHandler::getOperation( firstArg.name() ).build( firstArg );
		degree = 2;
	}
	return pow( argument > 0 ? argument : 0, 1 / degree );
}

double DegreeOperation::build( const pugi::xml_node& currentNode ) const
{
	pugi::xml_node degreeArg = currentNode.first_child();
	if( degreeArg.empty( ) ) {
		//error
	}
	pugi::xml_node numberArg = degreeArg.first_child();
	if( numberArg.empty() ) {
		//error
	}
	return numberArg.text().as_double();
}

double AbsOperation::build( const pugi::xml_node& currentNode ) const
{
	pugi::xml_node arg = currentNode.next_sibling();
	if( arg.empty( ) ) {
		//error
	}
	return abs( OperationHandler::getOperation( arg.name() ).build( arg ) );
}

double SinOperation::build( const pugi::xml_node& currentNode ) const
{
	pugi::xml_node arg = currentNode.next_sibling();
	if( arg.empty() ) {
		//error
	}
	return sin( OperationHandler::getOperation( arg.name() ).build( arg ) );
}

double CosOperation::build( const pugi::xml_node& currentNode ) const
{
	pugi::xml_node arg = currentNode.next_sibling();
	if( arg.empty() ) {
		//error
	}
	return cos( OperationHandler::getOperation( arg.name() ).build( arg ) );
}

double TanOperation::build( const pugi::xml_node& currentNode ) const
{
	pugi::xml_node arg = currentNode.next_sibling();
	if( arg.empty() ) {
		//error
	}
	return tan( OperationHandler::getOperation( arg.name() ).build( arg ) );
}

double CotOperation::build( const pugi::xml_node& currentNode ) const
{
	pugi::xml_node arg = currentNode.next_sibling();
	if( arg.empty() ) {
		//error
	}
	return 1 / tan( OperationHandler::getOperation( arg.name() ).build( arg ) );
}

double Ident::build(const pugi::xml_node& currentNode) const {
	//TODO: var is not found
	return OperationHandler::getVar(currentNode.text().as_string());
}

double Number::build(const pugi::xml_node& currentNode) const {
	//TODO: var is not found
	return currentNode.text().as_double();
}

double ApplyOperation::build(const pugi::xml_node& currentNode) const {
	const pugi::xml_node opNode = currentNode.first_child();
	return OperationHandler::getOperation(opNode.name()).build(opNode);
}

const Operation& OperationHandler::getOperation(const std::string& opName) {
	auto foundOperationIterator = operations.find(opName);
	if (foundOperationIterator == operations.end()) {
		//error
	}
	return *(foundOperationIterator->second);
}


std::map< std::string, std::unique_ptr<Operation> > OperationHandler::fillOperations() {
	std::map< std::string, std::unique_ptr<Operation> > operationsToFill;
	operationsToFill.emplace( "plus", std::unique_ptr<Operation>( new PlusOperation() ) );
	operationsToFill.emplace( "minus", std::unique_ptr<Operation>( new MinusOperation() ) );
	operationsToFill.emplace( "times", std::unique_ptr<Operation>( new TimesOperation() ) );
	operationsToFill.emplace( "divide", std::unique_ptr<Operation>( new DivideOperation() ) );
	operationsToFill.emplace( "power", std::unique_ptr<Operation>( new PowerOperation() ) );
	operationsToFill.emplace( "degree", std::unique_ptr<Operation>( new DegreeOperation() ) );
	operationsToFill.emplace( "root", std::unique_ptr<Operation>( new RootOperation() ) );
	operationsToFill.emplace( "abs", std::unique_ptr<Operation>( new AbsOperation() ) );
	operationsToFill.emplace( "sin", std::unique_ptr<Operation>( new SinOperation() ) );
	operationsToFill.emplace( "cos", std::unique_ptr<Operation>( new CosOperation() ) );
	operationsToFill.emplace( "tan", std::unique_ptr<Operation>( new TanOperation() ) );
	operationsToFill.emplace( "cot", std::unique_ptr<Operation>( new CotOperation() ) );
	operationsToFill.emplace( "ci", std::unique_ptr<Operation>( new Ident() ) );
	operationsToFill.emplace( "cn", std::unique_ptr<Operation>( new Number() ) );
	operationsToFill.emplace( "apply", std::unique_ptr<Operation>( new ApplyOperation() ) );
	return operationsToFill;
}

std::map<std::string, double> OperationHandler::fillVars() {
	std::map<std::string, double> varsToFill;
	varsToFill["x"] = 0;
	varsToFill["y"] = 0;
	varsToFill["z"] = 0;
	varsToFill["t"] = 0;
	varsToFill["u"] = 0;
	return varsToFill;
}

double OperationHandler::getVar(const std::string& varName) {
	return vars[varName];
}

void OperationHandler::setVar(const std::string& varName, double value) {
	vars[varName] = value;
}
std::map< std::string, std::unique_ptr<Operation> > OperationHandler::operations = OperationHandler::fillOperations();
std::map<std::string, double> OperationHandler::vars = OperationHandler::fillVars();