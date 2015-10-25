#pragma once
#include <string>
#include <vector>
#include <functional>
#include "./pugixml/pugixml.hpp"
#include "Operation.h"

//class Calculator {
//public:
//	virtual double countExpression( double xArgument, double yArgument ) = 0;
//};


//class StringCalculator : public Calculator  {
//public:
//	//конструктор калькулятора
//	StringCalculator(const std::string &_formula);
//
//	//подсчет значения выражения
//	double countExpression(double xArgument, double yArgument);
//	
//private:
//	std::string formula;
//	std::stack<double> numbers; //стек для чисел
//	std::stack<std::string> operations; //стек для операций
//
//	//получение числа для выполнения операции
//	double getOperand();
//
//	//подсчет верхней в стеке операции
//	void countOperation();
//
//	//получение приоритета операции
//	int getPriority(std::string &operation);
//};

class MathMlCalculator/*: Calculator*/ {
public:
	MathMlCalculator() {}
	MathMlCalculator( const wchar_t* formulaPath );

	void RecalculatePoints( int gridSize );

	//double countExpression(double xArgument, double yArgument);
	double GetX( int i, int j );
	double GetY( int i, int j );
	double GetZ( int i, int j );

	double& GlobalXShift();
	double& GlobalYShift();
	double& GlobalZShift();
	double& Scale();
	bool Is2D();

	int GetGridSize();
private:
	void buildFormula(const pugi::xml_node& formulaRoot);
	std::function<double()> xFormula;
	std::function<double()> yFormula;
	std::function<double()> zFormula;

	int gridSize;

	bool is2D = false;

	std::vector<std::vector<double>> xPoints;
	std::vector<std::vector<double>> yPoints;
	std::vector<std::vector<double>> zPoints;

	// Смещения и увеличение графика
	double globalXShift;
	double globalYShift;
	double globalZShift;
	double scale;
};