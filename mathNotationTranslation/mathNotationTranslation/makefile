all:
	bison -d --report all --report-file y.out bison.y
	flex lexer.l
	g++ -std=c++11 lex.yy.c bison.tab.c Node.h variableNode.h arithmeticNode.h Parser.h  Node.cpp variableNode.cpp arithmeticNode.cpp Parser.cpp main.cpp
	rm -f Node.h.gch variableNode.h.gch arithmeticNode.h.gch Parser.h.gch lex.yy.cc bison.tab.h.gch bison.tab.h bison.tab.c lex.yy.c
