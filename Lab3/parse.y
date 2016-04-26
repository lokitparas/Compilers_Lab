
%scanner Scanner.h
%scanner-token-function d_scanner.lex()
%token VOID INT FLOAT FLOAT_CONSTANT INT_CONSTANT AND_OP OR_OP EQ_OP NE_OP GE_OP LE_OP STRING_LITERAL IF ELSE WHILE FOR RETURN STRUCT IDENTIFIER INC_OP PTR_OP OTHERS

%polymorphic exp_ast : exp_ast* ; stmt_ast : stmt_ast*; Opr: oper1*; Int : int; Float : float; String : string;

%type <exp_ast> expression logical_and_expression logical_or_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression postfix_expression primary_expression  constant_expression expression_list
%type <stmt_ast> selection_statement iteration_statement assignment_statement translation_unit function_definition compound_statement statement statement_list type_specifier M_type_specifier
%type <Int> INT_CONSTANT 
%type <Float> FLOAT_CONSTANT
%type <String> STRING_LITERAL IDENTIFIER base_type
%type <Opr> unary_operator
%%

start : translation_unit{
		 printData();
		 printCode();
	};

translation_unit 
        :  struct_specifier
 	|function_definition {
 		$<stmt_ast>$ = $<stmt_ast>1;
 		//$<stmt_ast>$->print();
 		//cout<<endl<<endl;
 		//code
 		function_setup(local_sym_tab,global_sym_tab);
 		$<stmt_ast>$->gencode(false,false);
 		 function_return();
 	}
 	|  translation_unit function_definition{
 		$<stmt_ast>$ = $<stmt_ast>2;
 		//$<stmt_ast>$->print();
 		//cout<<endl<<endl;
 		
 		//code
 		//fn_count--;
 		function_setup(local_sym_tab,global_sym_tab);
 		$<stmt_ast>$->gencode(false,false);
 		function_return();
	}
        | translation_unit struct_specifier
        ;
 

M_struct_specifier
	:STRUCT IDENTIFIER{
        	
        	gsym=new gsymbol();
        	type="struct";
		gsym->type_list.push_back(type);
		gsym->name=$2;
		gsym->offset=loffset;
		gsym->func_tab=new local_symbol_table();
		loffset=0;
		local_sym_tab=gsym->func_tab;
		
		if(!getGlobalType(gsym->name,global_sym_tab).empty() && getGlobalType(gsym->name,global_sym_tab)[0].substr(0,6)=="struct"){
			cout<<"ERROR in line "<<line_num<<" :Symbol "<<gsym->name<<" already declared \n";
			exit(0);
		}
		global_sym_tab->sym.push_back(*gsym);
        };

struct_specifier 
        : M_struct_specifier '{' declaration_list '}' ';'{
        	gsym->width=loffset;
        	global_sym_tab->sym.back().width=gsym->width;
        	loffset=0;
        	//cout<<endl<<"------Struct begins------------"<<endl;
        	gsym->print();
        	
        }
        ;
M_fun_type_specifier
	:type_specifier{
		gsym=new gsymbol();
		//gsym->type_list.push_back(type);
		fntype=type;
	//	cout<<fntype<<endl;
		gsym->width=width;
		gsym->offset=loffset;
		gsym->func_tab=new local_symbol_table();
		loffset=0;
		local_sym_tab=gsym->func_tab;
	};

function_definition
	:M_fun_type_specifier fun_declarator{
		
		
		gsym->type_list.push_back(fntype);
		if(getGlobalType(gsym->name,global_sym_tab)[0]!=" " && getGlobalType(gsym->name,global_sym_tab)[0].substr(0,6)!="struct"){
			cout<<"ERROR in line  "<<line_num<<" :Symbol "<<gsym->name<<" already declared \n";
			exit(0);
		}
		global_sym_tab->sym.push_back(*gsym);
	} compound_statement{
	//	cout<<"\n------ symbol table of next function -------\n";
		gsym->print();
		//cout<<endl<<"--------AST for this function------------"<<endl;
		$<stmt_ast>$ = $<stmt_ast>4;
	}
	;

