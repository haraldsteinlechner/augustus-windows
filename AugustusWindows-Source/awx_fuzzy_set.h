#ifndef AWX_FUZZY_SET_H
#define AWX_FUZZY_SET_H

#include <iostream>
#include <vector>
using namespace std;


#include "awx_vector.h"
#include "awx_fuzzy_utilities.h"
class awx_fuzzy_set
{
	public:
		awx_string name;
		vector <awx_vector2f> points;
		void sort();

		bool marked;
		float maxValue;
		
		awx_fuzzy_set();
		
		void addPoint(awx_vector2f point);
		awx_vector2f &getPoint(int index);
		
		bool containsValue(float value);
		
		awx_string &getName();
		void setName(awx_string &name);
		
		float getValue(float x);
		
		bool writeToFile(FILE* file);
		bool readFromFile(FILE* file , bool inverted);
};

#endif
