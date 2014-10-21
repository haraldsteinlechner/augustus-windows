#include "awx_fuzzy_condition.h"


awx_comparison::awx_comparison()
{
	validity = false;
	set = false;
	debug=false;
	
	value = 0;
}

awx_comparison::awx_comparison(awx_string variable , awx_string state , awx_compare_operator op)
{
	this->variable = variable;
	this->state = state;
	this->op = op;

	
	validity = false;
	debug=false;
} 

void awx_comparison::setState(awx_string variable , awx_string state , float value)
{
	if(variable.compare(this->variable) == 0)
	{
		set = true;
		if(state.compare(this->state) == 0)
		{
			if(this->op == EQUALS)
			{
				validity = true;
			}
			else validity = false;
		}
		else
		{
			if(this->op == NOT_EQUALS)
			{
				validity = true;
			}
			else validity = false;
		}
		this->value = value;
	}
}

awx_string awx_comparison::getString(int level)
{
	const char* temp  = (op == EQUALS ? " = " : " != ");
	return "$" + variable.stringByAppendingString((char*)temp).stringByAppendingString(state);
}

void awx_comparison::plot(int level)
{
	cout << variable << (op == EQUALS ? " = " : "!=") << state;
}

bool awx_comparison::prove(float &value)
{
	if(!set)
	{
		cout << "ERROR: " << variable <<" has never been set!" << endl;
		exit(0);
	}
	
	if(validity)
	{
		value = this->value;
		if(debug)cout << variable << (op == EQUALS ? " = " : " != ") << state << " " << value << endl;
	}
	return validity;
}

void awx_comparison::writeToFile(FILE* file)
{
	char c = 'c';
	fwrite(&c,sizeof(c),1,file);
	writeString(variable,file);
	writeString(state,file);
	awx_fwrite(&op,sizeof(op),1,file);
}

void awx_comparison::readFromFile(FILE* file , bool inverted , bool top)
{
	variable = readString(file , inverted);
	state = readString(file , inverted);
	/*char temp[256];
	int l = 0;
	cout << awx_fread(&l , sizeof(l) , 1 ,file , inverted) << endl;
	fread(temp , sizeof(char) , l , file);
	temp[l+1] = '\0';*/
	
	//cout << "#" << variable << "#" << state << "#" << endl; 
	awx_fread(&op,sizeof(op),1,file , inverted);
}


awx_fuzzy_condition::awx_fuzzy_condition()
{
}

void awx_fuzzy_condition::interpret(awx_string condition , awx_fuzzy_condition* parent)
{
	condition = condition.deleteAllAppearances(" ");
	char* temp = condition.cString();
	
	
	int klammerCounter = 0;
	int klammerStart = -1;
	int klammerEnd = -1;
	bool everStarted = false;
	
	int varStart = -1;
	int stateStart = -1;
	
	awx_string cur_variable;
	awx_compare_operator cur_op;
	
	for(int i = 0; i < strlen(temp);i++)
	{
		if(temp[i] == '(')
		{
			if(klammerStart == -1)klammerStart = i+1;
			klammerCounter++;
			everStarted = true;
		}
		else if(temp[i] == ')')
		{
			klammerEnd = i-1;
			klammerCounter--;
		}
		
		if(klammerCounter == 0 && klammerStart > -1)
		{
			if(klammerStart-3 > 0 && (temp[klammerStart-3] == '&' || temp[klammerStart-3] == '|'))
			{
				operators.push_back( (temp[klammerStart-3] == '&' ? AND : OR) );
			}
			awx_fuzzy_condition* tempc = new awx_fuzzy_condition();
			tempc->interpret(condition.substring(klammerStart,klammerEnd) , this);
			subconditions.push_back(tempc);
			
			klammerStart = -1;
			klammerEnd = -1;
		}
		
		if(klammerCounter == 0)
		{
			if(temp[i] == '$')
			{
				varStart = i+1;
			}
			else if(temp[i] == '=' || (temp[i] == '!' && temp[i+1] == '='))
			{
				if(varStart > -1)
				{
					if(varStart-3 >= 0 && (temp[varStart-3] == '&' || temp[varStart-3] == '|'))
					{
						operators.push_back( (temp[varStart-3] == '&' ? AND : OR) );
					}
					int end = i+1;
					if(temp[i+1] == '=')end++;
					cur_variable = condition.substring(varStart,i-1);
					cur_op = (temp[i] == '!' ? NOT_EQUALS : EQUALS);
					stateStart = end;
					varStart = -1;
				}
			}
			else if(temp[i] == '&' || temp[i] == '|' || i == strlen(temp)-1)
			{
				if(stateStart > -1)
				{
					int end = i;
					if(temp[i] == '&' || temp[i] == '|')end--;
					awx_comparison* comp = new awx_comparison(cur_variable,condition.substring(stateStart,end),cur_op);
					subconditions.push_back(comp);
					stateStart = -1;
				}
			}
		}
	}
}

