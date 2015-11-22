all:
	bison -d --report all --report-file mathml.out -p ml mathml.y
	flex -P ml mathml.l
	bison -d --report all --report-file tex.out -p tex tex.y
	flex -P tex tex.l
	bison -d --report all --report-file openmath.out -p om openmath.y
	flex -P om openmath.l
	g++ -std=c++11 lex.ml.c lex.om.c lex.tex.c Node.h Node.cpp mathml.tab.c openmath.tab.c tex.tab.c main.cpp
	rm -f Node.h.gch lex.ml.c mathml.tab.h.gch mathml.tab.h mathml.tab.c lex.ml.c tex.tab.c tex.tab.h lex.tex.c lex.om.c openmath.tab.c openmath.tab.h
