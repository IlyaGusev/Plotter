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
	double GetX( int i, int j );
	double GetY( int i, int j );
	double GetZ( int i, int j );

	double& GlobalXShift();
	double& GlobalYShift();
	double& GlobalZShift();
	double& Scale();
	bool Is2D();

	int GetGridSize();

	double getXMax() const;
	double getYMax() const;
	double getZMax() const;
	
private:
	void buildFormulas(const pugi::xml_node& formulaRoot);
	void buildCoordFormula(const pugi::xml_node& coordRoot);
	double getFirstArg( int i, int j );
	double getSecondArg( int i, int j );
	double calculateMax(const std::vector<std::vector<double>>& points) const;

	std::function<double()> xFormula;
	std::function<double()> yFormula;
	std::function<double()> zFormula;

	int gridSize;

	bool is2D = false;
	bool isParametric = false;

	static pugi::xml_document doc;

	std::vector<std::vector<double>> xPoints;
	std::vector<std::vector<double>> yPoints;
	std::vector<std::vector<double>> zPoints;
};