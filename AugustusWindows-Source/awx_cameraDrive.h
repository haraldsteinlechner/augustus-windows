#ifndef AWX_CAMERADRIVE_H
#define AWX_CAMERADRIVE_H

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

#include "awx_vector.h"
#include "awx_cpp_utilities.h"
#include "globals_extern.h"


string floatToString(float f);

string intToString(int f);

string awxVectorToString(awx_vector3f &f);

class untertitelStructure
{
	public:
	
	char untertitel[300];
	float zeit;
};

class cameraDriveKeyframe
{
	public:
		awx_vector3f posWorld,targetWorld;
		float timeToNextKeyframe;
		string untertitel;
		float fov;
		
		float speed;
		
		cameraDriveKeyframe();
};

class cameraDrive
{
	public:
	
		vector<cameraDriveKeyframe> points;
		vector<untertitelStructure> untertitel;
		
		string bezeichnung;
		string serialisierung;
		string startScript,endScript;
		
		float timeInCameraDrive;
		float frame0,percentageInTween;
		int lastFrame0;
		int lastTimedUntertitel;

		
		cameraDrive();
		
		void writeBinaryToFile(FILE *file);
		
		void readBinaryFromFile(FILE *file);
		
		string serialize();
		
		void writeToFile(string filename);
		
		void parseFromSerialisation(char* serial);
		
		void updateCameraDrivePosition(awx_vector3f &camPos, awx_vector3f &camTarget, float &fov, int untertitel, int obertitel, string &script);
		
		void rewind();
		
		void setToFrame(int frame);
};
#endif