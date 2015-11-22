#pragma once
#include <string>
#include <vector>
#include <functional>
#include "./pugixml/pugixml.hpp"
#include "Operation.h"

class MathMlCalculator {
public:
	MathMlCalculator() {}
	MathMlCalculator( const wchar_t* formulaPath, bool is2D = false );

	void RecalculatePoints();
	void RecalculatePoints( int gridSize );

	//double countExpression(double xArgument, double yArgument);
	double GetX( int i, int j ) const;
	double GetY( int i, int j ) const;
	std::vector<double> GetZ( int i, int j ) const;

	bool Is2D();

	int GetGridSize() const;

	double getXMax() const;
	double getYMax() const;
	double getZMax() const;
	
private:
	void buildFormulas(const pugi::xml_node& formulaRoot);
	bool buildCoordFormula(const pugi::xml_node& coordRoot);
	double getFirstArg( int i, int j );
	double getSecondArg( int i, int j );
	double calculateMax(const std::vector<std::vector<double>>& points) const;
	double calculateZMax( const std::vector<std::vector<std::vector<double>>>& points ) const;

	// Ищет корень уравнения zFormula(z) / func(z) = 0
	bool findRoot( const std::function<double(double)>& func, double& root );
	// Ищет все корни уравнения zFormula(z) = 0
	std::vector<double> findAllZRoots();

	std::function<double()> xFormula;
	std::function<double()> yFormula;
	std::function<double()> zFormula;

	int gridSize;

	bool is2D = false;
	bool isImplisit = false;
	bool isParametric = false;

	static pugi::xml_document doc;
	static double eps;

	std::vector<std::vector<double>> xPoints;
	std::vector<std::vector<double>> yPoints;
	std::vector<std::vector<std::vector<double>>> zPoints; // z(x, y) может принимать несколько значений
};