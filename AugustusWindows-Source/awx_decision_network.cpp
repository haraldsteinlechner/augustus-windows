#include "awx_decision_network.h"

awx_string awx_decision_network::getFullScriptFor(awx_string command)
{
    int index = -1;
    
	
    for(int i = 0; i < knots.size();i++)
    {
		knots[i].setDictionary(*variables);
        if(knots[i].command.compare(command) == 0)
        {
            index = i;
            break;
        }
    }
    
    if(index >= 0)
    {
        int next = index;
        awx_string fullScript = "";

        
        while(next != -1)
        {
			knots[next].setDictionary(*variables);
			awx_string additional = knots[next].getScript();
			if(additional.length()>1)
			{
				fullScript = fullScript.stringByAppendingString(additional);
				fullScript = fullScript + "\n";
			}
            next = knots[next].calculate();
        }
        
        return fullScript;
    }
    else return awx_string("");
}

void awx_decision_network::addDecision(awx_string command , awx_string parentCommand , bool parentValue , awx_string bedingung)
{
    int index = -1;
    
    for(int i = 0; i < knots.size();i++)
    {
        if(knots[i].command.compare(parentCommand) == 0)
        {
            index = i;
            break;
        }
    }
    
    if(index >= 0)
    {
		awx_decision dec(command , bedingung , -1 , -1);
		dec.variables = variables;
		
        knots.push_back(dec);
    
        if(parentValue)
        {
            knots[index].yes = knots.size()-1;
        }
        else
        {
            knots[index].no = knots.size()-1;
        }
    }
    else
    {	
		awx_decision dec(command , bedingung , -1 , -1);
		dec.variables = variables;
		
        knots.push_back(dec);
    }
}

void awx_decision_network::addDecision(awx_decision dec , awx_string parentCommand , bool parentValue)
{
    int index = -1;
    
    for(int i = 0; i < knots.size();i++)
    {
        if(knots[i].command.compare(parentCommand) == 0)
        {
            index = i;
            break;
        }
    }
    
    if(index >= 0)
    {
		dec.variables = variables;
		
        knots.push_back(dec);
    
        if(parentValue)
        {
            knots[index].yes = knots.size()-1;
        }
        else
        {
            knots[index].no = knots.size()-1;
        }
    }
    else
    {
		dec.variables = variables;
        knots.push_back(dec);
    }
    
}

void awx_decision_network::setDictionary(awx_dictionary &dict)
{
	variables = &dict;
}

bool awx_decision_network::writeToFile(FILE* file)
{
	if(file != NULL)
	{
		int temp = knots.size();
		fwrite(&temp,sizeof(temp),1,file);

		for(int i = 0; i < temp;i++)
		{
			knots[i].write(file);
		}
		
		fclose(file);
		return true;
	}
	return false;
	 
}

bool awx_decision_network::readFromFile(FILE* file)
{
	knots.clear();
	if(file != NULL)
	{
		int temp = 0;
		awx_fread(&temp,sizeof(temp),1,file);


		for(int i = 0; i < temp;i++)
		{
			awx_decision tempd;
			tempd.read(file);
			knots.push_back(tempd);
		}
		
		fclose(file);
		return true;
	}
	return false;
}