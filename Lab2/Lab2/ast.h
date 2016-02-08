#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

class abstract_astnode
{
public:
	virtual void print () = 0;
	// virtual bool checkTypeofAST() = 0;
protected:
private:
	// typeExp astnode_type;
};
/* base nodes */

class stmt_ast:public abstract_astnode{
public:
	virtual void print()=0;
};



class exp_ast:public abstract_astnode{
public:
	virtual void print()=0;
};

/* stmt nodes */


class ass:public stmt_ast{
public:
	ass();
	ass(exp_ast* l, exp_ast* r);
	exp_ast* left, *right;
	void print();
};


class seq:public stmt_ast{
public:
	seq();
	seq(stmt_ast*);
	seq(vector<stmt_ast*> v);
	vector<stmt_ast*> vec;
	void print();
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
};

class For:public stmt_ast{
public:
	For();
	For(exp_ast* a, exp_ast* b,exp_ast* c,stmt_ast* d);
	exp_ast* a,*b,*c;
	stmt_ast* d;
	void print();
};

class While:public stmt_ast{
public:
	While();
	While(exp_ast* l, stmt_ast* r);
	exp_ast* left;
	stmt_ast* right;
	void print();
};

class If:public stmt_ast{
public:
	If();
	If(exp_ast* , stmt_ast* , stmt_ast*);
	exp_ast* a;
	stmt_ast *b,*c;
	void print();
};

/* end of stmt_ast */
/* operator class */
class oper2:public exp_ast{
public:
	oper2(string s);
	string op;
	void print();
};

class oper1:public exp_ast{
public:
	oper1(string s);
	string op;
	void print();
};

/* begin exp_ast nodes */

class op2:public exp_ast{
public:
	op2(exp_ast* l,exp_ast* r, oper2* p);
	exp_ast* left, *right;
	oper2* op;
	void print();
};

class op1:public exp_ast{
public:
	op1(exp_ast* l,oper1* r);
	exp_ast* child;
	oper1* op;
	void print();
};

class func:public exp_ast{
public:
	func();
	func(exp_ast*);
	func(vector<exp_ast*> l);
	vector<exp_ast*> vec;
	void print();
};

class float_const:public exp_ast{
public:
	float_const(float l);
	float child;
	void print();
};

class int_const:public exp_ast{
public:
	int_const(int l);
	int child;
	void print();
};

class string_const:public exp_ast{
public:
	string_const(string l);
	string child;
	void print();
};

class ref_ast:public exp_ast{

};

/* Ref ast */

class Array:public exp_ast{
	
};

class Identifier:public Array{
public:
	Identifier();
	Identifier(string s);
	string child;
	void print();
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
};