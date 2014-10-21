#ifndef AWX_FUZZY_VARIABLE_H
#define AWX_FUZZY_VARIABLE_H

#include <iostream>
#include <vector>

using namespace std;


#include "awx_fuzzy_set.h"
#include "awx_fuzzy_result.h"


class awx_fuzzy_variable
{
	public:
		awx_string name;
		vector <awx_fuzzy_set> sets;
		
		float minimum;
		float maximum;
		
		bool inCondition;
		
		void sort();

		awx_fuzzy_variable();
	
		void addFuzzySet(awx_fuzzy_set set);
		
		awx_vector2f &getMassPoint();
		awx_fuzzy_result &calculate(float value);
		
		bool writeToFile(FILE* file);
		bool readFromFile(FILE* file , bool inverted);
		
		bool writeToFile(char* path);
		bool readFromFile(char* path , bool inverted);
		
		awx_fuzzy_set *operator[](char* name);
		awx_fuzzy_set *operator[](awx_string &name);
};

#endif