void awx_fuzzy_condition::setState(awx_string variable , awx_string state , float value)
{
	for(int i = 0; i < subconditions.size();i++)
	{
		subconditions[i]->setState(variable , state , value);
	}
}

void awx_fuzzy_condition::writeToFile(FILE* file)
{
	char c = 'd';
	fwrite(&c,sizeof(c),1,file);
	int tempi = operators.size();
	//cout << "tempi: " << tempi << endl;
	//fwrite(&tempi, sizeof(tempi) , 1, file);
	awx_fwrite(&tempi,sizeof(tempi),1,file);
	
	for(int i = 0; i < operators.size();i++)
	{
		awx_fwrite(&operators[i],sizeof(operators[i]),1,file);
	}
	
	tempi = subconditions.size();
	awx_fwrite(&tempi,sizeof(tempi),1,file);
	
	for(int i = 0; i < subconditions.size();i++)
	{
		subconditions[i]->writeToFile(file);
	}
}

void awx_fuzzy_condition::readFromFile(FILE* file ,bool inverted, bool top)
{
	//cout << "RULE" << endl;
	for(int i = 0; i < subconditions.size();i++)
	{
		delete subconditions[i];
	}
	subconditions.clear();
	
	if(top)
	{
		char c = '0';
		fread(&c,sizeof(c),1,file);
		if(c != 'd')
		{
			//cout << "scheisse" << endl;
			return;
		}
	}
	
	int tempi = 0;
	//cout << inverted << endl;
	awx_fread(&tempi,sizeof(tempi),1,file , inverted);
	//cout << "operators: " << tempi << endl;
	for(int i = 0; i < tempi;i++)
	{
		awx_bool_operator comp;
		awx_fread(&comp,sizeof(comp),1,file , inverted);
		
		operators.push_back(comp);
	}
	
	awx_fread(&tempi,sizeof(tempi),1,file , inverted);
	//cout << "subconditions: " << tempi << endl;
	for(int i = 0; i < tempi;i++)
	{
		char c = '0';
		fread(&c , sizeof(c) , 1 , file);
		
		if(c == 'c')
		{
			awx_comparison *comp = new awx_comparison();
			//cout << "READ COMP" << endl;
			comp->readFromFile(file , inverted , false);
			subconditions.push_back(comp);
		}
		else if(c == 'd')
		{
			awx_fuzzy_condition *cond = new awx_fuzzy_condition();
			//cout << "READ CONDITION" << endl;
			cond->readFromFile(file , inverted , false);
			subconditions.push_back(cond);
		}
	}
}

awx_string awx_fuzzy_condition::getString(int level)
{
	awx_string ret = "";
	if(level > 0)ret = "(";
	for(int i = 0; i < subconditions.size();i++)
	{
		if(i > 0)
		{
			char* temp = (char*)(operators[i-1] == AND ? " && " : " || ");
			awx_string substr =subconditions[i]->getString(level+1);
			ret = (ret.stringByAppendingString(temp)).stringByAppendingString(substr);
		}
		else
		{
			awx_string substr =subconditions[i]->getString(level+1);
			ret = ret.stringByAppendingString(substr);
		}
	}
	if(level > 0)ret = ret + ")";
	return ret;
}

void awx_fuzzy_condition::plot(int level)
{
	if(level > 0)cout << "(";
	for(int i = 0; i < subconditions.size();i++)
	{
		if(i > 0)cout << (operators[i-1] == AND ? " && " : " || ");
		subconditions[i]->plot(level+1);
	}
	if(level > 0)cout << ")";
	else cout << endl;
}

bool awx_fuzzy_condition::prove(float &value)
{
	int trueCounter = 0;
	bool val = false;
	
	for(int i = 0; i < subconditions.size();i++)
	{
		float val2 = 0;
		bool cur_val = subconditions[i]->prove(val2);
		
		
		if(i == 0)
		{
			if(cur_val)
			{
				value = val2;
				trueCounter++;
			}
			
			val = cur_val;
		}
		else
		{
			if(operators[i-1] == AND)
			{
				if(cur_val)
				{
					value = (value < val2 ? value : val2);//(value*trueCounter + val2)/(trueCounter+1);
					trueCounter++;
				}
				val = val && cur_val;
			}
			else
			{
				if(cur_val)
				{
					value = (value > val2 ? value : val2);
					trueCounter = 1;
				}
				val = val || cur_val;
			}
		}
	}
	
	//value /= trueCounter;
	
	return val;
}

