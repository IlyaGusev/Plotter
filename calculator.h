#pragma once
#include <string>
#include <stack>

class Calculator {
public:
	//конструктор калькулятора
	Calculator(const std::string &_formula);

	//подсчет значения выражения
	double countExpression(double xArgument, double yArgument);
	
private:
	const std::string formula;
	std::stack<double> numbers; //стек для чисел
	std::stack<std::string> operations; //стек для операций

	//получение числа для выполнения операции
	double getOperand();

	//подсчет верхней в стеке операции
	void countOperation();

	//получение приоритета операции
	int getPriority(std::string &operation);
};