type_specifier                   // This is the information 
        : VOID{type="void";width=0; $$ = (stmt_ast*)new string_const("void");} 	                 // that gets associated with each identifier
        | INT{type="int";width=4; $$ = (stmt_ast*)new string_const("int");}   
	| FLOAT{type="float";width=4;$$ = (stmt_ast*)new string_const("float");}  
        | STRUCT IDENTIFIER{
        	 type="struct "+$2;
        	 if(getGlobalType($2,global_sym_tab)[0] != "struct"){		//redundant to check type struct
        	 	cout<<"ERROR in line  "<<line_num<<" : Struct "<<$2<<" not declared"<<endl;
        	 	exit(0);
        	 }
        	 width=getStructSize($2,global_sym_tab);
        	 $$ = (stmt_ast*)new string_const(type);
        }
        ;

fun_declarator
	:IDENTIFIER '(' parameter_list ')'{
		loffset=0;
		gsym->name=$1;
		
		
	}
	| IDENTIFIER '(' ')'{
		 gsym->name=$1;
	}
        | '*' fun_declarator{
        	gsym->width= 4;
        	gsym->type_list.push_back("pointer");

        }
	;                      


parameter_list
	: parameter_declaration{
		if(gsym->param_num ==0){
			//loffset-=local_sym_tab->sym[0].width;				//confirm later
			local_sym_tab->sym[0].offset=-12;   //loffset;	
		}
		gsym->param_num=local_sym_tab->sym.size();
	}
	| parameter_list ',' parameter_declaration{
		loffset=-local_sym_tab->sym[0].width+local_sym_tab->sym[0].offset;
	//	for(int i=local_sym_tab->sym.size()-1;i>=0;i--){
	//		local_sym_tab->sym[i].offset=loffset;
	//		loffset-=local_sym_tab->sym[i].width;				//confirm later
	//	}
		for(int i=1;i<local_sym_tab->sym.size();i++){
			local_sym_tab->sym[i].offset=loffset;
			loffset-=local_sym_tab->sym[i].width;				//confirm later
		}
		loffset=0;
		gsym->param_num=local_sym_tab->sym.size();
	}
	;

M_parameter_type
	: type_specifier{
		locsym=new lsymbol();
		locsym->width=width;
		locsym->offset=loffset;
	};
parameter_declaration
	:M_parameter_type declarator{
		locsym->type_list.push_back(type);
		if(!getLocalType(locsym->name,local_sym_tab).empty()){
			cout<<"ERROR in line  "<<line_num<<" :Symbol "<<locsym->name<<" already declared \n";
			exit(0);
		}
		if(locsym->type_list.size()==1 && type == "void"){cout<<"ERROR in line "<<line_num<<" : variable or field "<<locsym->name<<" declared void"<<endl;exit(0);}
		
		local_sym_tab->sym.push_back(*locsym);
	
	}
        ;

declarator
	: IDENTIFIER {
		locsym->name=$1;
	}
	
	| declarator '[' {array_constant=true;}primary_expression']' // check separately that it is a constant DOUBT HERE
	{array_constant=false;

		if($4->exp_type.size()==1 && $4->exp_type[0]=="int"){
			locsym->type_list.push_back("array "+to_string($4->val));
			locsym->width=locsym->width * $4->val;
		}
		else{cout<<"ERROR in line  "<<line_num<<" size of array is not an integer\n"; exit(0);}
	}
        | '*' declarator{
        	locsym->width=4;
        	locsym->type_list.push_back("pointer");
        }
        ;

