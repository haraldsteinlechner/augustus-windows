#include "awx_fuzzy_rule.h"

awx_fuzzy_rule::awx_fuzzy_rule()
{
	effectState = "";
	effectVariable = "";
	
	debug = true;
}

void awx_fuzzy_rule::interpret(char* condition)
{
	this->condition.interpret(awx_string(condition));
}

void awx_fuzzy_rule::setState(awx_string variable , awx_string state , float membership)
{
	condition.setState(variable,state,membership);
}

bool awx_fuzzy_rule::approve(awx_string &effectVariable , awx_string &effectState , float &membership)
{
	bool tempb = condition.prove(membership);
	effectVariable = this->effectVariable;
	effectState = this->effectState;
	
	if(debug)cout << "        \"" << condition.getString() << "\" -> " << effectString() << endl;

	
	return tempb;
}

awx_string awx_fuzzy_rule::conditionString()
{
	awx_string a = condition.getString();
	return a;
}

awx_string awx_fuzzy_rule::effectString()
{
	return "$" + effectVariable + " = " + effectState;
}

bool awx_fuzzy_rule::writeToFile(FILE* file)
{
	condition.writeToFile(file);
	
	writeString(effectVariable , file);
	writeString(effectState , file);
	return true;
}

bool awx_fuzzy_rule::readFromFile(FILE* file , bool inverted)
{
	condition.readFromFile(file , inverted);
	
	effectVariable = readString(file , inverted);
	effectState = readString(file , inverted);	
	return true;
}
		