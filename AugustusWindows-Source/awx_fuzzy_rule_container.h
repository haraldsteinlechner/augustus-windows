#ifndef AWX_FUZZY_RULE_CONTAINER_H
#define AWX_FUZZY_RULE_CONTAINER_H

#include <iostream>
#include <vector>
using namespace std;


#include "awx_fuzzy_variable.h"
#include "awx_fuzzy_rule.h"
#include "awx_fuzzy_utilities.h"

class awx_fuzzy_value
{
	public:
		awx_string variable;
		awx_string state;
		float value;
		
		awx_fuzzy_value()
		{
			variable = "";
			state = "";
			value = 0;
		}
		
		awx_fuzzy_value(awx_string state , float value)
		{
			this->variable = "";
			this->state = state;
			this->value = value;
		}
		
		awx_fuzzy_value(awx_string variable , awx_string state , float value)
		{
			this->variable = variable;
			this->state = state;
			this->value = value;
		}
};

class awx_fuzzy_rule_container
{
	public:
		bool debug;
		int version;
		bool inverted;
		
		vector <awx_fuzzy_variable> variables;
		vector <awx_fuzzy_rule> rules;
		
		vector <float> current_values;
		
	//public:
	
		awx_fuzzy_rule_container();
	
		void addRule(awx_fuzzy_rule &rule);
		void addRule(const char* condition, const char* effect);
		void addVariable(awx_fuzzy_variable &variable);
		
		awx_fuzzy_rule &getRule(int index);
		awx_fuzzy_variable &getVariable(int index);
		
		void setValue(awx_string variable , float value);
		void recurse(int index ,vector <awx_fuzzy_value> &prefix , vector< vector<awx_fuzzy_value> > &before , vector< vector<awx_fuzzy_value> > &after);
		
		bool proveConsistency(awx_string &retString);		
		void calculate(vector <awx_string> &ret_variables , vector <float> &ret_values);
		
		bool writeToFile(char* path);
		bool readFromFile(char* path);
};

#endif