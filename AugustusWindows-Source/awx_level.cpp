#ifdef level

#include "awx_level.h"


int level::load_level(const char* filename)
{
	int shadowflag;
	int sichttestflag; 

	FILE* input = fopen(filename,"rb");
	if(input==NULL)
	{
		return -1;
	}
	
	number_of_objects = 0;
	
	workingdirectory = filename;
	workingdirectory = workingdirectory.stringByDeletingLastPathComponent();
	
	unsigned short version;
	awx_fread(&version,sizeof(version),1,input);
	
	int len;
	char name[50];
	
	data = new levelpart[1000]; // BUG
	vector<names> usedNames;
	
	usedNames.clear();
	
	awx_string temp = workingdirectory + "/terrain.terrain";
	terrain.loadTerrain(temp.cString());
	terrain.fillVRAM();
	
	while(!feof(input))
	{
		strcpy(name,"");
		len=0;
		
		awx_fread(&len,sizeof(len),1,input);
		if(len==0) break; // Dateiende erreicht
		
		fread(name,sizeof(char),len,input);
		fread(modelview,sizeof(float),16,input);
		awx_fread(&shadowflag,sizeof(shadowflag),1,input);
		awx_fread(&sichttestflag,sizeof(sichttestflag),1,input);

		if(getCPUArchitecture()==AWXx86)
			for(int u=0;u<16;u++) reverseByteOrder(&modelview[u],sizeof(float),1);
		
		for(int i=0;i<16;i++) data[number_of_objects].modelview[i] = modelview[i];
		
		awx_vector3f point(modelview[12] , modelview[13] , modelview[14]);
				
		name[len] = '\0';
		
		bool already=false;
		int firstData = 0;
		
		for(int i=0;i<usedNames.size();i++)
		{
			if(strcmp(usedNames[i].data,name)==0)
			{
				already = true;
				firstData = i;
				break;
			}
		}
		
		names tempname;
		strcpy(tempname.data,name);
		usedNames.push_back(tempname);
		
		awx_string temp = workingdirectory + "/" + name + ".apx";
		
		if(!already)
		{
			data[number_of_objects].load_apx(temp.cString());
			data[number_of_objects].fillVRAM(awx_vector3f(0,0,0),false,0);
			data[number_of_objects].shadowflag = shadowflag;
			data[number_of_objects].sichttestflag = sichttestflag;
			
			data[number_of_objects].renderBlobTexture(awx_vector3f(0.6,7.4,0),awx_vector3f(-0.5,0,0));
			data[number_of_objects].blobtexturID = currentID-1;
		}
		
		else
		{
			data[number_of_objects].IDs = new int[data[firstData].anzahl_frames];
			data[number_of_objects].blobtexturID = data[firstData].blobtexturID;
			data[number_of_objects].xmin = data[firstData].xmin;
			data[number_of_objects].xmax = data[firstData].xmax;
			data[number_of_objects].zmin = data[firstData].zmin;
			data[number_of_objects].zmax = data[firstData].zmax;
			data[number_of_objects].capacity = data[firstData].capacity;
			data[number_of_objects].resourceID = data[firstData].resourceID;
			data[number_of_objects].platzbedarf = data[firstData].platzbedarf;
			data[number_of_objects].abbaufaktor = data[firstData].abbaufaktor;
			for(int i=0;i<data[firstData].anzahl_frames;i++)
			{
				data[number_of_objects].IDs[i] = data[firstData].IDs[i];
				data[number_of_objects].shadowflag = shadowflag;
				data[number_of_objects].sichttestflag = sichttestflag;
				
			}
		}
		
		number_of_objects++;
	}
	
	return 0;
}


int level::render()
{
	terrain.render();
	
	return 0;
}

int level::renderLevelObjects()
{
	for(u=0;u<number_of_objects;u++)
	{
		data[u].render();
	}
	return 0;
}

int level::renderAllLevelObjects()
{
	for(u=0;u<number_of_objects;u++)
	{
		data[u].renderWithoutTest();
	}
	
	return 0;
}

void level::renderBlobsAll()
{
	for(u=0;u<number_of_objects;u++)
	{
		if(data[u].shadowflag==1&&data[u].visible&&data[u].current_frame==0)
		//data[u].renderBlobs(awx_vector3f(data[u].modelview[12],data[u].modelview[13],data[u].modelview[14]),data[u].blobtexturID);
		data[u].renderBlobs(data[u].modelview,data[u].blobtexturID);
	}
}

void level::checkVisibilityAll()
{
	for(u=0;u<number_of_objects;u++)
	{
		data[u].checkVisibility();
	}
}

#endif