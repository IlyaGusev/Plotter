//#pragma once
#ifndef _TacCn_H
#define _TacCn_H
#include "Tag.h"

//класс для работы с тегом <cn>
class CTagCn : public CTagAtamar
{
	void nodeIsInteger(const CNode& node)const;
	void nodeIsReal(const CNode& node)const;
public:
	CTagCn();
	virtual void operator ()(const CNode& node)const;
};

#endif