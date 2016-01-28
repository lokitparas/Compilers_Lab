%%
[ \t]                                    					//skip
"void"										return Parser::VOID;
"int"										return Parser::INT;
"float"										return Parser::FLOAT;
((-?)[1-9][0-9]*)|(0[bB][01]*)|(0[0-7]*)|(0[xX][0-9a-fA-F]*)			return Parser::INT_CONSTANT;
(-?)([0-9]+|([0-9]*"."[0-9]*))([eE]([+-]?)[0-9]+)?			return Parser::FLOAT_CONSTANT;
"return "									return Parser::RETURN;
"||"										return Parser::OR_OP;
"&&"										return Parser::AND_OP;
"=="										return Parser::EQ_OP;
"!="										return Parser::NE_OP;
"<="										return Parser::LE_OP;
">="										return Parser::GE_OP;
"++"										return Parser::INC_OP;
"while"										return Parser::WHILE;
"for"										return Parser::FOR;
"if"										return Parser::IF;
"else"										return Parser::ELSE;
[A-Za-z_][_a-zA-Z0-9]*						return Parser::IDENTIFIER;
"("											return '(';
")"											return ')';
"["											return '[';
"]"											return ']';
"{"											return '{';
"}"											return '}';
"+"											return '+';
"="											return '=';
";"											return ';';
\"(\\.|[^"])*\"									return Parser::STRING_LITERAL;
"/*"[^\*]*(\*([^\*\/][^\*]*)?)*"*/"			//skip
.                                           					return Parser::OTHERS;                                          
