%scanner Scanner.h
%scanner-token-function d_scanner.lex()

%token VOID INT FLOAT IDENTIFIER INT_CONSTANT FLOAT_CONSTANT RETURN OR_OP AND_OP EQ_OP NE_OP LE_OP GE_OP INC_OP STRING_LITERAL IF ELSE WHILE FOR OTHERS SYMBOL STRUCT PTR_OP

/* %polymorphic INT: int; TEXT: std::string; IF: If*; */
/* %type <TEXT> unary_operator */
/* %type <IF> selection_statement */
%polymorphic exp_ast : exp_ast* ; stmt_ast : stmt_ast*; Opr: oper1*; Int : int; Float : float; String : string;

%type <exp_ast> expression logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression postfix_expression primary_expression l_expression constant_expression expression_list
%type <stmt_ast> selection_statement iteration_statement assignment_statement translation_unit function_definition compound_statement statement statement_list
%type <Int> INT_CONSTANT 
%type <Float> FLOAT_CONSTANT
%type <String> STRING_LITERAL IDENTIFIER 
%type <Opr> unary_operator
%%

translation_unit 
        :  struct_specifier
 	| function_definition{
 		$<stmt_ast>$ = $<stmt_ast>1;
 		$<stmt_ast>$->print();
 		cout<<endl;
		
 	}
 	| translation_unit function_definition{
		$<stmt_ast>$ = $<stmt_ast>2;
 		$<stmt_ast>$->print();
 		 cout<<endl;

 	}
        | translation_unit struct_specifier
        ;

struct_specifier 
        : STRUCT IDENTIFIER '{' declaration_list '}' ';'
        ;

function_definition
	: type_specifier fun_declarator compound_statement{
		$<stmt_ast>$ = $<stmt_ast>3;
	} 
	;

type_specifier
        : base_type
        | type_specifier '*'
        ;

base_type 
        : VOID {
        	
        }
        | INT{
        	
        }
	| FLOAT{
        }
        | STRUCT IDENTIFIER{
        } 
        ;

fun_declarator
	: IDENTIFIER '(' parameter_list ')' 
	| IDENTIFIER '(' ')' 
	;

parameter_list
	: parameter_declaration 
	| parameter_list ',' parameter_declaration 
	;

parameter_declaration
	: type_specifier declarator 
        ;

declarator
	: IDENTIFIER
	| declarator '[' constant_expression ']' 
        ;

constant_expression 
        : INT_CONSTANT{
       	$$ = new int_const($1);

        }
        | FLOAT_CONSTANT{
               	$$ = new float_const($1);
        }
        ;