primary_expression 
        : IDENTIFIER            // primary expression has IDENTIFIER now
        {		
        	$$=new Identifier($1);
        	vector<string> temp = getLocalType($1,local_sym_tab); 
        	if(temp.size() == 0){
        		cout<<"ERROR in line  "<<line_num<<" : "<<$1<<" is not declared"<<endl;
        		exit(0);
        		
        	}
        	
        	for(int i=0; i<temp.size(); i++){
        		$$->exp_type.push_back(temp[i]);
        	}
		        	
        	$$->exp_type=rem_star($$->exp_type, count_star,line_num);
        	//cout<<line_num<<endl;
        	
        	
        	if($$->exp_type.size()==1 && $$->exp_type[0]=="void"){
			cout<<"ERROR: VOID * cannot be dereferenced"<<endl;
			exit(0);
		}
        	
        	count_star=0;
        	
        	
        	//code
        	$$->label=1;
        	
        }
        | INT_CONSTANT{
        	$$=new int_const($1);
        	$$->val=$1; 
        	$$->exp_type.push_back("int");
        	//cout<<array_constant<<" "<<$1<<endl;
        	if(!array_constant){lvalue=false;}
        	else{lvalue=true;}
        	
        	 //code
        	$$->label=1;
        } 
        | FLOAT_CONSTANT{
        	
        	$$=new float_const($1);
        	$$->exp_type.push_back("float");
        	lvalue=false;
        	
        	//code
        	$$->label=2;

        }
        | STRING_LITERAL{
        	//string_const* s=new string_const($1);
        	$$ = addString($1) ;
        	
        	$$->exp_type.push_back("string");
        	lvalue=false;
		//addString($$);        	
        	//code
        	$$->label=1;
        }
        | '(' expression ')'{
        	$$= $2;
        } 
        ;

compound_statement
	: '{' '}' {
		$<stmt_ast>$ = new seq();
	}
	| '{' statement_list '}' {
		$$=$2;
	}
        | '{' declaration_list statement_list '}'{
        	$$=$3;
        }
	;

statement_list
	: statement{
		$<stmt_ast>$ = new seq($<stmt_ast>1);
	}		
        | statement_list statement{
        	((seq*)$<stmt_ast>1)->vec.push_back($<stmt_ast>2);
        	$<stmt_ast>$=$<stmt_ast>1;
        }	
	;

statement
        : '{' statement_list '}' {
        	lvalue=true;
        	$<stmt_ast>$=$<stmt_ast>2;
        } 
        | selection_statement {
        	lvalue=true;
		$<stmt_ast>$=$<stmt_ast>1;
	}	
        | iteration_statement{
        	lvalue=true;
		$<stmt_ast>$=$<stmt_ast>1;
	} 	
	| assignment_statement{
		lvalue=true;
		$<stmt_ast>$=$<stmt_ast>1;
	}	
        | RETURN expression ';'	{
        	lvalue=true;
        	if(!matchType(gsym->type_list,$2->exp_type)){
        		cout<<"ERROR in line  "<<line_num<<" : Return type invalid \n";
        		exit(0);
        	}
        	
		$<stmt_ast>$= new Return($<exp_ast>2);        
        }
        ;

assignment_statement
	: ';'{
		$<stmt_ast>$=new ass();
	} 								
	|  expression ';'  {
		//cout<<"in here"<<endl;
		$<stmt_ast>$=new new_ass($<exp_ast>1);
	}	
	;

expression             //assignment expressions are right associative
        : logical_or_expression{
        	lvalue=true;
        }
        |  unary_expression '='{
        
        	if($1->exp_type.size() ==1 && $1->exp_type[0] == "void"){
        		cout<<"ERROR in line "<<line_num<<" : Invalid operation on Void"<<endl;
        		exit(0);
        	}
        	if(!lvalue){
        		cout<<"ERROR in line  "<<line_num<<" : left expression not valid \n";
        		exit(0);
        	}
        
        } expression {
        	lvalue = true;
        	int l=getLabel($1->label,$4->label);
        	oper2* t = new oper2("ASSIGN");
        	if(!assignCheck($1,$4,t)){
        		cout<<"ERROR in line  "<<line_num<<" : Incompatible Assignment \n";
        		exit(0);
        	}
        	exp_ast* temp=$1;
        	$$=new op2($1,$4,t);
        	$$->exp_type=temp->exp_type;
        	
        	$$->label=l;
        	
        	
        }
        ; // l_expression has been replaced by unary_expression.
                                           // This may generate programs that are syntactically incorrect.
                                             // Eliminate them during semantic analysis.
                          
         

