#include "awx_fuzzy_utilities.h"



void splitCondition(const char* condition , vector <awx_string> &variables , vector <awx_string> &states , vector <AWX_COMPARE_OPERATOR> &logicals, vector <AWX_LOGIC_OPERATOR> &operators)
{
	
	awx_string theCondition(condition);
	theCondition = theCondition.deleteAllAppearances(" ");
	theCondition = theCondition.deleteAllAppearances("(");
	theCondition = theCondition.deleteAllAppearances(")");

	int start , end;
	bool started = false;
	
	for(int i = 0; i < theCondition.length();i++)
	{
		if(theCondition[i] == '$')
		{
			started = true;
			start = i+1;
		}
		else if(theCondition[i] == '=' &&
				started)
		{
			end = i-1;
			//cout << theCondition << ": " << start << " " << end << endl;
			awx_string temps =  theCondition.substring(start,end);
			variables.push_back(temps);
			logicals.push_back(AWX_EQUALS);
			
			started = false;
		}
		else if(theCondition[i] == '!' && theCondition[i+1] == '=' &&
				started)
		{
			end = i-1;
			//cout << theCondition << ": " << start << " " << end << endl;
			awx_string temps =  theCondition.substring(start,end);
			variables.push_back(temps);
			logicals.push_back(AWX_NOT_EQUALS);
			
			started = false;
		}
	}
	
	started = false;
	
	for(int i = 0; i < theCondition.length()+1;i++)
	{
		if(theCondition[i] == '=')
		{
			started = true;
			start = i+1;
		}
		else if((theCondition[i] == '&' || theCondition[i] == '|' || theCondition[i] == '\0') && started)
		{
			if(variables.size() > 0)
			{
				if(theCondition[i] == '|')operators.push_back(AWX_OR);
				else operators.push_back(AWX_AND);
			}
			end = i-1;

			states.push_back(theCondition.substring(start,end));
			//cout << theCondition.substring(start,end) << endl;
			started = false;
		}
	}
}


void writeString(awx_string str , FILE* file)
{
	int tempi = str.length();
	awx_fwrite(&tempi , sizeof(tempi) , 1 , file);
	if(tempi > 0)fwrite(str.cString() , sizeof(char) , tempi , file);
}

awx_string readString(FILE* file , bool inverted)
{
	int tempi = 0;
	awx_fread(&tempi , sizeof(tempi) , 1 , file , inverted);
	char* temp = new char[tempi+1];
	if(tempi > 0)fread(temp , sizeof(char) , tempi , file);
	temp[tempi] = '\0';
	awx_string str = temp;
	delete[] temp;
	
	return str;
}
