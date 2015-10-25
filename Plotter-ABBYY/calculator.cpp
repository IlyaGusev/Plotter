#include "calculator.h"
#include <string>
#include <exception>
#include <stack>

//StringCalculator::StringCalculator(const std::string &_formula) :
//	formula(_formula)
//{
//}
//
////получение числа для выполнения операции
//double StringCalculator::getOperand() {
//	if (numbers.empty()) {
//		throw std::logic_error("wrong expression");
//	}
//	double result = numbers.top();
//	numbers.pop();
//
//	return result;
//}
//
////подсчет верхней в стеке операции
//void StringCalculator::countOperation() {
//	std::string operation;
//	if (!(operations.empty())) {
//		operation = operations.top();
//		operations.pop();
//	} else {
//		throw std::logic_error("wrong expression"); //попытка выполнить несуществующую операцию
//	}
//
//	if (operation == "(") {
//		throw std::logic_error("mismatched brackets");
//	}
//
//	double secondOperand = getOperand();
//
//	if (operation == "!") {
//		numbers.push( 1 /*getFactorial(secondOperand)*/ );
//		return;
//	}
//
//	double firstOperand = getOperand();
//
//	//подсчет значения операции для полученных операндов
//	if (operation == "+") {
//		numbers.push(firstOperand + secondOperand);
//	} else if (operation == "-") {
//		numbers.push(firstOperand - secondOperand);
//	} else if (operation == "*") {
//		numbers.push(firstOperand * secondOperand);
//	} else if (operation == "/") {
//		numbers.push(firstOperand / secondOperand);
//	}
//}
//
////получение приоритета операции
//int StringCalculator::getPriority(std::string &operation) {
//	if ((operation == "+") || (operation == "-")) {
//		return 1;
//	} else if ((operation == "*") || (operation == "/")) {
//		return 2;
//	} else {
//		return 0;
//	}
//}
//
////подсчет значения выражения
//double StringCalculator::countExpression(double xArgument, double yArgument) {
//	size_t posInArgument = 0;
//	while (posInArgument < formula.length()) {
//		std::string currentToken = "";
//		currentToken += formula[posInArgument];
//
//		if ((currentToken >= "0") && (currentToken <= "9")) {
//			while ((posInArgument + 1 < formula.length()) && ((formula[posInArgument + 1] == '.')
//				|| ((formula[posInArgument + 1] >= '0') && (formula[posInArgument + 1] <= '9')))) {
//				currentToken += formula[posInArgument + 1];
//				++posInArgument;
//			}
//			double currentNumber = stod(currentToken);
//			numbers.push(currentNumber);
//		} else if (currentToken == "x") {
//			numbers.push(xArgument);
//		} else if (currentToken == "y") {
//			numbers.push(yArgument);
//		} else if (currentToken == "(") {
//			operations.push("(");
//		} else if (currentToken == ")") {
//			while (!(operations.empty()) && !(operations.top() == "(")) {
//				countOperation();
//			}
//
//			if (!(operations.empty())) {
//				operations.pop();
//			} else {
//				throw std::logic_error("mismatched brackets");
//			}
//		} else if ((currentToken == "+") || (currentToken == "-") || (currentToken == "*") || (currentToken == "/")) {
//			while (!(operations.empty()) && (getPriority(operations.top()) >= getPriority(currentToken))) {
//				countOperation();
//			}
//			operations.push(currentToken);
//		} else if (currentToken == "!") {
//			operations.push("!");
//		} else if (!(currentToken == " ")) {
//			throw std::logic_error("wrong expression");
//		}
//
//		++posInArgument;
//	}
//
//	while (!operations.empty()) {
//		countOperation();
//	}
//
//	if (numbers.empty()) {
//		throw std::logic_error("wrong expression");
//	} else {
//		double result = numbers.top();
//		numbers.pop();
//		if (!numbers.empty()) {
//			throw std::logic_error("wrong expression");
//		} else {
//			return result;
//		}
//	}
//}

pugi::xml_document MathMlCalculator::doc;

MathMlCalculator::MathMlCalculator( const wchar_t* formulaPath, bool _is2D ) :
	is2D( _is2D ),
	scale( 1 ),
	globalXShift( 0 ),
	globalYShift( 0 ),
	globalZShift( 0 )
{
	pugi::xml_parse_result result = doc.load_file( formulaPath );
	buildFormula( doc );
}

void MathMlCalculator::RecalculatePoints()
{
	RecalculatePoints( gridSize );
}

void MathMlCalculator::RecalculatePoints( int _gridSize )
{
	gridSize = _gridSize;
	xPoints.resize( gridSize );
	yPoints.resize( gridSize );
	zPoints.resize( gridSize );
	for( int i = 0; i < gridSize; i++ ) {
		if( !is2D ) {
			zPoints[i].resize( gridSize );
			for( int j = 0; j < gridSize; j++ ) {
				OperationHandler::setVar( "x", GetX(i, j) );
				OperationHandler::setVar( "y", GetY(i, j) );
				zPoints[i][j] = scale * ( zFormula() - globalZShift );
			}
		} else {
			zPoints[i].resize( 1 );
			OperationHandler::setVar( "x", GetX(i, 0) );
			zPoints[i][0] = scale * (zFormula() - globalZShift);
		}
	}
}

double MathMlCalculator::GetX( int i, int j )
{
	return (double) (i - gridSize / 2) / 4;
}

double MathMlCalculator::GetY( int i, int j )
{
	return (double) (j - gridSize / 2) / 4;
}

double MathMlCalculator::GetZ( int i, int j )
{
	return zPoints[i][j];
}

double& MathMlCalculator::GlobalXShift()
{
	return globalXShift;
}

double& MathMlCalculator::GlobalYShift()
{
	return globalYShift;
}

double& MathMlCalculator::GlobalZShift()
{
	return globalZShift;
}

double& MathMlCalculator::Scale()
{
	return scale;
}

bool MathMlCalculator::Is2D()
{
	return is2D;
}

void MathMlCalculator::buildFormula( const pugi::xml_node& formulaRoot )
{
	zFormula = [&]() -> double {
		pugi::xml_node curArg = formulaRoot.first_child();
		return OperationHandler::getOperation(curArg.name()).build(curArg);
	};
}

int MathMlCalculator::GetGridSize()
{
	return gridSize;
}

//double MathMlCalculator::countExpression(double xArgument, double yArgument) {
//	OperationHandler::setVar("x", xArgument);
//	OperationHandler::setVar("y", yArgument);
//	return zFormula(xArgument, yArgument);
//}