logical_or_expression            // The usual hierarchy that starts here...
	: logical_and_expression
        | logical_or_expression OR_OP logical_and_expression{
		if($1->exp_type[0].substr(0,6)=="struct" || $3->exp_type[0].substr(0,6)=="struct"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l = getLabel($1->label,$3->label);
		$$=new op2($1,$3,new oper2("OR_OP"));
		$$->exp_type.push_back("int");
		
		//code
		$$->label = l;
        }
	;
logical_and_expression
        : equality_expression
        | logical_and_expression AND_OP equality_expression{
		if($1->exp_type[0].substr(0,6)=="struct" || $3->exp_type[0].substr(0,6)=="struct"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l =  getLabel($1->label,$3->label);
		$$=new op2($1,$3,new oper2("AND_OP"));
		$$->exp_type.push_back("int");
		
		//code
		$$->label = l;
        }
	;

equality_expression
	: relational_expression 
        | equality_expression EQ_OP relational_expression{
        	oper2* o = new oper2("EQ_OP");
        	if(compatible($1,$3,o) == "NULL"){
			cout<<"ERROR in line   "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l =  getLabel($1->label,$3->label);
		$$=new op2($1,$3,o);
		$$->exp_type.push_back("int");
		//code
				$$->label = l;
		
        }
	| equality_expression NE_OP relational_expression{
		oper2* o =new oper2("NE_OP");
		if(compatible($1,$3,o)=="NULL"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l = getLabel($1->label,$3->label);
		$$=new op2($1,$3,o);
		$$->exp_type.push_back("int");
		//code
				$$->label = l;
	}
	;
relational_expression
	: additive_expression
        | relational_expression '<' additive_expression{
        	oper2* o =new oper2("LT");
        	if(compatible($1,$3,o)== "NULL"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l = getLabel($1->label,$3->label);
		$$=new op2($1,$3,o);
		$$->exp_type.push_back("int");
		//code
				$$->label = l;
        }
	| relational_expression '>' additive_expression{
		oper2* o =new oper2("GT");
		if(compatible($1,$3,o) == "NULL"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l = getLabel($1->label,$3->label);
		$$=new op2($1,$3,o);
		$$->exp_type.push_back("int");
		//code
		$$->label = l;
	}
	| relational_expression LE_OP additive_expression{
		oper2* o=new oper2("LE_OP");

		if(compatible($1,$3,o) == "NULL"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l = getLabel($1->label,$3->label);
		$$=new op2($1,$3,o);
		$$->exp_type.push_back("int");
		
		//code
		$$->label = l;
		
	}
        | relational_expression GE_OP additive_expression 
        {	oper2* o = new oper2("GE_OP");
        	if(compatible($1,$3,o) == "NULL"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l = getLabel($1->label,$3->label);
		$$=new op2($1,$3,o);
		$$->exp_type.push_back("int");
		
		//code
		$$->label = l;
        }
	;

additive_expression 
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression{
		oper2* x=new oper2("PLUS");
		string ret_type=compatible($1,$3,x);
		if(ret_type=="NULL"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l = getLabel($1->label,$3->label);
		$$=new op2($1,$3,x);
		$$->exp_type.push_back(ret_type);
		lvalue= false;
		
		//code
		$$->label = l;
	}
	| additive_expression '-' multiplicative_expression{
		oper2* x=new oper2("MINUS");
		string ret_type=compatible($1,$3,x);
		if(ret_type=="NULL"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l = getLabel($1->label,$3->label);
		$$=new op2($1,$3,x);
		$$->exp_type.push_back(ret_type);
		lvalue= false;
		
		//code
		$$->label = l;
	}
	;

multiplicative_expression
	: unary_expression
	| multiplicative_expression '*' unary_expression{
		oper2* x=new oper2("MULT");
		string ret_type=compatible($1,$3,x);
		if(ret_type=="NULL"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l = getLabel($1->label,$3->label);
		$$=new op2($1,$3,x);
		$$->exp_type.push_back(ret_type);
		lvalue= false;
		
				//code
				$$->label = l;
	}
	| multiplicative_expression '/' unary_expression{
		oper2* x=new oper2("DIV");
		string ret_type=compatible($1,$3,x);
		if(ret_type=="NULL"){
			cout<<"ERROR in line  "<<line_num<<" : Type Incompatible \n"; 
			exit(0);
		}
		int l = getLabel($1->label,$3->label);
		$$=new op2($1,$3,x);
		$$->exp_type.push_back(ret_type);
		lvalue= false;
		
		//code
		$$->label = l;
	}
	;


unary_expression                                              
	: postfix_expression	
	| unary_operator unary_expression{
		
		$$=new op1 ($2,$1);
		//$$= $2;
		vector<string> temp;
		if(address){
			temp.push_back("pointer");
		
		}
		
		for(int i=0; i<$2->exp_type.size();i++){
			temp.push_back($2->exp_type[i]);
		}
		
		$$->exp_type = temp;
		if($$->exp_type.size()==1){
			if($$->exp_type[0]=="float"){ $$->label=2;}
			if($$->exp_type[0].substr(0,5) =="struct"){ $$->label=getStructSize($$->exp_type[0].substr(7),global_sym_tab)/4;}
			else{ $$->label=1;}
		}
		else{
			$$->label =1;
		}
		address =false;

		
	}     // unary_operator can only be '*' on the LHS of '='
	;                                     // you have to enforce this during semantic analysis


postfix_expression
	: primary_expression{
	
	}
        | IDENTIFIER '('')' 
        {			// Cannot appear on the LHS of '='. Enforce this.
 		int l= getStructSize($1,global_sym_tab)/4;
 		$$=new func($1);
        	$$->exp_type=getGlobalType($1,global_sym_tab);
        	$$->match_fncall($1,global_sym_tab,line_num);
        	lvalue=false;
        	if($$->exp_type.size() == 1 && $$->exp_type[0] =="int" && array_constant==true){
        		lvalue=true;
        	}
        	
        	//code
        	$$->label=l;
        }
	 | IDENTIFIER '('{fname=$1;}expression_list ')' {
	 	
	 	$$ = $4;
	 	if(fname!="printf"){
		 	$$->label=getStructSize($1,global_sym_tab)/4;
		 	$$->exp_type=getGlobalType($1,global_sym_tab);	// Cannot appear on the LHS of '='. Enforce this.
	        	$$->match_fncall($1,global_sym_tab,line_num);
        	}
        	lvalue=false;
        	if($$->exp_type.size() == 1 && $$->exp_type[0] =="int" && array_constant==true){
        		lvalue=true;
        	}
        	 //code
        }  	
        | postfix_expression '[' {array_constant=true;}expression ']'{
        	vector<string> temp = $1->exp_type;
        	int l=getLabel($1->label,$4->label+1);
        	$$ = new array_ref(((Array*)$1), $4);
        	$$->exp_type=temp;
        	array_constant=false;
        	$$->exp_type=rem_dimension($$->exp_type,line_num);
        	
        			//code
		$$->label=l;
        	
        }
        | postfix_expression '.' {
        	
        	if($1->exp_type[0].substr(0,6) != "struct"){
        		cout<<"ERROR in line  "<<line_num<<" : Invalid operation"<<endl;
        		exit(0);
        	}
        }IDENTIFIER{		//why this happening
        	exp_ast* temp = $1;	
        	vector<string> v = getMemType($4,temp->exp_type[0].substr(7),global_sym_tab);
        	$$ = new op2($1, new Identifier($4), new oper2("DOT"));
        	int wid=getMemWidth($4,temp->exp_type[0].substr(7),global_sym_tab);
        	if(v.empty()){
        		cout<<"ERROR in line  "<<line_num<<" : No data member "<<$4<<" for "<<temp->exp_type[0]<<endl;
        		exit(0);
        	}
        	$$->exp_type=v;
        	$$->label=getLabel(1,wid);
        }
        | postfix_expression{
        	if($1->exp_type.size() != 2) {
        		cout<<"ERROR in line  "<<line_num<<" : Invalid operation"<<endl;
        		exit(0);
        	}
        	if($1->exp_type[1].substr(0,6) != "struct" || $1->exp_type[0] != "pointer" ){
        		cout<<"ERROR in line  "<<line_num<<" : Invalid operation"<<endl;
        		exit(0);
        	}
        	
        }PTR_OP IDENTIFIER 
        {        	
        	exp_ast* temp = $1;	
        	$$ = new op2($1, new Identifier($4), new oper2("PTR_OP"));
        	vector<string> v = getMemType($4,temp->exp_type[1].substr(7),global_sym_tab);
        	int wid= getMemWidth($4,temp->exp_type[1].substr(7),global_sym_tab);
        	if(v.empty()){
        		cout<<"ERROR in line  "<<line_num<<" : No data member "<<$4<<" for "<<temp->exp_type[1]<<endl;
        		exit(0);
        	}
        	$$->exp_type=v;
        	$$->label=getLabel(1,wid);
        }
	| postfix_expression INC_OP 	       // Cannot appear on the LHS of '='   Enforce this
	{ 	if(lvalue!=true){
			cout<<"ERROR: in line "<<line_num<<" : Increment operator requires lvalue"<<endl;
			exit(0);
		}
		
		int l=$1->label;
	       $$=new op1($1, new oper1("INC_OP"));
	       lvalue=false;
	       $$->label=l;
	}
	;

expression_list
        :{
            	count_star=0;
        }expression{
        	$$ = new func(fname,$<exp_ast>2);
        }
        | expression_list ','{
        	count_star=0;
        } expression{
		((func*)$1)->vec.push_back($4);
		$$ = $1;       
	}
	;

unary_operator
        : '-'{	$$ = new oper1("UMINUS");
        	lvalue=false;
        }
	| '!'{	$$ = new oper1("NOT");
	        lvalue=false;
	}
        | '&'{	//cout<<"came here 2"<<endl;
        	$$ = new oper1("DEREF");
        	address=true;
                lvalue=false;
        }
        | '*'{	//cout<<"came here1"<<endl;
        	$$ = new oper1("POINTER");
        	count_star++;
        }
	;

selection_statement
        : IF '(' expression ')' statement ELSE statement 
        	{$<stmt_ast>$ = new If($3, $5, $7);}
	;

iteration_statement
	: WHILE '(' expression ')'  statement {     //change while
		$$ = new While($3, $5);
	}	
	| FOR '(' expression  ';'  expression ';'  expression ')' statement  //modified this production
        {
        	$$ = new For($3, $5, $7, $9);
        }
        ;

declaration_list
        : declaration  					
        | declaration_list declaration
	;

declaration
	: type_specifier declarator_list';' 
	;

M_decl
	:{		
		locsym=new lsymbol();
		locsym->width=width;
		//locsym->offset=loffset+width;
		//loffset+=width;
	
	};

declarator_list
	: M_decl declarator{
		loffset+=locsym->width;
		locsym->offset=loffset;
		locsym->type_list.push_back(type);
		
		if(!getLocalType(locsym->name,local_sym_tab).empty()){
			cout<<"ERROR in line  "<<line_num<<" :Symbol "<<locsym->name<<" already declared \n";
			exit(0);
		}
		
		if(locsym->type_list[0] == gsym->type_list[0] +" "+gsym->name){
			cout<<"ERROR in line  "<<line_num<<" : type not allowed"<<endl;
			exit(0);
		}
		if(locsym->type_list.size()==1 && type == "void"){cout<<"ERROR in line  "<<line_num<<" : variable or field "<<locsym->name<<" declared void"<<endl;exit(0);}
		local_sym_tab->sym.push_back(*locsym);
		//locsym->print();
		//loffset+=locsym->width;
	}
	| declarator_list ',' M_decl declarator{
		loffset+=locsym->width;
		locsym->offset=loffset;
		locsym->type_list.push_back(type);
		if(!getLocalType(locsym->name,local_sym_tab).empty()){
			cout<<"ERROR in line  "<<line_num<<" :Symbol "<<locsym->name<<" already declared \n";
			exit(0);
		}
		if(locsym->type_list.size()==1 && type == "void"){cout<<"ERROR in line  "<<line_num<<" : variable or field "<<locsym->name<<" declared void"<<endl;exit(0);}
		
		local_sym_tab->sym.push_back(*locsym);
		//locsym->print();
		//loffset+=locsym->width;	
	}
 	;


