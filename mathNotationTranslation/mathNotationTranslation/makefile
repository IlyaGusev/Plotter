all:
	bison -d --report all --report-file y.out bison.y
	flex lexer.l
	g++ -std=c++11 lex.yy.c Node.h bison.tab.c main.cpp
	rm -f Node.h.gch variableNode.h.gch arithmeticNode.h.gch Parser.h.gch lex.yy.cc bison.tab.h.gch bison.tab.h bison.tab.c lex.yy.c
