#ifndef AWX_FUZZY_CONDITION_H
#define AWX_FUZZY_CONDITION_H

#include <vector>
#include <iostream>


int awx_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int awx_fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream);
int awx_freadT(void *ptr, size_t size, size_t nmemb, FILE *stream);
int awx_fread(void *ptr, size_t size, size_t nmemb, FILE *stream , bool inverted);

#include "awx_fuzzy_utilities.h"

typedef enum awx_compare_operator
{
	EQUALS,
	NOT_EQUALS
};

typedef enum awx_bool_operator
{
	AND,
	OR
};

class awx_comparison
{
	private:
		bool debug;
		
		awx_string variable;
		awx_string state;
		awx_compare_operator op;
		
		bool validity;
		float value;
		bool set;
		
	public: 
		awx_comparison();
		awx_comparison(awx_string variable , awx_string state , awx_compare_operator op);
		//virtual void autom();
		virtual void setState(awx_string variable , awx_string state , float value);
		
		virtual void writeToFile(FILE* file);
		virtual void readFromFile(FILE* file , bool inverted , bool top = true);
		
		virtual void plot(int level = 0);
		virtual awx_string getString(int level = 0);
		virtual bool prove(float &value);
};

class awx_fuzzy_condition : awx_comparison
{
	private:
		vector <awx_comparison*> subconditions;
		vector <awx_bool_operator> operators;
		
	public:
		awx_fuzzy_condition();
		
		
		bool prove(float &value);
		void interpret(awx_string condition , awx_fuzzy_condition* parent = NULL);
		
		void writeToFile(FILE* file);
		void readFromFile(FILE* file , bool inverted , bool top = true);
		
		void setState(awx_string variable , awx_string state , float value);
		void plot(int level = 0);
		awx_string getString(int level = 0);
		//bool verify(float &value);
		
};

#endif