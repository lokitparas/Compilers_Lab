%debug
%scanner Scanner.h
%scanner-token-function d_scanner.lex()

%token VOID INT FLOAT INT_CONSTANT FLOAT_CONSTANT RETURN OR_OP AND_OP EQ_OP NE_OP LE_OP
%token GE_OP INC_OP WHILE FOR IF ELSE IDENTIFIER STRING_LITERAL OTHERS
 


%%
translation_unit :  
    function_definition 
  | translation_unit function_definition 
  ;

function_definition :  
          type_specifier fun_declarator compound_statement 
        ;

type_specifier : 
          VOID  
        | INT   
        | FLOAT
        ; 
        

fun_declarator : 
          IDENTIFIER '(' parameter_list ')' 
          {
            $$= count++;
            DotPrinter("IDENTIFIER",$1,"fun_declarator", $$);
          }
        | IDENTIFIER '(' ')' 
         {
            printf("IDENTIFIER" \n YO");
          }
        ;

parameter_list : 
    parameter_declaration 
  | parameter_list ',' parameter_declaration 
  ;

parameter_declaration : 
          type_specifier declarator 
          ;     

declarator : 
          IDENTIFIER 
        | declarator '[' constant_expression ']' 
        ;

constant_expression : 
          INT_CONSTANT 
        | FLOAT_CONSTANT 
        ;

compound_statement : 
          '{' '}' 
        | '{' statement_list '}' 
        | '{' declaration_list statement_list '}' 
        ;

statement_list : 
          statement   
        | statement_list statement  
        ;

statement : 
          '{' statement_list '}'  //a solution to the local decl problem
        | selection_statement   
        | iteration_statement   
        | assignment_statement  
        | RETURN expression ';' 
        ;

assignment_statement : 
          ';'               
        |  l_expression '=' expression ';' 
        ;

expression : 
          logical_and_expression 
        | expression OR_OP logical_and_expression
        {
          $$ = ($1 || $3);
        }
        ;
{
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"register_exp\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"(\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"REGISTER\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\")\"]\n";

       }

logical_and_expression : 
          equality_expression
          {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"logical_and_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
          }
        | logical_and_expression AND_OP equality_expression 
        {
           $$ = ++nodeCount;
          std::cout << $$ << "[label=\"logical_and_expression\"]\n";
          std::cout << $$ << " -> " <<$1<< "\n";
        }
        ;

equality_expression : 
          relational_expression 
          {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"equality_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
          }
        | equality_expression EQ_OP relational_expression 
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"equality_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"EQ_OP\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
          }
        | equality_expression NE_OP relational_expression
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"equality_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"NE_OP\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
        }
        ;

relational_expression : 
          additive_expression
          {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"relational_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
          }
        | relational_expression '<' additive_expression 
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"relational_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"<\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
        }
        | relational_expression '>' additive_expression
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"relational_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\">\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
        } 
        | relational_expression LE_OP additive_expression
        {
          $$ = ++nodeCount;
            std::cout << $$ << "[label=\"relational_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"LE_OP\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
        } 
        | relational_expression GE_OP additive_expression 
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"relational_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"GE_OP\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
        }
        ;

additive_expression : 
          multiplicative_expression
          {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"additive_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
          }
        | additive_expression '+' multiplicative_expression 
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"additive_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"+\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
        }
        | additive_expression '-' multiplicative_expression 
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"additive_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"-\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
        }
        ;  

multiplicative_expression : 
          unary_expression
          {
          $$ = ++nodeCount;
            std::cout << $$ << "[label=\"multiplicative_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
          }
        | multiplicative_expression '*' unary_expression 
        {
          std::cout << $$ << "[label=\"multiplicative_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"*\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
        }
        | multiplicative_expression '/' unary_expression 
        {
          std::cout << $$ << "[label=\"multiplicative_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"/\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
        }
        ;

unary_expression : 
          postfix_expression
          {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"unary_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
          }
          }          
        | unary_operator postfix_expression 
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"unary_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " <<$2<< "\n";
        ;

