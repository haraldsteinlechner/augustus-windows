#ifndef AWX_FUZZY_RULE_H
#define AWX_FUZZY_RULE_H

#include <iostream>
#include <vector>
using namespace std;

#include "awx_fuzzy_utilities.h"
#include "awx_fuzzy_variable.h"
#include "awx_fuzzy_condition.h"

class awx_fuzzy_rule
{
	public:
		bool debug;
		awx_fuzzy_condition condition;
	
		awx_string effectVariable;
		awx_string effectState;
		
		awx_fuzzy_rule();
		
		void interpret(char* condition);
		void setState(awx_string variable , awx_string state , float membership);
		bool approve(awx_string &effectVariable , awx_string &effectState , float &membership);
		
		awx_string conditionString();
		awx_string effectString();
		
		bool writeToFile(FILE* file);
		bool readFromFile(FILE* file , bool inverted);
};

#endif