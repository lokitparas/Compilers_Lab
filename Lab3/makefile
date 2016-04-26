all:    Scanner.ih Scanner.h Scannerbase.h lex.cc parse.cc main.cc Parser.h Parserbase.h Parser.ih
	g++   --std=c++0x lex.cc parse.cc main.cc ast.cpp -o parser
	./parser < test.c

        
Scanner.ih: lex.l
	if [ ! -e Scanner.ih ]; then \
		flexc++ lex.l;\
                sed -i '/include/a #include "Parserbase.h"' Scanner.ih; \
	fi

parse.cc Parser.h Parserbase.h Parser.ih: parse.y
	bisonc++  --construction parse.y; 
	sed -i '/insert preincludes/a #include "ast.h"' Parserbase.h;
	sed -i '\|STYPE__     d_val__;| d' Parserbase.h;
	# sed -i '\|int     line_num;| d' Parserbase.h;
	sed -i '\|typedef Meta__::SType STYPE__;| a \\tpublic: static STYPE__  d_val__;\n static int line_num; ' Parserbase.h;
	# sed -i '/include "Parser.ih"/a #include "ast.cpp"' parse.cc;
	sed -i '\|void Parser::print__()| i Meta__::SType ParserBase::d_val__; ' parse.cc
	sed -i '\|void Parser::print__()| i int ParserBase::line_num=1; ' parse.cc
	sed -i '\|ParserBase::ParserBase()| i int fn_count=0;bool pointer_lhs=false;bool lvalue= true; vector<int> star_list;  bool rhs=false; vector<bool> address_list;bool address=false;string fname="";string type;string fntype;bool array_constant=false;int loffset,goffset, width;vector< vector<string> > fncall_params;int count_star=0;global_symbol_table* global_sym_tab = new global_symbol_table();local_symbol_table* local_sym_tab = new local_symbol_table();lsymbol* locsym = new lsymbol();gsymbol* gsym = new gsymbol(); int argno;string currfn;string currstruct;  ' parse.cc



.PHONY: clean     
clean:
	$(RM) Parser.ih Parser.h Parserbase.h parse.cc Scanner.ih Scanner.h Scannerbase.h lex.cc a.out graph.ps junk *~

