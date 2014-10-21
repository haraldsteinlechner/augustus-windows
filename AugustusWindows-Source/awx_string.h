#ifndef AWX_STRING_H
#define AWX_STRING_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class awx_string
{
	private:
		string value;
	public:
	
	//Konstruktoren:
		awx_string()
		{
			value = "";
		}
		
		awx_string(char val)
		{
			char temp[128];
			sprintf(temp , "%c" , val);
			value = temp;
		}
		
		awx_string(int val)
		{
			char temp[128];
			sprintf(temp , "%d" , val);
			value = temp;
		}
		
		awx_string(float val)
		{
			char temp[128];
			sprintf(temp , "%f" , val);
			value = temp;
		}
		
		awx_string(char* val)
		{
			value = val;
		}
		
		awx_string(const char* val)
		{
			value = val;
		}
		
		awx_string(awx_string& val)
		{
			value = val.value;
		}
		
		awx_string(const awx_string& val)
		{
			value = val.value;
		}
		
		awx_string(string val)
		{
			value = val;
		}
		
		
		void plot();
		bool compare(awx_string str);
		
		bool containsString(awx_string str);
		bool containsString(char* str);
		bool containsChar(char c);
		
		void set(char* x);
		char* cString();
		int length();
		
		int intValue();
		float floatValue();
		double doubleValue();
		string stringValue();
		
		
		awx_string &substring(int begin , int end);
		awx_string &deleteString(char* str);
		awx_string &deleteString(awx_string &str);
		awx_string &stringByAppendingString(char* a);
		awx_string &stringByAppendingString(awx_string &a);
		awx_string &stringByAppendingInteger(int val);
		awx_string &stringByAppendingPathComponent(char* a);	
		awx_string &stringByAppendingPathComponent(awx_string &a);	
		awx_string &lastPathComponent();
		awx_string &pathExtension();
		awx_string &stringByDeletingExtension();		
		awx_string &stringByDeletingLastPathComponent();
		awx_string &replaceChar(char rep , char newc);
		awx_string &replaceString(char* oldstr , char* newstr);
		
		int parseFunctionSyntax(awx_string &functionName, awx_string &functionParam);
		
		awx_string &deleteAllAppearances(char *oldstr);
		
		bool readFromFile(char* path);
		bool readFromFile(awx_string path);
		
		//bool writeToFile(char* path);
		//bool writeToFile(awx_string path);
		
		void split(char delimiter , vector<awx_string> &ret_vector);
		void splitExact(char delimiter , vector<awx_string> &ret_vector);
		
		bool operator== (awx_string b);
		bool operator== (char* b);
		
		char operator[](int index);
		awx_string &operator=(awx_string val);
		awx_string &operator=(const char* val);
	
		awx_string &operator+(char val);
		awx_string &operator+(char* val);
		awx_string &operator+(awx_string &val);
		
		//awx_string &operator-(char val);
		awx_string &operator-(char* val);
		awx_string &operator-(awx_string &val);
		
		awx_string &operator%(int length);
		
		awx_string &operator%(char* val);
		
};

awx_string &operator+(char *val , awx_string &str);

awx_string &operator%(char *val , awx_string &str);

ostream &operator<<(ostream &outs , awx_string val);

#endif