#pragma once
#include "Node.h"
#include <map>
#include <array>
class arithmeticNode :
	public Node
{
public:
	void computeResult(string& result, int notation);
	arithmeticNode();
	~arithmeticNode();
	string Translate(int notation);/*
	void setLeft(unique_ptr <Node> left);
	void setRight(unique_ptr <Node> right);*/

//private:
	unique_ptr <Node> left;
	unique_ptr <Node> right;
	int operation; 
	static map<int, array<string, 3>> createMap()
	{
		map<int, array<string, 3>> m;
		m[ADD] = { " <mo> + </mo> ", "", " + " };
		m[SUB] = { " <mo> - </mo> ", "", " - " };
		m[MUL] = { " <mo> * </mo> ", "", " * " };
		m[DIV] = { " <mo> / </mo> ", "", " / " };
		m[EQ] = { " <mo> = </mo> ", "", " = " };
		m[LFRAC] = { " <mfrac> ", "", " \\frac " };
		m[RFRAC] = { " </mfrac> ", "", "" };
		m[LROW] = { " <mrow> ", "", " { " };
		m[RROW] = { " </mrow> ", "", " } " };
		return m;
	}

	static const map<int, array<string, 3>> notations;
};

