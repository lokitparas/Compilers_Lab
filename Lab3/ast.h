#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

#define REG_NUM 3



/***************************** Symbol Table ************************************/

class lsymbol{
public:
	string name;
	vector<string> type_list;
	int offset;
	int width;
	int scope;		
	bool var;		// true for variable , false for functions
	lsymbol();
	void print();
};

class local_symbol_table{
public:
	vector<lsymbol> sym;
	local_symbol_table();
	void print();
};

class gsymbol{
public:
	string name;
	vector<string> type_list;
	int width;
	int param_num;
	int offset;
	int scope;		//scope 0 param 1 local 2 global
	bool var;		// true for variable , false for functions
	local_symbol_table* func_tab;
	gsymbol();
	void print();
	
};

class global_symbol_table{
public:
	vector<gsymbol> sym;
	global_symbol_table();
	void print();
};

// class string_symbol{
// public:
// 	string s;
// 	string lab;
// 	string_symbol();
// }

// class string_symtab{
// public:
// 	vector<string_symbol> sym;
// 	string_symtab();
// 	void print();
// }

/************************** Ast classes begin here ***************************/


class abstract_astnode
{
public:
	virtual void print () =0;
	virtual void gencode(bool,bool) =0;
	vector<string> exp_type;
	int val;
	int label;
};
/* base nodes */

class stmt_ast:public abstract_astnode{
public:
	virtual void print()=0;
	virtual void gencode(bool,bool) =0;
};



class exp_ast:public abstract_astnode{
public:
	virtual void print()=0;
	virtual void match_fncall(string,global_symbol_table*,int)=0;
	virtual void gencode(bool,bool) =0;

	string typecast;
	 string name;
	 int exp_offset;
};

/* stmt nodes */


class ass:public stmt_ast{
public:
	ass();
	ass(exp_ast* l, exp_ast* r);
	exp_ast* left, *right;
	void print();
	void gencode(bool,bool){}

};

class new_ass:public stmt_ast{
public:
	new_ass();
	new_ass(exp_ast* r);
	exp_ast* right;
	void print();
	void gencode(bool,bool);

};

class seq:public stmt_ast{
public:
	seq();
	seq(stmt_ast*);
	seq(vector<stmt_ast*> v);
	vector<stmt_ast*> vec;
	void print();
	void gencode(bool,bool);

};

// class func_list:public stmt_ast{
// public:
// 	func_list();
// 	func_list(stmt_ast* l, stmt_ast* r);
// 	stmt_ast* left;
// 	stmt_ast* right;
// 	void print();
// };


class empty:public stmt_ast{
public:
	
	void print();
};

class Return:public stmt_ast{
public:
	Return();
	Return(exp_ast*);
	exp_ast* child;
	void print();
	void gencode(bool,bool);

};

class For:public stmt_ast{
public:
	For();
	For(exp_ast* a, exp_ast* b,exp_ast* c,stmt_ast* d);
	exp_ast* a,*b,*c;
	stmt_ast* d;
	void print();
		void gencode(bool,bool);

};

class While:public stmt_ast{
public:
	While();
	While(exp_ast* l, stmt_ast* r);
	exp_ast* left;
	stmt_ast* right;
	void print();
		void gencode(bool,bool);

};

class If:public stmt_ast{
public:
	If();
	If(exp_ast* , stmt_ast* , stmt_ast*);
	exp_ast* a;
	stmt_ast *b,*c;
	void print();
		void gencode(bool,bool);

};

/* end of stmt_ast */
/* operator class */
class oper2:public exp_ast{
public:
	oper2(string s);
	string op;
	void print();
	void match_fncall(string,global_symbol_table*,int){}
		void gencode(bool,bool){}

};

class oper1:public exp_ast{
public:
	oper1(string s);
	string op;
	void print();
	void match_fncall(string,global_symbol_table*,int){}
		void gencode(bool,bool){}

};

/* begin exp_ast nodes */

class op2:public exp_ast{
public:
	op2(exp_ast* l,exp_ast* r, oper2* p);
	exp_ast* left, *right;
	oper2* op;
	void print();
	void match_fncall(string,global_symbol_table*,int){}
		void gencode(bool,bool);

};

