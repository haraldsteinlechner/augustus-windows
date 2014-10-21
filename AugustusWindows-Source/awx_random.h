#ifndef AWX_RANDOM
#define AWX_RANDOM

#ifndef RANDOM_FLOAT
#define RANDOM_FLOAT (((float)rand())/RAND_MAX)
#endif

#include <ctime>

class awxRandom
{
	public:
	
	vector<float> numbers;
	int numberOfNumbers;
	int currentNumber;
	
	awxRandom()
	{
		numberOfNumbers = 8000;
		currentNumber = -1;
	}
	
	void initialize()
	{
		srand(time(NULL));
		for(int i=0;i<numberOfNumbers;i++)
		{
			//srand(time(NULL));
			numbers.push_back(RANDOM_FLOAT);
		}
	}
	
	
	float next(float min, float max)
	{
		currentNumber++;
		if(currentNumber>=numberOfNumbers) currentNumber=0;
		return min+numbers[currentNumber]*(max-min);
	}
	
	float next()
	{
		currentNumber++;
		if(currentNumber>=numberOfNumbers) currentNumber=0;
		return numbers[currentNumber];
	}
};

#endif