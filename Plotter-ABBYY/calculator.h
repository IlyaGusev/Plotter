#pragma once
#include <string>
#include <stack>
#include <functional>
#include "./pugixml/pugixml.hpp"

class Calculator {
public:
	virtual double countExpression(double xArgument, double yArgument) = 0;
};


class StringCalculator : public Calculator  {
public:
	//конструктор калькулятора
	StringCalculator(const std::string &_formula);

	//подсчет значения выражения
	double countExpression(double xArgument, double yArgument);
	
private:
	std::string formula;
	std::stack<double> numbers; //стек для чисел
	std::stack<std::string> operations; //стек для операций

	//получение числа для выполнения операции
	double getOperand();

	//подсчет верхней в стеке операции
	void countOperation();

	//получение приоритета операции
	int getPriority(std::string &operation);
};

class MathMlCalculator: Calculator {
public:
	MathMlCalculator(const pugi::xml_node& formulaRoot);
	double countExpression(double xArgument, double yArgument);
private:
	void buildFormula(const pugi::xml_node& formulaRoot);
	std::function<double(double, double)> formula;
};