#ifndef _COMMON_H
#define _COMMON_H
#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include <set>
#include <map>
#include <memory>
#include <utility>
#include <sstream>
#include <typeinfo>
#include "../pugixml/pugixml.hpp"

//битовые маски, определяющие типы тегов
#define VOID (~0)
#define NUMBER			0x00000001
#define BOOL			0x00000002
#define VARIABLE		0x00000004
#define FUNCTION		0x00000008
#define CALCULATEBLE	0x00000010
#define LIMIT_LO		0x00000020
#define LIMIT_UP		0x00000040
#define LIMITABLE		0x00000080 //sum, product
#define BOUND 			0x00000100 //bvar
#define QUALIFIER		0x00000200 //lowlimit, uplimit, bvar, degree
#define DEGREE			0x00000400
#define CONDITION		0x00000800
#define SPECIAL			0x00001000

using namespace std;
using namespace pugi;

typedef unsigned int CType;
typedef pugi::xml_node CNode;

#endif
