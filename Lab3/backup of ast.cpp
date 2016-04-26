#include "ast.h"
using namespace std;

global_symbol_table* glob;
stack <string> rstack,fstack;
vector<string> code;
vector <string> string_symtab;
local_symbol_table* loc,*struct_symtab;
int label_count=0,string_label=0,ret_label=0;

lsymbol::lsymbol(){
	name="";
	var=true;
	scope=0;
	vector<string> type_list;
	width = 0;
	offset=0;
}

gsymbol::gsymbol(){
	
}


local_symbol_table::local_symbol_table(){
	vector<lsymbol> v;
	 sym=v;
}


global_symbol_table::global_symbol_table(){
	vector<gsymbol> v;
	// sym=s;
}

void lsymbol::print(){
	cout<<"   --";
	for(int i=0; i<type_list.size(); i++){
		cout<<type_list[i]<<"|";	
	}
	cout<<" "<<name<<" width: "<<width<<" offset: "<<offset<<endl;
}

void local_symbol_table::print(){
	for(int i=0;i<sym.size();i++){
		sym[i].print();
	}
}

void gsymbol::print(){
	for(int i=0; i<type_list.size(); i++){
		cout<<type_list[i]<<"|";	
	}
	cout<<" "<<name<<" width: "<<width<<" offset: "<<offset<<endl;
	func_tab->print();
}

void global_symbol_table::print(){
	for(int i=0;i<sym.size();i++){
		sym[i].print();
	}
}

/************************************* Ast  nodes *************************************/
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

new_ass::new_ass(){
		right=NULL;
	};
new_ass::new_ass(exp_ast* r){
	right=r;
};

void new_ass::print(){
	if(right==NULL){
		printf("(Empty)");
	}
	else{
		// printf("Assign_exp (");
		right->print();
		// printf(") ");
	}
}

