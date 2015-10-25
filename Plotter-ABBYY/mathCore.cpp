﻿//#include "mathCore.h"
//#include <iostream>
//#include <exception>
//
//MathCore::MathCore( const std::wstring& formulaPath, bool _is2D ) :
//	is2D( _is2D )
//{
//	pugi::xml_document doc;
//	pugi::xml_parse_result result = doc.load_file( formulaPath.c_str() );
//	calc = MathMlCalculator( doc );
//}
//
//MathCore::MathCore(const pugi::xml_node& formulaRoot, bool _is2D) : 
//	calc(formulaRoot),
//	is2D( _is2D)
//{
//}
//
////MathCore::~MathCore() {
////}
//
//double MathCore::calculate(double x, double y) {
//	try {
//		return calc.countExpression( x, y );
//	}
//	catch (std::logic_error catchedException) {
//		std::cout << catchedException.what() << ", impossible to count" << '\n';
//	}
//	return 0;
//}
//
////void MathCore::changeWindowCoordinates(double x, double y, double z) {
////	globalXShift = globalXShift + x;
////	globalYShift = globalYShift + y;
////	globalZShift = globalZShift + z;
////}
////
////void MathCore::changeScale(double factor) {
////	scale *= factor;
////}
//
//bool MathCore::Is2D() const
//{
//	return is2D;
//}