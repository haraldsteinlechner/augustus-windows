#include "awx_fuzzy_rule_container.h"

awx_fuzzy_rule_container::awx_fuzzy_rule_container()
{
	rules.clear();
	variables.clear();
	
	debug = true;
	
	version = 3;
}
	
void awx_fuzzy_rule_container::addRule(awx_fuzzy_rule &rule)
{
	rules.push_back(rule);
}

void awx_fuzzy_rule_container::addRule(const char* condition, const char* effect)
{	
	awx_fuzzy_rule temprule;

	vector <awx_string> temp_variables;
	vector <awx_string> v1;
	vector <AWX_LOGIC_OPERATOR> v3;
	vector <AWX_COMPARE_OPERATOR> v2;
	
	splitCondition(condition , temp_variables , v1 , v2 , v3);
	
	for(int i = 0; i < temp_variables.size();i++)
	{
		for(int u = 0; u < variables.size();u++)
		{
			if(variables[u].name.compare(temp_variables[i]) == 0)
			{
				variables[u].inCondition = true;
				break;	
			}
		}
	}

	vector <awx_string> effect_variables;
	vector <awx_string> effect_states;
	vector <AWX_LOGIC_OPERATOR> effect_operators;
	vector <AWX_COMPARE_OPERATOR> effect_logicals;
	
	splitCondition(effect , effect_variables , effect_states , effect_logicals ,effect_operators);
	
	if(effect_variables.size() > 0 && effect_states.size() > 0)
	{
		temprule.effectVariable = effect_variables[0];
		temprule.effectState = effect_states[0];
	
		temprule.interpret((char*)condition);
	
		rules.push_back(temprule);
	}
	//cout << rules.size() << endl;
}

void awx_fuzzy_rule_container::addVariable(awx_fuzzy_variable &variable)
{
	variables.push_back(variable);
	current_values.push_back(0.0);
}
		
awx_fuzzy_rule &awx_fuzzy_rule_container::getRule(int index)
{
	if(index >= rules.size())index = rules.size()-1;
	else if(index < 0) index = 0;
	
	return rules[index];
}

awx_fuzzy_variable &awx_fuzzy_rule_container::getVariable(int index)
{
	if(index >= variables.size())index = variables.size()-1;
	else if(index < 0) index = 0;
	
	return variables[index];
}

void awx_fuzzy_rule_container::setValue(awx_string variable , float value)
{
	for(int i = 0; i < variables.size();i++)
	{
		if(variables[i].name.compare(variable) == 0)
		{
			current_values[i] = value;
			break;
		}
	}
}

void awx_fuzzy_rule_container::recurse(int index ,vector<awx_fuzzy_value> &prefix , vector< vector<awx_fuzzy_value> > &before , vector< vector<awx_fuzzy_value> > &after)
{
    for(int i = 0; i < before[index].size();i++)
    {
        vector <awx_fuzzy_value> temp;
        for(int u = 0; u < prefix.size();u++)
        {
            temp.push_back(prefix[u]);
        }
        temp.push_back(before[index][i]);
        if(index < before.size()-1)recurse(index+1 , temp , before , after);
        else
        {
            after.push_back(temp);
        }
    }	
}

bool awx_fuzzy_rule_container::proveConsistency(awx_string &retString)
{
	vector< vector<awx_fuzzy_value> > list;
	
	for(int i = 0; i < variables.size();i++)
	{
		if(variables[i].inCondition)
		{
			vector <awx_fuzzy_value> temp;
			for(int u = 0; u < variables[i].sets.size();u++)
			{
				temp.push_back(awx_fuzzy_value(variables[i].name , variables[i].sets[u].name , 0.5));
			}
			list.push_back(temp);
		}
	}
	
	vector< vector<awx_fuzzy_value> > retVector;
	vector <awx_fuzzy_value> empty;
	recurse(0,empty , list , retVector);
	
	retString = "";
	bool returnValue = true;
	for(int i = 0; i < retVector.size();i++)
	{
		bool flag = false;
		for(int x = 0; x < rules.size();x++)
		{
			for(int u = 0; u < retVector[i].size();u++)
			{
				rules[x].setState(retVector[i][u].variable , retVector[i][u].state , 0.5);
				//cout << retVector[i][u].variable << " " << retVector[i][u].state << "  ";
			}
			
			//cout << endl;
			
			awx_string effv;
			awx_string effs;
			float effval;
			rules[x].debug = false;
			if(rules[x].approve(effv , effs , effval))
			{
				flag = true;
				break;
			}
		}
		if(!flag)
		{
			retString = retString + retVector[i][0].variable + "=" + retVector[i][0].state;
			for(int u = 1; u < retVector[i].size();u++)
			{
				retString = retString + " && " + retVector[i][u].variable + "=" + retVector[i][u].state;
				cout << retVector[i][u].variable << "=" << retVector[i][u].state << "   ";
			} 
			retString = retString + "\n";
			cout << " undefined!" << endl;
			
			returnValue = false;
		}
	}
	return returnValue;
	
}
		