void new_ass::gencode(bool addr,bool is_struct){
	right->gencode(addr,is_struct);
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

void seq::gencode(bool addr, bool is_struct){
	for(int i=0;i<vec.size();i++){
		vec[i]->gencode(addr,is_struct);
	}
}

Return::Return(exp_ast* c){
	child=c;
}

void Return::print(){
	printf("Return (");
	child->print();
	printf(")");
}

void Return::gencode(bool addr,bool is_struct){
	child->gencode(false,false);
	int w=getParamWidth(loc)+12;
	int width = getWidth(child->exp_type)/4;
	vector<string> temp;
	for(int i=0; i<width;i++){
		addcode("sw "+rstack.top()+", "+to_string(i*4+w)+"($sp)");
		temp.push_back(rstack.top());
		rstack.pop();
	}

	for(int i=0; i<width;i++){
		rstack.push(temp[width-i-1]);
	}
	addcode("j Ret"+to_string(ret_label));
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

void For::gencode(bool addr,bool is_struct){
	string loop="$L"+to_string(label_count++), test="$L"+to_string(label_count++);
	a->gencode(false,false);
	addcode("j "+test+ "\n\n"+loop+":             #in loop");
	d->gencode(false,false);
	c->gencode(false,false);
	addcode("\n"+test+":                 #test condition");
	b->gencode(false,false);
	if(b->exp_type[0]=="float"){b->typecast ="TO-INT"; cast(b);}
	addcode("bne $0, "+rstack.top()+", "+loop +"            #jump to loop of condition true");
	
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

void While::gencode(bool addr,bool is_struct){
	string loop="$L"+to_string(label_count++), test="$L"+to_string(label_count++);
	addcode("j "+test+ "\n\n"+loop+":             #in loop");
	right->gencode(false,false);
	addcode("\n"+test+":                 #test condition");
	left->gencode(false,false);
	if(left->exp_type[0]=="float"){left->typecast ="TO-INT"; cast(left);}
	addcode("bne $0, "+rstack.top()+", "+loop +"            #jump to loop of condition true");
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

void If::gencode(bool addr,bool is_struct){
	string false_label="$L"+to_string(label_count++), exit="$L"+to_string(label_count++);
	// cout<<a->exp_type[0]<<"v"<<endl;
	a->gencode(false,false);
	if(a->exp_type[0]=="float"){a->typecast ="TO-INT"; cast(a);}
	addcode("beq "+rstack.top()+", $0"+", "+false_label);
	b->gencode(false,false);
	addcode("j "+exit+"\n\n"+false_label+":");
	c->gencode(false,false);
	addcode("\n"+exit+":");
}

/* end of stmt_ast */
/* operator class */


oper2::oper2(string s){
	if( s=="OR_OP") op = "OR";
	if( s == "AND_OP") op = "AND";
	if( s == "EQ_OP") op = "EQ_OP";
	if( s == "NE_OP") op = "NE_OP";
	if( s == "LT") op = "LT";
	if( s == "GT") op = "GT";
	if( s == "LE_OP") op = "LE_OP";
	if( s == "GE_OP") op = "GE_OP";
	if( s == "PLUS") op = "PLUS";
	if( s == "MINUS") op = "MINUS";
	if( s == "MULT") op = "MULT";
	if( s == "DIV") op = "DIV";
	if( s == "ASSIGN") op = "ASSIGN";
	if(s == "DOT" ) op = "DOT_OP";
	if( s == "PTR_OP") op = "PTR_OP";
}

void oper2::print(){
	cout<<op<<" ";
}



oper1::oper1(string s){
	if( s=="UMINUS") op = "UMINUS";
	else if( s == "NOT") op = "NOT";
	else if( s == "PP") op = "PP";
	else if( s == "INC_OP") {op = "INC_OP";}
	else if(s=="POINTER") op="PTR";
	else if(s=="DEREF") op= "DEREF";


	
	
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
	cout<<"("<<left->typecast<<" ";
	left->print();
	cout<<")";
	cout<<"("<<right->typecast<<" ";
	right->print();
	cout<<")";
}

void op2::gencode(bool addr, bool is_struct){
	bool lfloat=false, rfloat=false;
		int idx=0;
		if(left->label <REG_NUM || right->label<REG_NUM){
			
			if(left->label>right->label){		//left label greater
				
				if(op->op == "DOT_OP" || op->op == "PTR_OP"){	//struct related statements
					//cout<<"Case 11"<<endl;
					if(op->op=="DOT_OP")left->gencode(true,is_struct);
					else {left->gencode(false,is_struct);idx=1;}
					string R = rstack.top();
					rstack.pop();
					string struct_name=left->exp_type[idx].substr(7);
					cout<<struct_name<<"S"<<endl;
					struct_symtab = getStruct(struct_name,glob);
					right->gencode(true,true);
					addcode("add "+R+", "+R+", "+rstack.top());   //addr of a.b
					rstack.push(R);
					if(!addr){
							if(exp_type[0]=="float")addcode("lwc1 "+fstack.top()+", 0("+rstack.top()+")");
							else addcode("lw "+rstack.top()+", 0("+rstack.top()+")");
							cout<<"gthrt5           "<<endl;
					}
				}
				else if(op->op.substr(0,6)  == "ASSIGN"){
					//cout<<"Case 12"<<endl;
					string R;
					left->gencode(true,false);
					R = rstack.top();  rstack.pop(); 						//lhs
					
					right->gencode(false,false);
					rfloat = cast(right);
					int rwidth = getWidth(right->exp_type)/4;
					vector<string> temp;
					
					if(rfloat){
						addcode("swc1 "+fstack.top()+", "+"0("+R+")" ); //rstack.top = R
					}
					else {
						
						if(right->exp_type[0].substr(0,6) == "struct"){			//for struct = struct case
							string R1 = rstack.top();  rstack.pop(); 						//address of rhs 
					
							for(int i=0; i<rwidth;i++){		//also check for floats
								addcode("lw "+rstack.top()+", "+to_string(i*4)+"("+R1+")" ); //rstack.top = offset(fp)
								addcode("sw "+rstack.top()+", "+to_string(i*4)+"("+R+")" );  //saved back to stack 
							}
							rstack.push(R1);
						}
						else{
							for(int i=0; i<rwidth;i++){
								addcode("sw "+rstack.top()+", "+to_string(i*4)+"("+R+")" ); //rstack.top = R
								temp.push_back(rstack.top());
								rstack.pop();
							}
						}
					}
					// if(left->exp_type[0]=="float")fstack.push(R);
					// else
					rstack.push(R);
					if(!rfloat){
						for(int i=0; i<temp.size();i++){
							rstack.push(temp[temp.size()-i-1]);
						}
					}
				}
				else{		//non-assign statements
					//cout<<"Case 13"<<endl;
					left->gencode(false,  false);
					string R,R2;
					lfloat =cast(left);

					if(lfloat){
						R= fstack.top();
						fstack.pop();
					}
					else {
						R= rstack.top();
						rstack.pop();
					}
					
					right->gencode(false, false);
					rfloat=cast(right);
					if(rfloat)R2=fstack.top();
					else R2=rstack.top();
					
					operatorCode(op->op,R,R2);
					
					if(lfloat)fstack.push(R);
					else rstack.push(R);
				}
				
			}
			else{	// right label greater
				if(op->op == "DOT_OP" || op->op == "PTR_OP"){	//struct related statements
					//cout<<"Case 21"<<endl;
					if(op->op=="PTR_OP")idx=1;
					string struct_name=left->exp_type[idx].substr(7);
					struct_symtab = getStruct(struct_name,glob);
					right->gencode(true,true);
					string R = rstack.top();
					rstack.pop();
					if(op->op=="DOT_OP")left->gencode(true,is_struct);
					else {left->gencode(false,is_struct);idx=1;}
					addcode("add "+rstack.top()+", "+rstack.top()+", "+R);   //addr of a.b
					rstack.push(R);
					if(!addr){
							if(right->exp_type[0]=="float")addcode("lwc1 "+fstack.top()+", 0("+rstack.top()+")");
							else addcode("lw "+rstack.top()+", 0("+rstack.top()+")");
					}
				}
				else {
					right->gencode(false,false);
			
					if(op->op.substr(0,6) == "ASSIGN"){
						string R1;
						// cout<<"Case 22"<<endl;
						rfloat = cast(right);
						int rwidth = getWidth(right->exp_type)/4;
						cout<<rwidth<<endl;
						vector<string> temp;
						if(rfloat){
								temp.push_back(fstack.top());
								fstack.pop();
						}
						else {
							if(exp_type[0].substr(0,6) == "struct"){			//for struct = struct case
								R1 = rstack.top();  rstack.pop(); 						//address of rhs 
							}
							else{
								for(int i=0; i<rwidth;i++){
									temp.push_back(rstack.top());
									rstack.pop();
								}
							}
						}
						left->gencode(true,false);    //lhs 
						if(rfloat){
								addcode("swc1 "+temp[0]+", "+ +"0("+rstack.top()+")" );
								fstack.push(temp[0]);
						}
						else{
							if(exp_type[0].substr(0,6) == "struct"){			//for struct = struct case
							 	string R3 = rstack.top(); rstack.pop(); 												//address of rhs 
								for(int i=0; i<rwidth;i++){		//also check for floats
									addcode("lw "+rstack.top()+", "+to_string(i*4)+"("+R1+")" ); //rstack.top = offset(fp)
									addcode("sw "+rstack.top()+", "+to_string(i*4)+"("+R3+")" );  //saved back to stack 
								}
								rstack.push(R3);
								rstack.push(R1);
							}
							else{
								for(int i=rwidth-1; i>=0; i--){
									addcode("sw "+temp[i]+", "+ to_string(i*4)+"("+rstack.top()+")" );
								}
								for(int i=0; i<rwidth;i++){
									rstack.push(temp[rwidth-i-1]);
								}
							}
						}
						
					}
					else{			//non assign statements
						// cout<<"Case 23"<<endl;
						rfloat=cast(right);
						string R,R2;
						if(rfloat){
							R = fstack.top();  //rhs
							fstack.pop();
						}
						else{
							R= rstack.top();
							rstack.pop();
						}
						left->gencode(false,false);
						lfloat=cast(left);
						if(lfloat)R2=fstack.top();
						else R2=rstack.top();
						
						operatorCode(op->op,R2,R);
						if(rfloat){fstack.push(R); fswap();}
						else { rstack.push(R); swap();}
		
					}
				}
			}
		}
		else{
			// store into memory
			string R,R2;
			if(op->op =="PTR_OP" || op->op=="DOT_OP"){
				// cout<<"Case 31"<<endl;
				if(op->op=="DOT_OP"){left->gencode(true,is_struct);idx=0;}
				else {left->gencode(false,is_struct);idx=1;}
				storeOnStack(rstack.top());
				string struct_name=left->exp_type[idx].substr(7);
				struct_symtab = getStruct(struct_name,glob);
				right->gencode(true,true);
				R = rstack.top();
				rstack.pop();
				loadFromStack(rstack.top());
				addcode("add "+R+", "+R+", "+rstack.top());   //addr of a.b
				if(!addr){
						if(right->exp_type[0]=="float")addcode("lwc1 "+fstack.top()+", 0("+R+")");
						else{
							addcode("lw "+rstack.top()+", 0("+R+")");
						}
				}
				rstack.push(R);
			}
			else if(op->op == "ASSIGN"){
				// cout<<"Case 32"<<endl;
				left->gencode(true,false);
				storeOnStack(rstack.top());
				
				right->gencode(false,false);
				rfloat=cast(right);
				int rwidth = getWidth(right->exp_type)/4;
				vector<string> temp;
				if(rfloat){
					//for(int i=rwidth-1; i>=0;i--){
						//addcode("sw "+rstack.top()+", "+to_string(i*4)+"("+R+")" ); //rstack.top = R
						temp.push_back(fstack.top());
						fstack.pop();
					//}
					
					loadFromStack(rstack.top());
					
					for(int i=1; i<rwidth;i++){
						addcode("swc1 "+temp[i]+", "+to_string(i*4)+"("+rstack.top()+")" );
						fstack.push(temp[rwidth-i-1]);
					}
				}
				else{
					if(exp_type[0].substr(0,6) == "struct"){			//for struct = struct case
						string R1 = rstack.top();  rstack.pop(); 						//address of rhs 
						R = rstack.top();  rstack.pop();
						loadFromStack(R);
					
						for(int i=0; i<rwidth;i++){		//also check for floats
							addcode("lw "+rstack.top()+", "+to_string(i*4)+"("+R1+")" ); //rstack.top = offset(fp)
							addcode("sw "+rstack.top()+", "+to_string(i*4)+"("+R+")" );  //saved back to stack 
						}
						rstack.push(R);
						rstack.push(R1);
					}
					else{
						for(int i=rwidth-1; i>=0;i--){
							//addcode("sw "+rstack.top()+", "+to_string(i*4)+"("+R+")" ); //rstack.top = R
							temp.push_back(rstack.top());
							rstack.pop();
						}
						
						loadFromStack(rstack.top());
						
						for(int i=1; i<rwidth;i++){
							addcode("sw "+temp[i]+", "+to_string(i*4)+"("+rstack.top()+")" );
							rstack.push(temp[rwidth-i-1]);
						}
					}
				}
			}
				// addcode("sw "+R+", 0("+rstack.top()+")" ); //rstack.top = R

		
			
			else{				//non assignment
				// cout<<"Case 33"<<endl;
				left->gencode(false,false);
				lfloat=cast(left);
				if(lfloat){
					addcode("addi $sp, $sp, -4");
					addcode("swc1 "+fstack.top()+", 0($sp)");
				}
				else{
					storeOnStack(rstack.top());
				}				
				right->gencode(false,false);
				rfloat=cast(right);
				if(rfloat){
					R = fstack.top(); 
					fstack.pop();
				}
				else{
					R = rstack.top();
					rstack.pop();
				}
				if(lfloat){
					addcode("lwc1 "+fstack.top()+", 0($sp)");  //load lhs from stack
					addcode("addi $sp, $sp, 4");
					R2=fstack.top();
				}
				else {
					loadFromStack(rstack.top());
					R2=rstack.top();
				}
				
				operatorCode(op->op,R2,R);
				
				if(rfloat){fstack.push(R); fswap();}
				else {rstack.push(R);swap();}
			}
			
		}
		
	
	
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


void op1::gencode(bool addr, bool is_struct){
	string s=op->op;
	if(s=="DEREF"){					// &
		child->gencode(true,is_struct);
	}
	else if(s=="PTR"){
		child->gencode(false,is_struct);
		if(!addr)
			addcode("lw "+rstack.top()+", 0("+rstack.top()+")");
	}
	else if(s=="UMINUS"){
		child->gencode(addr,is_struct);
		
		addcode("neg "+rstack.top()+", "+rstack.top());
	}
	else if(s=="NOT"){
		child->gencode(addr,is_struct);
		addcode("xori "+rstack.top()+", "+rstack.top()+", 1");
	}
}

func::func(){
	vector<exp_ast*> l;
	vec=l;
}

func::func(string s){
	fun_name =s;
	vector<exp_ast*> l;
	vec=l;
}


func::func(string q,exp_ast* s){

	vector<exp_ast*> l;
	fun_name=q;
	l.push_back(s);
	vec=l;
}

//redundant
func::func(vector<exp_ast*> l){
	fun_name="";
	vec=l;
}

void func::print(){
	printf("FuncCall ");
	cout<<fun_name<<" (";
	for(int i=0;i<vec.size();i++){
		printf("(");
		vec[i]->print();
		printf(")");
	}
	printf(") ");

}

void func::printfcode(){
	for(int i=0;i<vec.size();i++){
		if(vec[i]->exp_type[0]=="string"){			//string printf
			vec[i]->gencode(false,false);
		}
		else if(vec[i]->exp_type[0]=="int"){									//int 
			vec[i]->gencode(false,false);
			printInt(rstack.top());
		}
		else if(vec[i]->exp_type[0]=="float"){										//float
			vec[i]->gencode(false,false);
			printFloat(fstack.top());
		}
	}
}

void func::gencode(bool addr, bool is_struct){
	if(fun_name=="printf"){
		printfcode();
		return;
	}
	int width=getGlobalWidth(fun_name,glob);
	addcode("addi $sp, $sp, "+to_string(-width)+"                    #space for return value");    //space for Rv
	int w=0;
	addcode("                                               #parameters ");
	vector<string> temp;
	for(int i=vec.size()-1;i>=0;i--){				//store parameters
		int wid=getWidth(vec[i]->exp_type)/4;
		w+=wid*4;
		if(vec[i]->exp_type[0].substr(0,5)=="array"){			//for array , store address
			vec[i]->gencode(true,false);                //get address in top register
			// addcode("sw "+rstack.top()+", "+to_string(-wid*)+"($sp)");
			addcode("                                       #store array address ");
			storeOnStack(rstack.top());
		}
		else if(vec[i]->exp_type[0].substr(0,6)=="struct"){
			vec[i]->gencode(true,false);                //get struct address in  register
			addcode("                                       #store struct");
			string R = rstack.top(); rstack.pop();
			addcode("addi $sp, $sp, "+to_string(-wid*4));
			for(int i=0;i<wid;i++){
				addcode("lw "+rstack.top()+", "+to_string(i*4)+"("+R+")");
				addcode("sw "+rstack.top()+", "+to_string(i*4)+"($sp)");
			}
			
		}
		else{										//int  no for required
			vec[i]->gencode(false,false);                //get value in top register
			storeOnStack(rstack.top());
			// for(int i=0;i<wid;i++){
			// 	temp.push_back(rstack.top());
			// 	storeOnStack(temp[i]);
			// 	rstack.pop();
			// }
			// for(int i=0;i<wid;i++){	
			// 	rstack.push(temp[i]);
			// }
		}

	}
	addcode("addi $sp, $sp, -8        #space for RA and SL");    //space for SL & RA
	addcode("jal "+fun_name+"           #call to "+fun_name);
}

float_const::float_const(float l){
	child=l;
}

void float_const::print(){
	printf("FloatConst ");
	printf("%f ",child);
}	

void float_const::gencode(bool addr, bool is_struct){
	addcode("li.s "+fstack.top()+", "+to_string(child));
}


int_const::int_const(int l){
	child=l;
};

void int_const::print(){
	printf("IntConst ");
	cout<<child;
}

void int_const::gencode(bool addr, bool is_struct){
	addcode("li "+rstack.top()+", "+to_string(child));
}

string_const::string_const(string l){
	child=l;
};

void string_const::print(){
	printf("StringConst ");
	std::cout << child;	
}

void string_const::gencode(bool addr,bool is_struct){
	addcode("li $v0, 4                    #print string");
	addcode("la $a0,"+child);
	addcode("syscall");
}

/* Ref ast */

Identifier::Identifier(){
	child = " ";
}
Identifier::Identifier(string s){
	child = s;
}

void Identifier::print(){
	// cout<<typecast<<" ";
	cout << child;
}

void Identifier::gencode(bool addr, bool is_struct){
	
	int wid=getLocalWidth(child,loc)/4;
	vector<string> temp;
	if(!is_struct){
		vector<string> typ=getLocalType(child,loc);
		int off=-1*getLocalOffset(child,loc);
		 //if(off<=0)off-=4;  //SL and RA stored in between  
			if(!addr){
				if(typ[0]=="float"){
					addcode("lwc1 "+fstack.top()+", "+to_string(off)+"($fp)");
				}
				else if(typ[0].substr(0,6)=="struct"){			// not required , ask for address while calling PALAK
					addcode("addi "+rstack.top()+", $fp, "+to_string(off)+"			#only address of stack loaded");			
				}
				else{					// only ints handled here , no for req.
					for(int i=wid-1;i>=0;i--){
						addcode("lw "+rstack.top()+", "+to_string(off-4*i)+"($fp)");
						temp.push_back(rstack.top());
						rstack.pop();
					}
					for(int i=wid-1;i>=0;i--){
						rstack.push(temp[i]);
					}
				}
			}
			else if(typ[0].substr(0,5)=="array" && addr && off>0){  //param array
				addcode("lw "+rstack.top()+", "+to_string(off)+"($fp) ");
			}
			else {
				addcode("addi "+rstack.top()+", $fp, "+to_string(off));
			}
		
	}
	else{
		vector<string> typ=getLocalType(child,struct_symtab);
		int off=getLocalOffset(child,struct_symtab);
		off-=getLocalWidth(child,struct_symtab);
		addcode("li "+ rstack.top()+ ", "+ to_string(off));	
		
	}
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

deref::deref(){
	child=NULL;
};

deref::deref(ref_ast* r){
	child=r;
};

void deref::print(){
	printf("Deref ");
	child->print();
}
Ref::Ref(){
	child=NULL;
};
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



void array_ref::gencode(bool addr, bool is_struct){
	if(child->label <REG_NUM || vec->label<REG_NUM){
		if(child->label>vec->label){
			child->gencode(true,is_struct); 			 //address of lhs
			string R1 = rstack.top();   //laddr
			rstack.pop();
			vec->gencode(false,false);
			
			//compute size of vec
			int w = getWidth(exp_type);
			string R2 =rstack.top();rstack.pop(); //rvale
			addcode("li "+rstack.top()+", "+to_string(w));  //width
			addcode("mul "+R2+", "+rstack.top()+", "+R2);  // const*width
			addcode("add "+R1+", "+R1+", "+R2);								//addr so far +const*width
			rstack.push(R2);
			rstack.push(R1);
			
		}
		else{
			swap();
			vec->gencode(false,false);
			string R1 = rstack.top(); //rhs
			rstack.pop();
			
			child->gencode(true,is_struct); //lhs
			int w = getWidth(exp_type);
			string R2 =rstack.top();rstack.pop(); //lhs
			
			addcode("li "+rstack.top()+", "+to_string(w));  //width
			addcode("mul "+R1+", "+rstack.top()+", "+R1);  // const*width
			addcode("add "+R2+", "+R1+", "+R2);								//addr so far +const*width
			rstack.push(R2);
			rstack.push(R1);
			swap();
		}
	}
	else{
		child->gencode(true,is_struct);
		storeOnStack(rstack.top());   //put r0 on stack
		vec->gencode(false,false);
		int w = getWidth(exp_type);
		string R2 =rstack.top();rstack.pop(); //rvale
		addcode("li "+rstack.top()+", "+to_string(w));  //width
		addcode("mul "+R2+", "+rstack.top()+", "+R2);  // const*width
			
		loadFromStack(rstack.top());  //lhs in register
		addcode("add "+R2+", "+R2+", "+rstack.top());								//addr so far +const*width
		rstack.push(R2);

	}
	
	if(!addr ){								//get value
			addcode("lw "+rstack.top()+", 0("+rstack.top()+")");   
	}

}



/* function */

int getStructSize(string s,global_symbol_table* g){
	
	for(int i=0; i<g->sym.size();i++){
		if(g->sym[i].name == s){
		 	 return g->sym[i].width;
		 }
	}
	return 0;
}

vector<string> getGlobalType(string fn,global_symbol_table* l){

	for(int i=0; i<l->sym.size();i++){
		if(l->sym[i].name == fn){
			// cout<<"hello"<<endl;
		 	 return l->sym[i].type_list;
		 }
	}
	vector<string> s;
	s.push_back(" ");
	return s;
	
}

int getLocalWidth(string a,local_symbol_table* l){
	for(int i=0; i<l->sym.size();i++){
		if(l->sym[i].name == a){
		 	return l->sym[i].width;
		 }
	}
	
	return -1;
}

int getGlobalWidth(string a,global_symbol_table* l){
	for(int i=0; i<l->sym.size();i++){
		if(l->sym[i].name == a){
		 	return l->sym[i].width;
		 }
	}
	return -1;
}

int getLocalOffset(string a,local_symbol_table* l){
	for(int i=0; i<l->sym.size();i++){
		if(l->sym[i].name == a){
		 	return l->sym[i].offset;
		 }
	}
	
	return -1;
}

vector<string> getLocalType(string a,local_symbol_table* l){
	for(int i=0; i<l->sym.size();i++){
		if(l->sym[i].name == a){
		 	return l->sym[i].type_list;
		 }
	}
	vector<string> v;
	return v;
}

string compatible(exp_ast* a,exp_ast* b,oper2* p){
	//check atomic
	if(!(a->exp_type.size()==1 && b->exp_type.size()==1))return "NULL";
	string t1=a->exp_type[0],t2=b->exp_type[0];
	//check type compatibility
	if(t1=="int" && t2=="int"){
		p->op+="-INT";
		return "int";
	}
	else if(t1=="float" && t2=="float"){ 
		p->op+="-FLOAT";
		return "float";
	}
	else if(t1=="float" && t2=="int"){
		b->typecast="TO-FLOAT";
		p->op+="-FLOAT";
		return "float";
	}
	else if(t1=="int" && t2=="float"){
		if(p->op=="ASSIGN"){
			b->typecast="TO-INT";
			p->op+="-INT";
			return "int";
		}
		else {
			a->typecast="TO-FLOAT";
			p->op+="-FLOAT";
			return "float";
		}
	}
	else{  return "NULL";}
	
}

vector<string> rem_star(vector<string> tp1, int r,int line_num){
	// cout<<"Remove stars \n";
	int pos=0;
	int count = r;
	vector<string> temp;
	for(int i=0;i<tp1.size();i++){
		//cout<<tp1[i]<<"|";
		if(count > 0){
			if(tp1[i].substr(0,5) == "array" || tp1[i] == "pointer"){
				pos++;
				count--;
			}	
		}
		else{
			break;
		}
	}
	//cout<<endl;
	if(count>0){
		
		cout<<"ERROR in line "<<line_num<<" : Type not allowed"<<endl;
		exit(0);
	}
	for(int i= pos;i<tp1.size();i++){
		temp.push_back(tp1[i]);
		 //cout<<"yolo "<<temp[i-pos]<<"| ";
	}
	// cout<<"Exit stars \n";

	return temp;
	
	
}

//look for easier way
vector<string> rem_dimension(vector<string> s,int line_num){
	// cout<<"Remove Dimension \n";
	vector<string> res;
	if(s.size()==0 ){cout<<"Size 0 in line "<<line_num<<"\n"; exit(0);}
	else if(s[0].substr(0,5)!="array" && s[0]!="pointer"){cout<<"Error :Type Not possible\n";exit(0);}
	for(int i=1;i<s.size();i++){
		res.push_back(s[i]);
		// cout<<res[i-1]<<" | ";
	}
	// cout<<endl;
	return res;
}

local_symbol_table* getStruct(string s,global_symbol_table* g){
	cout<<"Struct "<<s<<endl;
	for(int i=0; i<g->sym.size();i++){
		cout<<"enter loop \n"<<g->sym[0].name<<"\n";
		if(g->sym[i].name == s){
		 	 return g->sym[i].func_tab;
		 }
	}
	cout << "Error: Struct not declared" <<endl;
	exit(0);
}

bool matchType(vector<string> param,vector<string> arg){
	if(param.size()!=arg.size())return false;
	else if(param.size()==1){
		oper2 *p=new oper2("ASSIGN");
		exp_ast* a=new Identifier(),*b=new Identifier();
		a->exp_type.push_back(param[0]);
		b->exp_type.push_back(arg[0]);
		if(compatible(a,b,p)!="NULL")return true;
		else if(param[0]==arg[0])return true;
		else return false;
	}
	else if(param.size()==2 && param[0]=="pointer" && arg[0]=="pointer" && (param[1]=="void" || arg[1]=="void")){
		return true;
	}
	for(int i=0;i<arg.size();i++){
		if(i==0 && param[0].substr(0,5)=="array" && arg[i].substr(0,5)=="array"){continue;}
		if(!(param[i]==arg[i] || param[i]=="pointer" || arg[i]=="pointer"))return false;
	}
	return true;
}

bool matchType2(vector<string> param,vector<string> arg){
	if(param.size()!=arg.size())return false;
	
	for(int i=0;i<arg.size()-1;i++){
		if(!(param[i]==arg[i] || param[i]=="pointer"))return false;
	}
	oper2 *p=new oper2("ASSIGN");
	exp_ast* a=new Identifier(),*b=new Identifier();
	a->exp_type.push_back(param[arg.size()-1]);
	b->exp_type.push_back(arg[arg.size()-1]);
	if(compatible(a,b,p)!="NULL")return true;
	else return false;
	
}

void func::match_fncall(string fn,global_symbol_table* g,int line_num){
	// cout<<"Enter Match fn call \n";
	local_symbol_table* l=NULL;
	int nparam=0;
	for(int i=0; i<g->sym.size();i++){
		if(g->sym[i].name == fn){
		 	 l=g->sym[i].func_tab;
		 	 nparam=g->sym[i].param_num;
		 	 //cout<<g->sym[i].name<<" "<<g->sym[i].param_num<<endl;
		 	 break;
		 }
	}
	if(l==NULL){
		cout << "Error in line "<<line_num<<" : Function not declared" <<endl;
		exit(0);
	}
	// cout<<fn<<" "<<v.size()<<" "<<nparam<<endl;
	if(vec.size()!=nparam){
		cout<<"Error in line "<<line_num<<" : Number of Parameters do not match"<<endl;
		exit(0);
	}
	 //cout<<vec.size()<<endl;
	for(int i=0;i<vec.size();i++){
			if(!matchType(vec[i]->exp_type,l->sym[i].type_list)){
        		cout<<"Error in line "<<line_num<<" : Function call arguments dont match \n";
        		exit(0);
        	}		
	}

}

vector<string> getMemType(string id,string struct_name,global_symbol_table* g){
	local_symbol_table* l=getStruct(struct_name,g);
	return getLocalType(id,l);
}

int getMemWidth(string id,string struct_name,global_symbol_table* g){
	local_symbol_table* l=getStruct(struct_name,g);
	return getLocalWidth(id,l);
}

bool assignCheck(exp_ast* lhs,exp_ast* rhs, oper2* o){
	if(lhs->exp_type.size()!=rhs->exp_type.size())return false;
	//only pointer assignable not array
	int sz=lhs->exp_type.size();
	for(int i=0;i<sz-1;i++){
		if(lhs->exp_type[i]!="pointer")return false;
	}
	
	if(lhs->exp_type.size()==2 && lhs->exp_type[0]=="pointer" && lhs->exp_type[1] =="void"){
		return true;
	}
	
	if(rhs->exp_type.size()==2 && rhs->exp_type[0]=="pointer" && rhs->exp_type[1] =="void"){
		return true;
	}
	
	if(lhs->exp_type[sz-1]==rhs->exp_type[sz-1])return true;
	if(sz > 1)return false;
	oper2 *p=new oper2("ASSIGN");
	exp_ast* a=new Identifier(),*b=new Identifier();
	a->exp_type.push_back(lhs->exp_type[sz-1]);
	b->exp_type.push_back(rhs->exp_type[sz-1]);
	if(compatible(a,b,p)!="NULL"){
		if(sz-1 == 0){
			lhs->typecast=a->typecast;
			rhs->typecast= b->typecast;
			o->op= p->op;
		}
		return true;
	}
	else return false;
}





int getWidth(vector<string> ex){
	int wid=1,i;
	for(i=0;i<ex.size();i++){
		if(ex[i].substr(0,5)!="array"){
			if(ex[i]=="int")return wid*4;
			else if(ex[i]=="float")return wid*4;
			else if(ex[i]=="pointer")return wid*4;
			else return wid*getStructSize(ex[i].substr(7,ex[i].length()-7),glob);			
		}
		int w=stoi(ex[i].substr(6,ex[i].length()-6));
		wid*=w;
	}
	
}

void storeAddress(int off){
	addcode("addi $t0, $fp,"+to_string(off));
	addcode("lw $t0 0($sp)");
	addcode("addi $sp,$sp,-4");
}

void printCode(){
	for(int i=0;i<code.size();i++){
		cout<<code[i]<<endl;
	}
}

int getLabel(int a,int b){
	
	if(a==b){
		// cout<<"Label 1 "<<a<<" 2: "<<b<<" final "<<a+1<<endl;
		return a+1;
	}
	// cout<<"Label 1 "<<a<<" 2: "<<b<<" final "<<max(a,b)<<endl;
	return max(a,b);
}

//syscalls
void printFloat(string reg){
	addcode("li $v0, 2 						# system call code for printing float ");
	addcode("mov.s $f12, "+reg);
    addcode("syscall");
}

void printInt(string reg){
	addcode("li $v0, 1						# system call code for printing int ");
	addcode("move $a0, "+reg);
    addcode("syscall");
}


//stack manipulation functions

void rstack_init(){
	while(!rstack.empty()){
		rstack.pop();
	}
	for(int i=10;i>=0;i--){
		string reg="$t"+to_string(i);
		rstack.push(reg);
	}
	for(int i=10;i>=0;i--){
		string reg="$s"+to_string(i);
		rstack.push(reg);
	}
	
}

void fstack_init(){
	while(!fstack.empty()){
		fstack.pop();
	}
	for(int i=0;i<REG_NUM;i++){
		string reg="$f"+to_string(i);
		fstack.push(reg);
	}
}

void swap(){
	string R1 = rstack.top();
	rstack.pop();
	string R2 = rstack.top();
	rstack.pop();
	rstack.push(R1);
	rstack.push(R2);
}

void fswap(){
	string R1 = fstack.top();
	fstack.pop();
	string R2 = fstack.top();
	fstack.pop();
	fstack.push(R1);
	fstack.push(R2);
}

void storeOnStack(string reg){
	string t="add $sp $sp -4";
	string s="sw "+reg+", 0($sp)";
	code.push_back(t);
	code.push_back(s);
}

void storeOnStack_float(string reg){
	addcode("add $sp $sp -4");
	addcode("swc1 "+fstack.top()+", 0($sp)");
}

void addcode(string s){
	code.push_back(s);
}

void getFloatToStack(float num,string reg){
	string t="add $sp $sp -4";
	string s="addi "+reg+", $0, "+to_string(num); 
	string s2="sw "+reg+", 0($sp)";
	code.push_back(t);
	code.push_back(s);
	code.push_back(s2);
}


void loadFromStack(string reg1){
	string s="lw "+reg1+", 0($sp)";
	string u="add $sp $sp 4";
	code.push_back(s);
	code.push_back(u);

}

void loadFromStack_float(string reg1){
	string s="lwc1 "+reg1+", 0($sp)";
	string u="add $sp $sp 4";
	code.push_back(s);
	code.push_back(u);
}


void getStackToVar(int offset,string reg,int wid){
	// string t="add $sp $sp -4"
	for(int i=0;i<wid/4;i++){
		string off=to_string(4*i);
		string s2="lw "+reg+", "+off+"($sp)";
		string s="sw "+reg+", "+to_string(-offset+4*i)+"($fp)";
		code.push_back(s2);
		code.push_back(s);
	}
	
}

void getIntToStack(int num,string reg){
	string t="add $sp $sp -4";
	string s="addi "+reg+", $0, "+to_string(num); 
	string s2="sw "+reg+", 0($sp)";
	code.push_back(t);
	code.push_back(s);
	code.push_back(s2);
}



//code

void dynamic_link(){
	addcode("addi $sp, $sp -4");
	addcode("lw $fp, 0($sp)");
	addcode("move $sp , $fp");
}

void addLocals(){
	int w=0;
	for(int i=0;i<loc->sym.size();i++){
		if(loc->sym[i].offset>=0)
			w+=loc->sym[i].width;
	}
	addcode("add $sp, $sp, "+to_string(-w));
}

bool cast(exp_ast* node){
	if(node->typecast=="TO-INT"){
		addcode("cvt.w.s "+fstack.top()+", "+fstack.top());
		addcode("addi $sp, $sp ,-4");
		addcode("swc1 "+fstack.top()+", 0($sp)                    #typecast to int");
		loadFromStack(rstack.top());
		return false;
	}
	else if(node->typecast=="TO-FLOAT"){
		addcode("addi $sp, $sp ,-4");
		addcode("sw "+rstack.top()+", 0($sp)");
		addcode("lwc1 "+fstack.top()+", 0($sp)                  #typecast to float");
		addcode("addi $sp, $sp ,4");
		return true;
	}
	if(node->exp_type[0]=="float")return true;
	else return false;
}

void float_relational(string op,string F1,string F2){
	string l1="$L"+to_string(label_count++) , l2="$L"+to_string(label_count++);

	addcode("c."+op+".s "+F1+", "+F2);
	addcode("bc1f "+l1);
	addcode("li "+rstack.top()+", 1");   //check if rstack
	addcode("j "+l2+"\n\n"+l1+":");
	addcode("li "+rstack.top()+", 0");
	addcode("\n"+l2+":");
}


void operatorCode(string s,string R1,string R2){
	if(s=="PLUS-INT")addcode("add "+ R1+", "+R1+", "+R2);
	else if(s=="PLUS-FLOAT")addcode("add.s "+R1+", "+R1+", "+R2);
	else if(s=="MINUS-INT" )addcode("sub "+ R1+", "+R1+", "+R2);
	else if(s=="MINUS-FLOAT")addcode("sub.s "+R1+", "+R1+", "+R2);
	else if(s=="MULT-INT") addcode("mul "+ R1+", "+R1+", "+R2);
	else if(s=="DIV-INT") addcode("div "+ R1+", "+R1+", "+R2);
	else if(s=="MULT-FLOAT")addcode("mul.s "+R1+", "+R1+", "+R2);
	else if(s=="DIV-FLOAT")addcode("div.s "+R1+", "+R1+", "+R2);
	else if( s == "EQ_OP-INT"){
		addcode("xor "+R1+", "+R2+", "+R1);
		addcode("sltu "+R1+", "+R1+",1");
	}
	else if( s == "NE_OP-INT"){
		addcode("xor "+R1+", "+R2+", "+R1);
		addcode("sltu "+R1+", $0, "+R1);
	}
	else if( s == "LT") 
		addcode("slt "+ R1+", "+R2+", "+R1);
	else if( s == "GT-INT") 
		addcode("slt "+ R1+", "+R1+", "+R2);
	else if( s == "LE_OP-INT") {
		addcode("slt "+ R1+", "+R1+", "+R2);
		addcode("xori "+R1+", "+R1+", 0x1");
	}
	else if( s == "GE_OP-INT"){
		addcode("slt "+ R1+", "+R2+", "+R1);
		addcode("xori "+R1+", "+R1+", 0x1");
	}
	else if(s=="EQ_OP-FLOAT")float_relational("eq",R1,R2);
	else if(s=="NE_OP-FLOAT")float_relational("ne",R1,R2);
	else if(s=="LT-FLOAT")float_relational("lt",R1,R2);
	else if(s=="GT-FLOAT")float_relational("gt",R1,R2);
	else if(s=="LE_OP-FLOAT")float_relational("le",R1,R2);
	else if(s=="GE_OP-FLOAT")float_relational("ge",R1,R2);
	else if( s=="OR_OP") addcode("or "+ R1+", "+R2+", "+R1);
	else if( s == "AND_OP") addcode("and "+ R1+", "+R2+", "+R1);

}

void function_setup(local_symbol_table* l, global_symbol_table* g){
	rstack_init();
	fstack_init();
	loc=l;
	glob =g;                                //init 
	string fn_name = glob->sym[glob->sym.size()-1].name;
	addcode("\n"+fn_name+":");
	dynamic_link();
	addLocals();
}

void function_return(){
	addcode("\nRet"+to_string(ret_label++)+" :");
	addcode("move $sp, $fp                         #sp=fp");
	addcode("lw $fp, 0($sp)                    #restore previous frame pointer");
	addcode("lw $ra, 4($sp)                #restore RA");
	int w=getParamWidth(loc)+12;
	addcode("add $sp, $sp, "+to_string(w)+"                  #stack restore");

}

int getParamWidth(local_symbol_table* l){
	int w=0;
	for(int i=0; i<l->sym.size();i++){
		if(getLocalOffset(l->sym[i].name,l)<0){
			if(l->sym[i].type_list[0].substr(0,5)=="array")w+=4;
			else w+=l->sym[i].width;
		}
	}
	return w;
}



//helper functions

exp_ast* addString(string e){
	string_symtab.push_back(e);
	exp_ast* lbl=new string_const("String"+to_string(string_label++));
	return lbl;
}

void printData(){
	cout<<".data \n\n";
	for(int i=0;i<string_symtab.size();i++){
		cout<<"String"<<i<<":\t .asciiz        "<<string_symtab[i]<<endl;
	}
	cout<<"\n\n.text \n\n";
}