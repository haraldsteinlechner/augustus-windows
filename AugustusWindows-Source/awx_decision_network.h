#ifndef AWX_DECISION_NETWORK_H
#define AWX_DECISION_NETWORK_H


#include <vector>
#include "awx_decision.h"
#include "awx_dictionary.h"
#include "awx_cpp_utilities.h"

#include "awx_ki.h"

class awx_decision_network
{
	public:
		vector <awx_decision> knots;
		
		awx_dictionary *variables;
		
		awx_string getFullScriptFor(awx_string command);
		
		void addDecision(awx_string command , awx_string parentCommand , bool parentValue , awx_string bedingung);
		void addDecision(awx_decision dec , awx_string parentCommand , bool parentValue);
		void setDictionary(awx_dictionary &dict);
		
		bool writeToFile(FILE* file);
		bool readFromFile(FILE* file);

};

#endif