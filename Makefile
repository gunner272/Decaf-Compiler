build:
	@read -p "Enter Program Name:" prog; \
	bison -d parse.ypp; \
	flex lexcp.l; \
	g++ -o compiler parse.tab.cpp lex.yy.c `llvm-config --libs core jit native --cxxflags` `llvm-config --ldflags`; \
	./compiler < $$prog;
