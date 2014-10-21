#include "awx_functionDictionary.h"


awx_simpleFunctionDictionary::awx_simpleFunctionDictionary()
{
}
	
	
awx_simpleFunctionDictionary::~awx_simpleFunctionDictionary()
{	
	dealloc();
}
	
void awx_simpleFunctionDictionary::dealloc()
{
	functions.clear();
	keys.clear();
}
	
void awx_simpleFunctionDictionary::addFunctionWithKey(int (*fp)(void* data), awx_string key)
{
	keys.push_back(key);
		
	simpleFunction tempFunctionStruct;
	tempFunctionStruct.fp = fp;
	functions.push_back(tempFunctionStruct);
}

int awx_simpleFunctionDictionary::executeFunctionWithKey(awx_string key, void *data)
{
	for(int i=0;i<keys.size();i++)
	{
		if(key==keys[i])
		{
			return executeFunctionWithIndex(i,data);
		}
	}
		
	cout << "Requested Function for key: " << key << " not found." << endl;
	return -1;
}
	
int awx_simpleFunctionDictionary::executeFunctionWithIndex(int index, void *data)
{
	if(index<0||index>=functions.size()) 
	{
		cout << "Function of functionDictionary out of bounds. Stop." << endl;
		return -1;
	}
		
	return functions[index].exec(data);
}
	
int awx_simpleFunctionDictionary::numberOfItems() const
{
	return functions.size();
}