class op1:public exp_ast{
public:
	op1(exp_ast* l,oper1* r);
	exp_ast* child;
	oper1* op;
	void print();
	void match_fncall(string,global_symbol_table*,int){}
	void gencode(bool,bool);

};

class func:public exp_ast{
public:
	func();
	func(string);
	func(string,exp_ast*);
	func(vector<exp_ast*> l );
	string fun_name;
	vector<exp_ast*> vec;
	void print();
	void match_fncall(string,global_symbol_table*,int);
	void gencode(bool,bool);
	void printfcode();

};



class float_const:public exp_ast{
public:
	float_const(float l);
	float child;
	void print();
	void match_fncall(string,global_symbol_table*,int){}
		void gencode(bool,bool);



};

class int_const:public exp_ast{
public:
	int_const(int l);
	int child;
	void print();
	void match_fncall(string,global_symbol_table*,int){}
		void gencode(bool,bool);



};

class string_const:public exp_ast{
public:
	string_const(string l);
	string child;
	void print();
	void match_fncall(string,global_symbol_table*,int){};
	void gencode(bool,bool);


};

class ref_ast:public exp_ast{
	virtual void gencode(bool,bool)=0;
};

/* Ref ast */

class Array:public exp_ast{
public:
	void match_fncall(string,global_symbol_table*,int){}
	virtual void gencode(bool,bool)=0;

};

class Identifier:public Array{
public:
	Identifier();
	Identifier(string s);
	string child;
	//symbol sym;
	void print();
	void gencode(bool,bool);
	
};

class pointer:public ref_ast{
public:
	pointer();
	pointer(ref_ast* r);
	ref_ast* child;
	void print();
};

class deref:public ref_ast{
public:
	deref();
	deref(ref_ast* r);
	ref_ast* child;
	void print();
};

class Ref:public ref_ast{
public:
	Ref();
	Ref(ref_ast* r);
	ref_ast* child;
	void print();
};

class array_ref:public Array{
public:
	array_ref();
	array_ref(Array* i,exp_ast* v);
	Array* child;
	exp_ast* vec;
	void print();
	void gencode(bool,bool);
};

/* functions */

vector<string> getLocalType(string, local_symbol_table*);
int getLocalOffset(string, local_symbol_table*);
vector<string> getGlobalType(string, global_symbol_table*);
vector<string> getMemType(string id,string struct_name,global_symbol_table* g);

string compatible(exp_ast*,exp_ast*,oper2*);

vector<string> rem_star(vector<string> , int,int);
vector<string> rem_dimension(vector<string>,int);
bool matchType(vector<string>,vector<string>);
bool matchType2(vector<string>,vector<string>);
bool assignCheck(exp_ast* lhs,exp_ast* rhs,oper2*);
int getStructSize(string,global_symbol_table*);
local_symbol_table* getStruct(string s,global_symbol_table* g);
//local_symbol_table* getFntable(string,global_symbol_table*);
// void match_fncall(vector< vector<string> >,string,global_symbol_table*);
void getVarToStack(int offset,string reg);
void getStackToVar(int offset,string reg,int);
void loadFromStack(string);
void storeOnStack(string reg);
void addcode(string s);
void getIntToStack(int num,string reg);
void getFloatToStack(float num,string reg);
int getWidth(vector<string>);
void storeAddress(int off);
void printCode();
int getMemWidth(string id,string struct_name,global_symbol_table* g);
int getLocalWidth(string a,local_symbol_table* l);
int getLabel(int,int);
int getGlobalWidth(string a,global_symbol_table* l);
int getParamWidth(local_symbol_table*);

//stack functions
void rstack_init();
void fstack_init();
void swap();
void fswap();
void setLocalSymtab(local_symbol_table* l);
void dynamic_link();
void function_return();
void function_setup(local_symbol_table* l, global_symbol_table* g);

void operatorCode(string,string,string);
bool cast(exp_ast*);
void float_relational(string,string);
void storeOnStack_float(string reg);
void loadFromStack_float(string reg1);

//syscall
void printfcode();
void printFloat(string reg);
void printInt(string);
exp_ast* addString(string e);
void printData();