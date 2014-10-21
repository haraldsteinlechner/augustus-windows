#include "awx_fuzzy_set.h"

void awx_fuzzy_set::sort()
{
	bool sorted = false;
	while(!sorted)
	{
		sorted = true;
		for(int i = 0; i < points.size()-1;i++)
		{
			if(points[i].x > points[i+1].x)
			{
				awx_vector2f temp = points[i+1];
				points[i+1] = points[i];
				points[i] = temp;
				sorted = false;
			}
		}
	}
}

awx_fuzzy_set::awx_fuzzy_set()
{
	name = "newset";
	marked = false;
	maxValue = 0;
}

void awx_fuzzy_set::addPoint(awx_vector2f point)
{
	 points.push_back(point);
	 sort();
}

awx_vector2f &awx_fuzzy_set::getPoint(int index)
{
	if(index <= 0)index = 0;
	if(index > points.size()-1)index = points.size()-1;
	
	return points[index];
}
		
bool awx_fuzzy_set::containsValue(float value)
{
	sort();
	return (points[0].x <= value && points[points.size()-1].x >= value);
}

awx_string &awx_fuzzy_set::getName()
{
	return name;
}

void awx_fuzzy_set::setName(awx_string &name)
{
	this->name = name;
}

float awx_fuzzy_set::getValue(float x)
{
	if(containsValue(x))
	{
		int left = -1;
		
		for(int i = 0; i < points.size();i++)
		{
			if(points[i].x >= x)
			{
				left = i-1;
				break;
			} 
		}
		if(left > -1)
		{
			int myright = left+1;
		
			///cout << left << " " << myright << endl;
			//cout << myright << " <-> " << points.size() << endl;
		
			if(points[left].x == points[myright].x)return (points[left].x > points[myright].x ? points[left].x : points[myright].x);
		
			float k = (points[myright].y - points[left].y)/(points[myright].x - points[left].x);
			//cout << "POINTS: " << points[left].x << "," << points[left].y << "  " << points[myright].x << "," << points[myright].y << endl;
		
			float d = points[left].y - points[left].x*k;
		
			//cout << k << "*"<< x << " + " << d << "=" << k*x + d << endl;
		
			return k*x + d;
		}
		else return 0;
	}
	else
	{
		return 0;
	}
}

bool awx_fuzzy_set::writeToFile(FILE* file)
{
	if(file != NULL)
	{
		int tempi = points.size();
		//cout << "TEMPI: " << tempi << " " << points.size() << endl;
		awx_fwrite(&tempi,sizeof(tempi),1,file);
		
		
		for(int i = 0; i < points.size();i++)
		{
			//cout << points[i];
			awx_fwrite(&points[i].x , sizeof(points[i].x) , 1 , file);
			awx_fwrite(&points[i].y , sizeof(points[i].y) , 1 , file);
		}
		//cout << "#" << name << "#" << endl;
		writeString(name,file);
		
		return true;
	}
	else return false;
}


bool awx_fuzzy_set::readFromFile(FILE* file , bool inverted)
{
	if(file != NULL)
	{
		points.clear();
		int tempi;
		awx_fread(&tempi,sizeof(tempi),1,file , inverted);
		
		//cout <<"points: " <<  tempi << endl;
		for(int i = 0; i < tempi;i++)
		{
			float x , y;
			awx_fread(&x , sizeof(x) , 1 , file , inverted);
			awx_fread(&y , sizeof(x) , 1 , file , inverted);
			//cout << x << "," << y << endl;
			points.push_back(awx_vector2f(x,y));
		}

		name = readString(file , inverted);
		return true;
	}
	else return false;	
}