postfix_expression : 
          primary_expression
          {
          $$ = ++nodeCount;
            std::cout << $$ << "[label=\"postfix_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
          }
        | IDENTIFIER '(' ')'
        {
          $$ = ++nodeCount;
            std::cout << $$ << "[label=\"postfix_expression\"]\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"IDENTIFIER\"]\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"(\"]\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\")\"]\n";

          }
        | IDENTIFIER '(' expression_list ')' 
        {
        $$ = ++nodeCount;
            std::cout << $$ << "[label=\"postfix_expression\"]\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"IDENTIFIER\"]\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"(\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\")\"]\n";

        }
        | l_expression INC_OP
        {
           $$ = ++nodeCount;
           std::cout << $$ << "[label=\"postfix_expression\"]\n";
           std::cout << $$ << " -> " << $1 << "\n";
           std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"INC_OP\"]\n";
        }
        ;

primary_expression : 
          l_expression
          {
           $$ = ++nodeCount;
           std::cout << $$ << "[label=\"primary_expression\"]\n";
            std::cout << $$ << " -> " << $1 << "\n";
          }
        | l_expression '=' expression 
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"primary_expression\"]\n";
            std::cout << $$ << " -> " <<$1<< "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"=\"]\n";
            std::cout << $$ << " -> " <<$3<< "\n";
        }
        | INT_CONSTANT
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"primary_expression\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"INT_CONSTANT\"]\n";
        }
        | FLOAT_CONSTANT
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"primary_expression\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"FLOAT_CONSTANT\"]\n";
        }
        | STRING_LITERAL
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"primary_expression\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"STRING_LITERAL\"]\n";
        }
        | '(' expression ')'  
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"primary_expression\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << $2<<"\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\")\"]\n";
        }
        ;

l_expression : 
          IDENTIFIER
          {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"l_expression\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"IDENTIFIER\"]\n";
          }
        | l_expression '[' expression ']'  
        {
           $$ = ++nodeCount;
            std::cout << $$ << "[label=\"l_expression\"]\n";
             std::cout << $$ << " -> " << $1 << "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"[\"]\n";
            std::cout << $$ << " -> " << $3 << "\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"]\"]\n";
          } 
        ;

expression_list : 
          expression
          {
          $$ = ++nodeCount;
            std::cout << $$ << "[label=\"expression_list\"]\n";
             std::cout << $$ << " -> " << $1 << "\n";
          }
        | expression_list ',' expression
         {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"expression_list\"]\n";
            std::cout << $$ << " -> " << $1 << "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\",\"]\n";
            std::cout << $$ << " -> " << $3 << "\n";

          }
        ;

unary_operator : 
          '-'
          {
             $$ = ++nodeCount;
            std::cout << $$ << "[label=\"unary_operator\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"-\"]\n";
          } 
        | '!'   
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"unary_operator\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"!\"]\n";
          } 
        ;

selection_statement : 
          IF '(' expression ')' statement ELSE statement 
          {
             $$ = ++nodeCount;
            std::cout << $$ << "[label=\"selection_statement\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"IF\"]\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"(\"]\n";
              std::cout << $$ << " -> " << $3<< "\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\")\"]\n";
            std::cout << $$ << " -> " << $5<< "\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"ELSE\"]\n";
               std::cout << $$ << " -> " << $7<< "\n";
          }
          ;
iteration_statement : 
          WHILE '(' expression ')' statement  
          {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"iteration_statement\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"WHILE\"]\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"(\"]\n";
              std::cout << $$ << " -> " << $3<< "\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\")\"]\n";
            std::cout << $$ << " -> " << $5<< "\n";
          }
        | FOR '(' expression ';' expression ';' expression ')' statement\
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"iteration_statement\"]\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"FOR\"]\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\"(\"]\n";
              std::cout << $$ << " -> " << $3<< "\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\";\"]\n";
            std::cout << $$ << " -> " << $5<< "\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\";\"]\n";
            std::cout << $$ << " -> " << $7<< "\n";
             std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\")\"]\n";
            std::cout << $$ << " -> " << $9<< "\n";
          }
        ;

declaration_list : 
          declaration  
          {
           $$ = ++nodeCount;
            std::cout << $$ << "[label=\"declaration_list\"]\n";
            std::cout << $$ << " -> " << $1 << "\n";
          }         
        | declaration_list declaration
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"declaration_list\"]\n";
            std::cout << $$ << " -> " << $1 << "\n";
            std::cout << $$ << " -> " << $2 << "\n";
        }
        ;

declaration : 
          type_specifier declarator_list';'
          {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"declaration\"]\n";
            std::cout << $$ << " -> " << $1 << "\n";
            std::cout << $$ << " -> " << $2 << "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=;")\"]\n";

        }
          ;
declarator_list : 
          declarator
          {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"declaration_list\"]\n";
            std::cout << $$ << " -> " << $1 << "\n";
          }
        | declarator_list ',' declarator 
        {
            $$ = ++nodeCount;
            std::cout << $$ << "[label=\"declaration_list\"]\n";
            std::cout << $$ << " -> " << $1 << "\n";
            std::cout << $$ << " -> " << ++nodeCount << "\n";
            std::cout << nodeCount << "[label=\",\"]\n";
            std::cout << $$ << " -> " << $3 << "\n";
        }  
        ;