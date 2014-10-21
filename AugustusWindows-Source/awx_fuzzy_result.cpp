#include "awx_fuzzy_result.h"

awx_fuzzy_result::awx_fuzzy_result(awx_string &root)
{
	this->root = root;
}

awx_fuzzy_result::awx_fuzzy_result()
{
	this->root = "";
}

int awx_fuzzy_result::count()
{
	return values.size();
}
		
void awx_fuzzy_result::clear()
{
	names.clear();
	values.clear();
}

void awx_fuzzy_result::addResult(awx_string &name , float value)
{
	names.push_back(name);
	values.push_back(value);
}
		
awx_string &awx_fuzzy_result::getName(int index)
{
	if(index >= names.size())index = names.size()-1;
	if(index < 0)index = 0;
	
	return names[index];
}

float awx_fuzzy_result::getValue(awx_string &name)
{
	int index = -1;
	for(int i = 0; i < names.size();i++)
	{
		if(names[i].compare(name) == 0)
		{
			index = i;
			break;
		}
	}
	
	if(index >= 0)
	{
		return values[index];
	}
	else return 0;
}

float awx_fuzzy_result::getValue(int index)
{
	if(index >= values.size())index = values.size()-1;
	if(index < 0)index = 0;
	
	return values[index];
}

float awx_fuzzy_result::operator[](char* temp)
{
	return getValue(*(new awx_string(temp)));
}
