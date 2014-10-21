 #include "awx_string.h"

#include <sstream>


void awx_string::plot()
{
	cout << this->value << endl;
}


void awx_string::set(char* x)
{
	this->value = x;
}

char* awx_string::cString()
{
	return (char*)value.c_str();
}

int awx_string::length()
{
	return value.length();
}

int awx_string::intValue()
{
	int temp = 0;
	sscanf(value.c_str() , "%d" , &temp);
	return temp;
}

float awx_string::floatValue()
{
	float temp = 0;
	sscanf(value.c_str() , "%f" , &temp);
	return temp;
}

double awx_string::doubleValue()
{
	double temp = 0;
	sscanf(value.c_str() , "%f" , &temp);
	return temp;
}

string awx_string::stringValue()
{
	return value;
}


//Einen Teilstring herausschneiden
awx_string &awx_string::substring(int begin , int end)
{
	if(begin < 0)begin = 0;
	if(end > value.length()-1)end = value.length()-1;
	
	awx_string *temp = new awx_string();
	temp->value = value.substr(begin , end - begin+1);
	
	return *temp;
}

//alle vorkommenden str löschen
awx_string &awx_string::deleteString(char* str)
{
	if(strlen(str) == 0)return *this;
	int index = value.find(str);
	int length = strlen(str);
	awx_string *tempstr = new awx_string(value);
	
	while(index != -1)
	{
		tempstr->value = tempstr->value.erase(index , length);
		index = tempstr->value.find(str);
	}
	
	return *tempstr;
	
}
awx_string &awx_string::deleteString(awx_string &str)
{	
	if(str.length() == 0)return *this;
	int index = value.find(str.value);
	int length = str.length();
	awx_string *tempstr = new awx_string(value);
	
	while(index != -1)
	{
		tempstr->value = tempstr->value.erase(index , length);
		index = tempstr->value.find(str.value);
	}
	
	return *tempstr;
}

//Hinten anhängen
awx_string &awx_string::stringByAppendingString(char* a)
{
	awx_string *tempstr = new awx_string(value);
	tempstr->value = tempstr->value.append(a);
	return *tempstr;
}
awx_string &awx_string::stringByAppendingString(awx_string &a)
{
	awx_string *tempstr = new awx_string(value);
	tempstr->value = tempstr->value.append(a.value);
	return *tempstr;
}
awx_string &awx_string::stringByAppendingInteger(int val)
{
	std::stringstream stringStream;
	stringStream << val;
	string temp = value + stringStream.str();
	
	awx_string *ret = new awx_string(temp);
	return *ret;
}



awx_string &awx_string::stringByAppendingPathComponent(char* a)
{
	char last = *(value.end()-1);
	awx_string *tempstr = new awx_string(value);
	
	if(last == '/')
	{
		tempstr->value = tempstr->value.append(a);
	}
	else
	{
		tempstr->value = tempstr->value.append("/").append(a);
	}
	
	return *tempstr;
}
awx_string &awx_string::stringByAppendingPathComponent(awx_string &a)
{
	awx_string *temp = new awx_string();
	temp->value = stringByAppendingPathComponent((char*)a.value.c_str()).value;
	return *temp;
}


awx_string &awx_string::lastPathComponent()
{
	int index = value.rfind("/");
	if(index != -1)
	{
		awx_string *tempstr = new awx_string(value);
		tempstr->value.erase(0 , index+1);
		return *tempstr;
	}
	else return *this;
}

awx_string &awx_string::pathExtension()
{
	int index = value.rfind(".");
	if(index != -1)
	{
		awx_string *tempstr = new awx_string(value);
		tempstr->value.erase(0 , index+1);
		return *tempstr;
	}
	else return *this;
}

awx_string &awx_string::stringByDeletingExtension()
{
	int index = value.rfind(".");
	if(index != -1)
	{
		awx_string *tempstr = new awx_string(value);
		tempstr->value.erase(index , value.length()-index);
		return *tempstr;
	}
	else return *this;
}		
awx_string &awx_string::stringByDeletingLastPathComponent()
{
	int index = value.rfind("/");
	if(index != -1)
	{
		awx_string *tempstr = new awx_string(value);
		tempstr->value.erase(index , value.length()-index);
		return *tempstr;
	}
	else return *this;
}

awx_string &awx_string::replaceChar(char rep , char newc)
{
	awx_string* tempstr = new awx_string(this->value);
	int index = tempstr->value.rfind(rep);
	while(index != -1)
	{
		tempstr->value[index] = newc;
		index = tempstr->value.rfind(rep);
	}
	
	return *tempstr;
}

awx_string &awx_string::replaceString(char* oldstr , char* newstr)
{
	awx_string* tempstr = new awx_string(this->value);
	int index = tempstr->value.find(oldstr);
	while(index != -1)
	{
		tempstr->value = tempstr->value.replace(index , strlen(oldstr) , newstr);
		index = tempstr->value.find(oldstr);
	}
	
	return *tempstr;
}

