#ifndef AWX_FUNCTION_DICTIONARY
#define AWX_FUNCTION_DICTIONARY

#include <iostream>
using namespace std;

#include <vector>

#include "awx_string.h"

struct simpleFunction
{
	int (*fp)(void* data);
	
	int exec(void *data)
	{
		return (*fp)(data);
	}
};

class awx_simpleFunctionDictionary
{
	private:
	
	vector<awx_string> keys;
	vector<simpleFunction> functions;
	
	public:
	
	awx_simpleFunctionDictionary();
	
	~awx_simpleFunctionDictionary();
	
	void dealloc();
	
	void addFunctionWithKey(int (*fp)(void* data), awx_string key);


	/*	
	
	Stack: 
	
	int intstack=3;
	func.executeFunctionWithKey("peppo",(void*) intstack);
	intern: (int) data
	
	Pointer:
	
	int *integer = new int;
	*integer=3;
	func.executeFunctionWithKey("peppo",(void*) *integer);
	intern: (int) data
			(char*)data
	
	NULL:
	
	func.executeFunctionWithKey("peppo",NULL);
	
	
	
	*/
	
	
	int executeFunctionWithKey(awx_string key, void *data);
	
	int executeFunctionWithIndex(int index, void *data);
	
	int numberOfItems() const;
};


#endif