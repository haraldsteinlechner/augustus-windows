#ifndef AWX_FUZZY_RESULT_H
#define AWX_FUZZY_RESULT_H

#include <iostream>
#include <vector>

using namespace std;

#include "awx_string.h"

class awx_fuzzy_result
{
	private:
		vector <float> values;
		vector <awx_string> names;
		awx_string root;
	
	public:
		awx_fuzzy_result(awx_string &root);
		awx_fuzzy_result();
		
		void clear();
		void addResult(awx_string &name , float value);
		
		int count();
		
		awx_string &getName(int index);
		float getValue(awx_string &name);
		float getValue(int index);
		
		float operator[](char* temp);
};

#endif