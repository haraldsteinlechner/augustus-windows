#include "awx_fuzzy_variable.h"

void awx_fuzzy_variable::addFuzzySet(awx_fuzzy_set set)
{
	sets.push_back(set);
	sort();
}

void awx_fuzzy_variable::sort()
{
	bool sorted = false;
	while(!sorted)
	{
		sorted = true;
		for(int i = 0; i < sets.size()-1;i++)
		{
			if(sets[i].points[0].x > sets[i+1].points[0].x)
			{
				awx_fuzzy_set temp = sets[i+1];
				sets[i+1] = sets[i];
				sets[i] = temp;
				sorted = false;
			}
		}
	}
}

awx_fuzzy_variable::awx_fuzzy_variable()
{
	minimum = 0;
	maximum = 4;
	
	inCondition = false;
	
	name = "newvar";
}

awx_vector2f &awx_fuzzy_variable::getMassPoint()
{
	float start = sets[0].points[0].x;
	float end = sets[sets.size()-1].points[sets[sets.size()-1].points.size()-1].x;
		
	float delta = 0.1;
		
	float surface = 0;
	float weighted = 0;
	float weightedy = 0;
		
	for(float x = start;x <= end;x+=delta)
	{
		int index = -1;
		for(int i = 0; i < sets.size();i++)
		{
			if(sets[i].containsValue(x))
			{
				index = i;
				break;
			}
		}
		if(index > -1 && sets[index].marked)
		{
			if(index < sets.size()-1 && sets[index+1].containsValue(x))
			{
				float v1 = sets[index].getValue(x);
				if(v1 > sets[index].maxValue)v1 = sets[index].maxValue;
				
				float v2 = sets[index].getValue(x);
				if(v2 > sets[index].maxValue)v2 = sets[index].maxValue;
				
				v1 = (v1 > v2 ? v1 : v2);
				
				surface += v1*delta;
				weighted += x*v1*delta;
				weightedy += (v1/2.0)*delta*v1;
			}
			else
			{
				float temp = sets[index].getValue(x);
				if(temp > sets[index].maxValue)temp = sets[index].maxValue;
					
				surface += temp*delta;
				weighted += x*temp*delta;
				weightedy += (temp/2.0)*delta*temp;
			}
		}
	}
	float retX = weighted/surface;
	return *(new awx_vector2f( (retX/4.0)*(maximum-minimum) + minimum , weightedy/surface));

}
		
awx_fuzzy_set *awx_fuzzy_variable::operator[](char* name)
{
	int index = -1;
	for(int i = 0; i < sets.size();i++)
	{
		if(sets[i].name.compare(name) == 0)
		{
			index = i;
			break;
		}
	}
	if(index >= 0 && index < sets.size())
	{
		return &sets[index];
	}
	else return NULL;
}

awx_fuzzy_set *awx_fuzzy_variable::operator[](awx_string &name)
{
	int index = -1;
	for(int i = 0; i < sets.size();i++)
	{
		if(sets[i].name.compare(name) == 0)
		{
			index = i;
			break;
		}
	}
	if(index >= 0 && index < sets.size())
	{
		return &sets[index];
	}
	else return NULL;	
}

awx_fuzzy_result &awx_fuzzy_variable::calculate(float value)
{
	awx_fuzzy_result* res = new awx_fuzzy_result(name);
	
	if(value >= maximum)value = maximum - 0.0001;
	else if(value <= minimum)value = minimum + 0.0001;
	
	for(int i = 0; i < sets.size();i++)
	{
		//cout << name << " " << ((value - minimum)/(maximum - minimum))*4.0 << endl;
		res->addResult(sets[i].name,sets[i].getValue( ((value - minimum)/(maximum - minimum))*4.0 ));
	}
	
	return *res;
}

bool awx_fuzzy_variable::writeToFile(FILE* file)
{
	if(file != NULL)
	{
		bool temp = true;
		int tempi = sets.size();
		awx_fwrite(&tempi , sizeof(tempi),  1 , file);
		
		for(int i = 0; i < sets.size();i++)
		{
			//cout << sets[i].name << endl;
			temp = sets[i].writeToFile(file);
			if(!temp)
			{
				return false;
			}
		}

		awx_fwrite(&minimum , sizeof(minimum),  1 , file);
		awx_fwrite(&maximum , sizeof(maximum),  1 , file);
		
		tempi = name.length();
		awx_fwrite(&tempi , sizeof(tempi),  1 , file);
		awx_fwrite(name.cString() , sizeof(char) , name.length() , file);
				
		return true;
	}
	else return false;
}

bool awx_fuzzy_variable::readFromFile(FILE* file , bool inverted)
{
	if(file != NULL)
	{	
		//cout << "load" << endl;
		
		sets.clear();
		bool temp = true;
		int tempi = 0;
		awx_fread(&tempi , sizeof(tempi),  1 , file , inverted);

		if(tempi > 100)
		{
			//cout << tempi << " " << inverted << endl;
			inverted = !inverted;
			fseek(file,SEEK_SET,0);
			awx_fread(&tempi , sizeof(tempi),  1 , file , inverted);
		}
		
		//cout << "Sets: " << tempi << endl;
		for(int i = 0; i < tempi;i++)
		{
			awx_fuzzy_set temps;
			temp = temps.readFromFile(file , inverted);
			if(!temp)
			{
				fclose(file);
				return false;
			}
			else
			{
				sets.push_back(temps);
			}
		}
		
		awx_fread(&minimum , sizeof(minimum),  1 , file , inverted);
		awx_fread(&maximum , sizeof(maximum),  1 , file , inverted);
		
		awx_fread(&tempi , sizeof(tempi),  1 , file , inverted);
		char* tempc = new char[tempi+1];
		awx_fread(tempc, sizeof(char) , tempi , file , false);
		tempc[tempi] = '\0';
		name = tempc;
		delete[] tempc;
				
		return true;
	}
	else return false;
}

bool awx_fuzzy_variable::writeToFile(char* path)
{
	FILE* file = fopen(path , "wb");
	if(writeToFile(file))
	{
		fclose(file);
		return true;
	}
	else return false;
}


bool awx_fuzzy_variable::readFromFile(char* path , bool inverted)
{
	FILE* file = fopen(path , "rb");
	if(readFromFile(file , inverted))
	{
		fclose(file);
		return true;
	}
	else return false;
}