all:
	bison -d --report all --report-file mathml.out -p ml mathml.y
	flex -P ml mathml.l
	bison -d --report all --report-file tex.out -p tex tex.y
	flex -P tex tex.l
	g++ -std=c++11 lex.ml.c lex.tex.c Node.h mathml.tab.c tex.tab.c main.cpp
	rm -f Node.h.gch lex.ml.cc mathml.tab.h.gch mathml.tab.h mathml.tab.c lex.ml.c tex.tab.c tex.tab.h lex.tex.c