void awx_fuzzy_rule_container::calculate(vector <awx_string> &ret_variables , vector <float> &ret_values)
{
	if(rules.size() == 0)return;
	
	awx_fuzzy_result *results = new awx_fuzzy_result[variables.size()];
	for(int r = 0; r < variables.size();r++)
	{
		results[r] = variables[r].calculate(current_values[r]);
		//cout << endl;
	}
	
	vector< vector<awx_fuzzy_value> > list;
	
	int count = 0;
	for(int i = 0; i < variables.size();i++)
	{
		if(variables[i].inCondition)count++;
	}
	
	/*
	
Container has 2 Variables:
   $brunnen_count = viel(100.00%)  
   $wirtshaus_count = viel(100.00%)  
VARIABLES END

Container has 3 Rules:
   RULE 1: 
       FALSE = "$brunnen_count = wenig(viel) || $wirtshaus_count = wenig(viel)"
   RULE 2: 
       TRUE  = "$brunnen_count = viel(viel) && $wirtshaus_count = mittel(viel) || $wirtshaus_count = viel(viel) || $brunnen_count = mittel(viel)"
   RULE 3: 
       TRUE  = "$brunnen_count = viel(viel) && $wirtshaus_count = viel(viel)"
   END OF RULES
Container END
	
	*/
	
	if(debug)
	{
		cout << "Container START" << endl;
		cout << endl << "   Container has " << count << " INPUT-VARIABLE" << (count > 1 ? "S" : "") << ":" << endl;
	}
	for(int i = 0; i < variables.size();i++)
	{
		if(variables[i].inCondition)
		{
			vector <awx_fuzzy_value> temp;
			if(debug)
			{
				cout << "     VARIABLE " << i+1 << ":  ";
				cout << "$" << variables[i].name << " = ";
			}
			for(int u = 0; u < results[i].count();u++)
			{
				if(results[i].getValue(u) > 0)
				{
					temp.push_back(awx_fuzzy_value(variables[i].name , results[i].getName(u) , results[i].getValue(u)));
					if(debug)
					{
						cout << results[i].getName(u) << "(";
						printf("%.2f" , results[i].getValue(u)*100);
						cout << "%)  ";
					}
				}
			}
			if(debug)cout << endl;
			list.push_back(temp);
		}
	}
	if(debug)cout << "   VARIABLES END" << endl << endl;
	
	vector< vector<awx_fuzzy_value> > retVector;
	vector <awx_fuzzy_value> empty;
	recurse(0,empty , list , retVector);
	
	vector <int> effectIndices;
	
	for(int x = 0; x < variables.size();x++)
	{
		for(int z = 0; z < variables[x].sets.size();z++)
		{
			variables[x].sets[z].maxValue = 0;
			variables[x].sets[z].marked = false;
		}
	}
	
	if(debug)cout << "   Container has " << rules.size() << " RULE" << (rules.size() > 1 ? "S" : "")  << ":" << endl;
	for(int x = 0; x < rules.size();x++)
	{
		if(debug)cout << "     RULE " << x+1 << ":  " << endl;
		for(int i = 0; i < retVector.size();i++)
		{
			for(int u = 0; u < retVector[i].size();u++)
			{
				rules[x].setState(retVector[i][u].variable , retVector[i][u].state , retVector[i][u].value);
			}
			awx_string effectVariable,effectState;
			float effectValue;
			
			rules[x].debug = debug;
			//if(i != 0)cout << "              ";
			if(rules[x].approve(effectVariable,effectState,effectValue))
			{
				//cout << "       " << effectValue << endl;
				int index = -1;
				for(int w = 0; w < variables.size();w++)
				{
					if(variables[w].name.compare(effectVariable) == 0)
					{
						index = w;
						bool found = false;
						for(int t = 0; t < effectIndices.size();t++)
						{
							if(effectIndices[t] == w)
							{
								found = true;
								break;
							}
						}
						if(!found)effectIndices.push_back(w);
						break;
					}
				}
				if(index > -1)
				{
					variables[index][effectState]->marked = true;
					if(effectValue > variables[index][effectState]->maxValue)variables[index][effectState]->maxValue = effectValue;
				}
			}
		}
	}
	
	if(debug)cout << "   END OF RULES" << endl <<   endl << "Container END" << endl << endl;
	
	for(int i = 0; i < effectIndices.size();i++)
	{
		ret_variables.push_back(variables[effectIndices[i]].name);
		ret_values.push_back(variables[effectIndices[i]].getMassPoint().x);
		//if(debug)cout << endl << endl << "EFFECT" << (effectIndices.size() > 1 ? "S[" : " ") << endl;
	}
	
}

