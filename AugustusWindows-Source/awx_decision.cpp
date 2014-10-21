#include "awx_decision.h"

awx_decision::awx_decision()
{
	yesScript = "//yes";
	noScript = "//no:";
	command = "";
    bedingung = "";
    yes = -1;
    no = -1;
}


awx_decision::awx_decision(awx_string command)
{
	awx_decision();
    this->command = command;
}

awx_decision::awx_decision(awx_string bedingung , int yes , int no)
{
	awx_decision();
    this->bedingung = bedingung;
    this->yes = yes;
    this->no = no;
}

awx_decision::awx_decision(awx_string command , awx_string bedingung , int yes , int no)
{
	awx_decision();
	this->command = command;
    this->bedingung = bedingung;
    this->yes = yes;
    this->no = no;
}

void awx_decision::write(FILE* file)
{
	/*
		awx_string command;
		awx_dictionary* variables;
		
		awx_string bedingung;
	
		int yes;
		int no;
	
		awx_string yesScript;
		awx_string noScript;
	*/
	fwrite(&yes,sizeof(yes),1,file);
	fwrite(&no,sizeof(no),1,file);
	
	//command:
	int length = command.length();
	fwrite(&length,sizeof(length),1,file);
	char *currentValue = new char[length+1];
	strcpy(currentValue,command.cString());
	fwrite(currentValue , sizeof(char) , length , file);
	delete[] currentValue;
	
	//bedingung:
	length = bedingung.length();
	fwrite(&length,sizeof(length),1,file);
	currentValue = new char[length+1];
	strcpy(currentValue,bedingung.cString());
	fwrite(currentValue , sizeof(char) , length , file);
	delete[] currentValue;
	
	//yesScript:
	length = yesScript.length();
	fwrite(&length,sizeof(length),1,file);
	currentValue = new char[length+1];
	strcpy(currentValue,yesScript.cString());
	fwrite(currentValue , sizeof(char) , length , file);
	delete[] currentValue;
	
	//noScript:
	length = noScript.length();
	fwrite(&length,sizeof(length),1,file);
	currentValue = new char[length+1];
	strcpy(currentValue,noScript.cString());
	fwrite(currentValue , sizeof(char) , length , file);
	delete[] currentValue;
}

void awx_decision::read(FILE* file)
{
	awx_fread(&yes,sizeof(yes),1,file);
	awx_fread(&no,sizeof(no),1,file);
	
	//command:
	int length = 0;
	char temp[128];
	 
	awx_fread(&length,sizeof(length),1,file);
	fread(temp , sizeof(char) , length , file);
	temp[length] = '\0';
	command = temp;
	
	//bedingung:
	awx_fread(&length,sizeof(length),1,file);
	fread(temp , sizeof(char) , length , file);
	temp[length] = '\0';
	bedingung = temp;
	
	//command:
	awx_fread(&length,sizeof(length),1,file);
	fread(temp , sizeof(char) , length , file);
	temp[length] = '\0';
	yesScript = temp;
	
	//command:
	awx_fread(&length,sizeof(length),1,file);
	fread(temp , sizeof(char) , length , file);
	temp[length] = '\0';
	noScript = temp;
}

bool awx_decision::isTrue()
{
	awx_string temp = replaceVariables(bedingung , *variables);
	//cout << temp << endl;
	bool tempbool = ausdruck(temp);
	//cout <<"#" << temp << " == " << tempbool << "#" << endl;
	return tempbool;
}

int awx_decision::calculate()
{
    bool flag = isTrue();
    
    if(flag)
    {
        //player[controlIndex].execute(yesScript);
        return yes;
    }
    else
    {
        //player[controlIndex].execute(noScript);
        return no;
    }
}

awx_string awx_decision::getScript()
{
	bool flag = isTrue();
	
	if(flag)return yesScript;
	else return noScript;
}

void awx_decision::setDictionary(awx_dictionary &dict)
{
	variables = &dict;
}
