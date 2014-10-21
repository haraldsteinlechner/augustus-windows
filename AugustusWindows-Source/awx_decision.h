#ifndef AWX_DECISION_H
#define AWX_DECISION_H

#include "awx_string.h"
#include "awx_dictionary.h"
#include "awx_ki.h"
#include "awx_cpp_utilities.h"

class awx_decision
{
	public:
		awx_string command;
		awx_dictionary *variables;
		
		awx_string bedingung;
	
		int yes;
		int no;
	
		awx_string yesScript;
		awx_string noScript;
				
		awx_decision();
		awx_decision(awx_string command);
		awx_decision(awx_string bedingung , int yes , int no);
		awx_decision(awx_string command , awx_string bedingung , int yes , int no);
		
		bool isTrue();
		int calculate();
		awx_string getScript();
		void write(FILE* file);
		void read(FILE* file);
		
		void setDictionary(awx_dictionary &dict);
	
};

#endif