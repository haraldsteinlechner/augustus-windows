#ifndef AWX_FUZZY_UTILITIES
#define AWX_FUZZY_UTILITIES

#include <iostream>
#include <vector>
using namespace std;

#include "awx_string.h"

#include "awx_cpp_utilities.h"


typedef enum AWX_LOGIC_OPERATOR
{
	AWX_AND,
	AWX_OR
};

typedef enum AWX_COMPARE_OPERATOR
{
	AWX_EQUALS,
	AWX_NOT_EQUALS,
	AWX_GREATER,
	AWX_SMALLER
};

void splitCondition(const char* condition , vector <awx_string> &variables , vector <awx_string> &states , vector <AWX_COMPARE_OPERATOR> &logicals, vector <AWX_LOGIC_OPERATOR> &operators);


void writeString(awx_string str , FILE* file);
awx_string readString(FILE* file , bool inverted);

#endif