awx_string &awx_string::deleteAllAppearances(char* oldstr)
{
	awx_string* tempstr = new awx_string(this->value);
	int index = tempstr->value.find(oldstr);
	while(index != -1)
	{
		tempstr->value.erase(tempstr->value.begin()+index , tempstr->value.begin()+index + strlen(oldstr));
		index = tempstr->value.find(oldstr);
	}
	
	return *tempstr;
}

void awx_string::split(char delimiter , vector<awx_string> &ret_vector)
{
	awx_string* tempstr = new awx_string(this->value);
	ret_vector.clear();
	
	int index = 0;
	
	while(index != -1)
	{
		index = tempstr->value.find(delimiter);
		ret_vector.push_back(tempstr->substring(0,index-1));
		tempstr = new awx_string(tempstr->substring(index+1,tempstr->length()));
	}
	delete tempstr;
}

void awx_string::splitExact(char delimiter , vector<awx_string> &ret_vector)
{
	awx_string* tempstr = new awx_string(this->value);
	ret_vector.clear();
	
	int index = 0;
	
	while(index != -1)
	{
		index = tempstr->value.find(delimiter);
		if(index > 0)ret_vector.push_back(tempstr->substring(0,index-1));
		else ret_vector.push_back("");
		tempstr = new awx_string(tempstr->substring(index+1,tempstr->length()));
	}
	delete tempstr;
}

/*awx_array &awx_string::split(char delimiter)
{
	awx_string* tempstr = new awx_string(value);
	awx_array* tempa = new awx_array();
	
	int index = 0;
	
	while(index != -1)
	{
		index = tempstr->value.find(delimiter);
		tempa->addObject(tempstr->substring(0,index-1));
		tempstr = new awx_string(tempstr->substring(index+1,tempstr->length()));
	}
	
	delete tempstr;
	return *tempa;
}

awx_array &awx_string::split(char* delimiter)
{
	awx_string* tempstr = new awx_string(value);
	awx_array* tempa = new awx_array();
	
	if(this->length() > 0)
	{
		int index = 0;
	
		while(index != -1)
		{
			index = tempstr->value.find(delimiter);
			tempa->addObject(tempstr->substring(0,index-1));
			tempstr = new awx_string(tempstr->substring(index+strlen(delimiter),tempstr->length()));
		}
	}
	
	delete tempstr;
	return *tempa;
}*/

bool awx_string::compare(awx_string str)
{
	return value.compare(str.value);
}


bool awx_string::containsString(awx_string str)
{
	return (value.find(str.value) != -1);
}

bool awx_string::containsString(char* str)
{
	return (value.find(str) != -1);
}

bool awx_string::containsChar(char c)
{
	return (value.find(c) != -1);
}

int awx_string::parseFunctionSyntax(awx_string &functionName, awx_string &functionParam)
{
	if(value.length()<3) 
	{
		cout << "parse error in awx_string: keine so kurzen Befehle m�glich." << endl;
		return -1;
	}
	
	int paramStart = value.find("(");
	int paramEnd =  value.find(");");
	
	if(paramStart<0||paramStart>=value.length()||
	   paramEnd<0||paramEnd>=value.length()) 
	{
	   cout << "parse error in awx_string" << endl;
	   return -1;
	}
	
	functionParam = value.substr(paramStart+1,paramEnd-paramStart-1);
	functionName = value.substr(0,paramStart);
	
	return 0;
}


bool awx_string::readFromFile(char* path)
{
	FILE* file = fopen(path , "r");
	if(file != NULL)
	{
		value = "";
		char c = fgetc(file);
		while(!feof(file))
		{
			value = value + c;
			c = fgetc(file);
		}
		fclose(file);
		return true;
	}
	return false;
}

bool awx_string::readFromFile(awx_string path)
{
	return readFromFile(path.cString());
}

awx_string &awx_string::operator=(const char* val)
{
	value = val;
	return *this;
}

awx_string &awx_string::operator=(awx_string val)
{
	this->value = val.value;
	return *this;
}

bool awx_string::operator == (awx_string b)
{
	return (value.compare(b.value) == 0);
}

bool awx_string::operator == (char* b)
{
	return (value.compare(b) == 0);
}

char awx_string::operator[](int index)
{
	return value[index];
}

awx_string &awx_string::operator+(char val)
{
	awx_string temp(val);
	return this->stringByAppendingString(temp);
}

awx_string &awx_string::operator+(char* val)
{
	return this->stringByAppendingString(val);
}

awx_string &awx_string::operator+(awx_string &val)
{
	return this->stringByAppendingString(val);
}
		
awx_string &awx_string::operator-(char* val)
{
	return this->deleteString(val);
}

awx_string &awx_string::operator-(awx_string &val)
{
	return this->deleteString(val);
}

awx_string &awx_string::operator% (int length)
{
	return this->substring(0,length);
}

awx_string &awx_string::operator%(char* val)
{
	return this->stringByAppendingPathComponent(val);
}

awx_string &operator+(char *val , awx_string &str)
{
	awx_string tempstr(val);
	return tempstr.stringByAppendingString(str);
}

awx_string &operator%(char *val , awx_string &str)
{
	awx_string tempstr(val);
	return tempstr.stringByAppendingPathComponent(str);
}

ostream &operator<<(ostream &outs , awx_string val)
{
	outs << val.cString();
	return outs;
}