bool awx_fuzzy_rule_container::writeToFile(char* path)
{
	FILE* file = fopen(path , "wb");
	if(file != NULL)
	{
		awx_fwrite(&version , sizeof(version) , 1 , file);
		int deb = debug;
		awx_fwrite(&deb , sizeof(deb) , 1 , file);
		
		int tempi = variables.size();
		awx_fwrite(&tempi,sizeof(tempi),1,file);
		for(int i = 0; i < variables.size();i++)
		{
			variables[i].writeToFile(file);
		}
		
		tempi = rules.size();
		//cout << "size: " << tempi << endl;
		awx_fwrite(&tempi,sizeof(tempi),1,file);
		for(int i = 0; i < rules.size();i++)
		{
			rules[i].writeToFile(file);
		}
		
		fclose(file);
		return true;
	}
	else return false;
}

bool awx_fuzzy_rule_container::readFromFile(char* path)
{
	inverted = false;
	FILE* file = fopen(path , "rb");
	if(file != NULL)
	{
		int vers;
		fread(&vers , sizeof(vers),1, file);
		if(vers > 100)
		{
			inverted = true;	
			vers = version;
		}
		
		if(debug)cout << (inverted ? "inverted" : "not inverted") << endl;
			
		int senseless;
		awx_fread(&senseless , sizeof(senseless) , 1 , file , inverted);

		//cout << senseless << endl;
		if(vers != version)return false;
		
		int tempi = 0;
		awx_fread(&tempi,sizeof(tempi),1,file , inverted);
		//cout << "tempi=" << tempi << endl;
		for(int i = 0; i < tempi;i++)
		{
			awx_fuzzy_variable tempvar;
			tempvar.readFromFile(file , inverted);
			variables.push_back(tempvar);
			
			current_values.push_back(0);
		
		}
		tempi = 0;
		awx_fread(&tempi,sizeof(tempi),1,file , inverted);
		
		for(int i = 0; i < tempi;i++)
		{
			awx_fuzzy_rule temprule;
			temprule.readFromFile(file , inverted);
			
			//int index = -1;
			awx_string str = temprule.condition.getString();
			vector <awx_string> temp_variables;
			vector <awx_string> v1;
			vector <AWX_LOGIC_OPERATOR> v2;
			vector <AWX_COMPARE_OPERATOR> v3;
			
			splitCondition(str.cString() , temp_variables, v1 , v3, v2);
			
			for(int u = 0; u < variables.size();u++)
			{
				for(int x = 0; x < temp_variables.size();x++)
				{
					if(temp_variables[x].compare(variables[u].name) == 0)
					{
						variables[u].inCondition = true;
						break;
					}
				}
			}
			//if(index >= 0)variables[index].inCondition = true;
			
			rules.push_back(temprule);
		}
				
		fclose(file);
		if(debug)cout << "Container loaded successfully! (" << variables.size() << " Variables, " << rules.size() << " Rules)" << endl << endl;
		return true;
	}
	else return false;	
}
		
