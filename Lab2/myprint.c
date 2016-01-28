#include <iostream>
#include "Scanner.h"
#include "Parser.h"
using namespace std;

class abstract_astnode
{
public:
	virtual void print () = 0;
	
};

class assign_ast{
	public:
	void print(){
		printf("Assignment\n");
	}
};

