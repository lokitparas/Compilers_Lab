#include "ast.h"
using namespace std;



//ast.cpp starts

/* stmt nodes */
void empty::print(){
	printf("(Empty)");
}

ass::ass(){
		left=right=NULL;
	};
ass::ass(exp_ast* l, exp_ast* r){
	left=l;
	right=r;
};

void ass::print(){
	if(left==NULL && right==NULL){
		printf("(Empty)");
	}
	else{
		printf("Assign_exp (");
		left->print();
		printf(") (");
		right->print();
		printf(") ");
	}
}

seq::seq(){
	vector<stmt_ast*> v;
	vec=v;
}

seq::seq(stmt_ast* l){
	vector<stmt_ast*> v;
	v.push_back(l);
	vec=v;
}

seq::seq(vector<stmt_ast*> v){
	vec=v;
}

void seq::print(){
	cout<<("(Block [");
	for(int i=0;i<vec.size();i++){
		cout<<"(";
		vec[i]->print();
		cout<<")";
	}
	cout<<"])";
}



Return::Return(exp_ast* c){
	child=c;
}

void Return::print(){
	printf("Return (");
	child->print();
	printf(")");
}

For::For(exp_ast* a1, exp_ast* b1,exp_ast* c1,stmt_ast* d1){
		a=a1;
		b=b1;
		c=c1;
		d=d1;
}

void For::print(){
	printf("For ( ");
	a->print();
	b->print();
	c->print();
	d->print();
	printf(")");
}



While::While(){
	left = NULL;
	right=NULL;
}

While::While(exp_ast* l, stmt_ast* r){
	left = l;
	right = r;
}

void While::print(){
	if(left == NULL && right == NULL){
		printf("(Empty)");
	}
	printf("While (");
	left->print();
	// printf(") (");
	right->print();
	// printf(")");
}


If::If(exp_ast* a1,stmt_ast* b1, stmt_ast * c1){
	a=a1; b=b1;c=c1;
}

void If::print(){
	printf("If(");
	a->print();
	b->print();
	c->print();
	printf(")");
}

/* end of stmt_ast */
/* operator class */


oper2::oper2(string s){
	if( s=="OR_OP") op = "OR ";
	if( s == "AND_OP") op = "AND ";
	if( s == "EQ_OP") op = "EQ_OP ";
	if( s == "NE_OP") op = "NE_OP ";
	if( s == "LT") op = "LT ";
	if( s == "GT") op = "GT ";
	if( s == "LE_OP") op = "LE_OP ";
	if( s == "GE_OP") op = "GE_OP ";
	if( s == "PLUS") op = "PLUS ";
	if( s == "MINUS") op = "MINUS ";
	if( s == "MULT") op = "MULT ";
	if( s == "DIV") op = "DIV ";
	if( s == "ASSIGN") op = "ASSIGN ";
	if(s == "DOT" ) op = "DOT_OP ";
	
}

void oper2::print(){
	cout<<op;
}



oper1::oper1(string s){
	if( s=="UMINUS") op = "UMINUS ";
	else if( s == "NOT") op = "NOT ";
	else if( s == "PP") op = "PP";
	else if( s == "INC_OP") {op = "INC_OP ";}
	else if( s == "PTR_OP") op = "PTR_OP ";
	
	
}

void oper1::print(){
	cout<<op;
}

op2::op2(exp_ast* l,exp_ast* r,oper2* p){
	left=l;
	right=r;
	op=p;
}

void op2::print(){
	op->print();
	cout<<"(";
	left->print();
	cout<<")";
	cout<<"(";
	right->print();
	cout<<")";
}

op1::op1(exp_ast* l,oper1* r){
	child=l;
	op=r;

}

void op1::print(){
	op->print();
	cout<<"(";
	child->print();
		cout<<")";

}	


func::func(){
	vector<exp_ast*> l;
	vec=l;
}

func::func(exp_ast* s){
	vector<exp_ast*> l;
	l.push_back(s);
	vec=l;
}

func::func(vector<exp_ast*> l){
	vec=l;
}

void func::print(){
	printf("FuncCall(");
	for(int i=0;i<vec.size();i++){
		printf("(");
		vec[i]->print();
		printf(")");
	}
	printf(")");

}

float_const::float_const(float l){
	child=l;
}

void float_const::print(){
	printf("FloatConst ");
	printf("%f ",child);
}	



int_const::int_const(int l){
	child=l;
};

void int_const::print(){
	printf("IntConst ");
	cout<<child;
}

string_const::string_const(string l){
	child=l;
};

void string_const::print(){
	printf("StringConst ");
	std::cout << child;	
}



/* Ref ast */

Identifier::Identifier(){
	child = " ";
}
Identifier::Identifier(string s){
	child = s;
}

void Identifier::print(){
	printf("ID \"");
	std::cout << child;
	printf("\"");
}

pointer::pointer(){
	child=NULL;
};

pointer::pointer(ref_ast* r){
	child=r;
};

void pointer::print(){
	printf("Ptr");
	child->print();
}

deref::deref(ref_ast* r){
	child=r;
};

void deref::print(){
	printf("Deref ");
	child->print();
}

Ref::Ref(ref_ast* r){
	child=r;
};

void Ref::print(){
	printf("Ref ");
	child->print();
}

array_ref::array_ref(){
	child=NULL;
	vec=NULL;
};
	
array_ref::array_ref(Array* i,exp_ast* v ){
	child = i;
	vec = v;
}

void array_ref::print(){
	printf("Array ");
	printf("(");
	child->print();
	printf(")");
	printf("(");
	vec->print();
	printf(")");

}