compound_statement
	: '{' '}' {
		$<stmt_ast>$ = new seq();	
	}
	| '{' statement_list '}'{
		$<stmt_ast>$=$<stmt_ast>2;
	}
        | '{' declaration_list statement_list '}'{
        	$<stmt_ast>$=$<stmt_ast>3;

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
        : '{' statement_list '}'{
        	$<stmt_ast>$=$<stmt_ast>2;
        }
        | selection_statement{
        	$<stmt_ast>$=$<stmt_ast>1;
        }
        | iteration_statement{
                $<stmt_ast>$=$<stmt_ast>1;
        }
	| assignment_statement{
		$<stmt_ast>$=$<stmt_ast>1;
	}
        | RETURN expression ';'{
		$<stmt_ast>$= new Return($<exp_ast>2);        
	}
        ;

assignment_statement
	: ';'{
		$<stmt_ast>$=new ass();
	}
	|  l_expression '=' expression ';'{
		$<stmt_ast>$=new ass($<exp_ast>1,$<exp_ast>3);
	}
	;

expression
	: logical_and_expression{
	 	$<exp_ast>$=$<exp_ast>1;
	}
        | expression OR_OP logical_and_expression{
        	 $<exp_ast>$=new op2($<exp_ast>1,$<exp_ast>3,new oper2("OR_OP"));
        }
	;
logical_and_expression
        : equality_expression{
		$<exp_ast>$ = $<exp_ast>1;
        }
        | logical_and_expression AND_OP equality_expression{
                 $<exp_ast>$=new op2($<exp_ast>1,$<exp_ast>3,new oper2("AND_OP"));

        }
	;

equality_expression
	: relational_expression{
		$<exp_ast>$=$<exp_ast>1;
	}
        | equality_expression EQ_OP relational_expression{
                 $<exp_ast>$=new op2($<exp_ast>1,$<exp_ast>3,new oper2("EQ_OP"));

        }
	| equality_expression NE_OP relational_expression{
	         $<exp_ast>$=new op2($<exp_ast>1,$<exp_ast>3,new oper2("NE_OP"));

	}
	;
relational_expression
	: additive_expression{
		$<exp_ast>$=$<exp_ast>1;
	}
        | relational_expression '<' additive_expression{
               $$=new op2($1,$3,new oper2("LT"));
        }
	| relational_expression '>' additive_expression{
	     $<exp_ast>$=new op2($<exp_ast>1,$<exp_ast>3,new oper2("GT"));

	}
	| relational_expression LE_OP additive_expression{
	               $<exp_ast>$=new op2($<exp_ast>1,$<exp_ast>3,new oper2("LE_OP"));
}
        | relational_expression GE_OP additive_expression 
        {
                       $<exp_ast>$=new op2($<exp_ast>1,$<exp_ast>3,new oper2("GE_OP"));
	}
	;

additive_expression 
	: multiplicative_expression{
		$$=$1;
	}
	| additive_expression '+' multiplicative_expression{
	       $$=new op2($1,$3,new oper2("PLUS"));
	}
	| additive_expression '-' multiplicative_expression{
	       $$=new op2($1,$3,new oper2("MINUS"));
	}
	;

multiplicative_expression
	: unary_expression{
		$$=$1;
	}
	| multiplicative_expression '*' unary_expression{
		$$=new op2($1,$3,new oper2("MULT"));

	}
	| multiplicative_expression '/' unary_expression{
		       $$=new op2($1,$3,new oper2("DIV"));

	}
	;
	
unary_expression
	: postfix_expression{
		$$=$1;
	}
	| unary_operator postfix_expression{
		$$=new op1 ($2,$1);
	}
	;

postfix_expression
	: primary_expression{
		$$=$1;
	}
        | IDENTIFIER '(' ')' {
        	$$ = new func();

        }
	| IDENTIFIER '(' expression_list ')'{
		$$=$3;
		
	}
	| l_expression INC_OP{
		$$=new op1($1, new oper1("INC_OP"));
	}
	;

primary_expression
	: l_expression{
		$$ = $1;	
	}
        | l_expression '=' expression
        {
        	$$ = new op2($1, $3, new oper2("ASSIGN"));

        }
        | INT_CONSTANT{
        	$$ = new int_const($1);
        } 
	| FLOAT_CONSTANT{
		$$ = new float_const($1);	
	}
        | STRING_LITERAL
        {
	  $$ = new string_const($1); 
	}
	| '(' expression ')' 
	{
		$$ = $2;
	}
	;

l_expression
        : IDENTIFIER
        {
	//	cout<<"Identifier lexp "<<$1<<endl;
		$$ = new Identifier($1);
	}
        | l_expression '[' expression ']'
        {
        	$$ = new array_ref(((Array*)$1), $3); 
        }
        | '*' l_expression
        {
        	$$=new deref((ref_ast*)$2);
        }
        | '&' l_expression 
        {
        	$$=new Ref((ref_ast*)$2);
        }
        | l_expression '.' IDENTIFIER
        {
        	$$ = new op2($1, new Identifier($3), new oper2("DOT"));
        }
        | l_expression PTR_OP IDENTIFIER
        {
        	$$ = new op2($1, new Identifier($3), new oper2("PTR_OP"));
        }
        ;

expression_list
        : expression
        {
	  $<exp_ast>$ = new func($<exp_ast>1);
	}
        | expression_list ',' expression{
             ((func*)$1)->vec.push_back($3);
		$<exp_ast>$ = $1;
        }
	;

unary_operator
        : '-'{
        	$$ = new oper1("UMINUS");
        }
	| '!'
	{
		$$ = new oper1("NOT"); 
	}
	;

selection_statement
        : IF '(' expression ')' statement ELSE statement {
        	$<stmt_ast>$ = new If($3, $5, $7);
	
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement{
		$$ = new While($3, $5);
	}	
	| FOR '(' expression ';' expression ';' expression ')' statement  //modified this production
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

declarator_list
	: declarator
	| declarator_list ',' declarator 
 	;


