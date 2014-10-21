#include "awx_cameraDrive.h"

extern vector<awx_trueTypeFont> ttfFonts;
extern int untertitelFontIndex;
extern int untertitelTimedFontIndex;

string floatToString(float f)
{
	stringstream stream;
	
	stream << f;
	
	return stream.str();
}

string intToString(int f)
{
	stringstream stream;
	
	stream << f;
	
	return stream.str();
}

string awxVectorToString(awx_vector3f &f)
{
	stringstream stream;
	
	stream << f.x << "/" << f.y << "/" << f.z;
	
	return stream.str();
}

cameraDriveKeyframe::cameraDriveKeyframe()
{
	untertitel="";
	timeToNextKeyframe=1.0f;
	fov=80.0f;
	speed=5;
}

cameraDrive::cameraDrive()
{
    bezeichnung = "Null";
    serialisierung = "notset";
    
    startScript="notset";
    endScript="notset";
    
    timeInCameraDrive=0;
    
    points.clear();
    
    cameraDriveKeyframe frame;
    points.push_back(frame);
	
	lastTimedUntertitel=-1; lastFrame0=-1;
}

void cameraDrive::writeBinaryToFile(FILE *file)
{
    char temp[300];
    strcpy(temp,bezeichnung.c_str());
    fwrite(temp,1,50,file);
    strcpy(temp,startScript.c_str());
    fwrite(temp,1,50,file);
    strcpy(temp,endScript.c_str());
    fwrite(temp,1,50,file);
    int n=points.size();
    awx_fwrite(&n,sizeof(int),1,file);
    
    for(int i=0;i<n;i++)
    {
        awx_fwrite(&points[i].posWorld.x,sizeof(float),1,file);
        awx_fwrite(&points[i].posWorld.y,sizeof(float),1,file);
        awx_fwrite(&points[i].posWorld.z,sizeof(float),1,file);
        
        awx_fwrite(&points[i].targetWorld.x,sizeof(float),1,file);
        awx_fwrite(&points[i].targetWorld.y,sizeof(float),1,file);
        awx_fwrite(&points[i].targetWorld.z,sizeof(float),1,file);
        
        awx_fwrite(&points[i].fov,sizeof(float),1,file),
        awx_fwrite(&points[i].timeToNextKeyframe,sizeof(float),1,file);
		awx_fwrite(&points[i].speed,sizeof(float),1,file);
        
        strcpy(temp,points[i].untertitel.c_str());
        fwrite(temp,1,300,file);
    }
	
	n = untertitel.size();
	
	awx_fwrite(&n,sizeof(int),1,file);
	
	for(int i=0;i<n;i++)
	{
		fwrite(untertitel[i].untertitel,1,300,file);
		awx_fwrite(&untertitel[i].zeit,sizeof(float),1,file);
	}
	
}

void cameraDrive::readBinaryFromFile(FILE *file)
{
    char temp[300];
    fread(temp,1,50,file);
    bezeichnung=temp;
    fread(temp,1,50,file);
    startScript=temp;
    fread(temp,1,50,file);
    endScript=temp;

    int n;
    awx_fread(&n,sizeof(int),1,file);
    
    points.clear();
    for(int i=0;i<n;i++)
    {
        cameraDriveKeyframe temp;
        
        awx_fread(&temp.posWorld.x,sizeof(float),1,file);
        awx_fread(&temp.posWorld.y,sizeof(float),1,file);
        awx_fread(&temp.posWorld.z,sizeof(float),1,file);
        
        awx_fread(&temp.targetWorld.x,sizeof(float),1,file);
        awx_fread(&temp.targetWorld.y,sizeof(float),1,file);
        awx_fread(&temp.targetWorld.z,sizeof(float),1,file);
        
        awx_fread(&temp.fov,sizeof(float),1,file);
        awx_fread(&temp.timeToNextKeyframe,sizeof(float),1,file);
		awx_fread(&temp.speed,sizeof(float),1,file);
        

        char tempi[300];
        fread(tempi,1,300,file);
        temp.untertitel=tempi;
        
        points.push_back(temp);
        
    }
	
	awx_fread(&n,sizeof(int),1,file);
	untertitelStructure tempP; 
	
	for(int i=0;i<n;i++)
	{
		fread(tempP.untertitel,1,300,file);
		awx_fread(&tempP.zeit,sizeof(float),1,file);
		untertitel.push_back(tempP);
	}
}

string cameraDrive::serialize()
{
    string ret = "WolfEngine - Kamerafahrt V1\n";
    
    ret+=string("BEZEICHNUNG = ") + bezeichnung + string("\n");
    ret+=string("STARTSCRIPT = ") + startScript + string("\n");
    ret+=string("ENDSCRIPT = ") + endScript + string("\n\n");
    
    ret+=string("POINTS = ") + intToString(points.size()) + string("\n");
    
    for(int i=0;i<points.size();i++)
    {
        ret+=awxVectorToString(points[i].posWorld) + string(",") + awxVectorToString(points[i].targetWorld) + string("\n");
    }
    
    return ret;
}

void cameraDrive::writeToFile(string filename)
{
    FILE *myFile = fopen(filename.c_str(),"w");
    
    string ser = serialize();

    
    fprintf(myFile,"%s",ser.c_str());
    
    fclose(myFile);
}

void cameraDrive::parseFromSerialisation(char* serial)
{
    char bez[50],start[50],end[50];
    int i;
    cout << "ser=" << serial << endl;
    sscanf(serial,"WolfEngine - Kamerafahrt V1\nBEZEICHNUNG = %s\nSTARTSCRIPT = %s\nENDSCRIPT = %s\nPOINTS = %i",bez,start,end,&i);
    cout << bez << "  " << start << "  " << end << "  " << i << endl;
}

void cameraDrive::updateCameraDrivePosition(awx_vector3f &camPos, awx_vector3f &camTarget, float &fov, int untertitel, int obertitel, string &script)
{
	frame0=-1;
	float frame1=-1;
    float kumTime=0;
    
    timeInCameraDrive+=1.0/float(runtime.fps);
	
    
    // Find currentFrameset
    int i;
    for(i=0;i<points.size();i++)
    {
		float gesamtzeit = points[i].timeToNextKeyframe;
		if(points[i].speed>0)
		{
			
			gesamtzeit = ((points[ (i>=points.size()-1) ? 0 : i+1].posWorld-points[i].posWorld)).getLength()/points[i].speed;
		}
        kumTime += gesamtzeit;
        
        if(kumTime>=timeInCameraDrive) 
        {
            frame0 = i;
            frame1 = (i < points.size()-1) ? i+1 : 0;
            kumTime-=gesamtzeit;
        
            break;
        }
    }
	
	// Neuer Durchlauf?
	if(frame0==-1)
	{
		frame0=0;
		if(points.size()>=1) frame1 = 1;
		else frame1=0;
		timeInCameraDrive=1.0/runtime.fps;
		kumTime=0;
		i=points.size();

		if(startScript.length()>1)
		{
			script = startScript;
		}
	}
    
	// Letzer Frame?
    if(i==points.size()-1)
    {
        frame0 = points.size()-1;
        if(points.size()>=1) frame1 = 0;

		if(endScript.length()>1)
		{
			script = endScript;
		}
    }
	
	if(lastFrame0!=int(frame0))
	{
		// Calculate new untertitels
		if(untertitel!=-1&&strcmp((char*)ttfFonts[untertitel].caption.cString(),(char*)points[int(frame0)].untertitel.c_str())!=0)
		ttfFonts[untertitel].calculate(points[int(frame0)].untertitel.c_str());
		
		lastFrame0=frame0;
	}
	
	float kumTime2=0;
	int currentTimedUntertitel=-1;
	for(int i=0;i<this->untertitel.size();i++)
	{
		kumTime2+=this->untertitel[i].zeit;
		if(kumTime2>timeInCameraDrive)
		{
			currentTimedUntertitel=i;
			break;
		}
	}
	
	if(currentTimedUntertitel!=lastTimedUntertitel&&obertitel>=0&&obertitel<ttfFonts.size())
	{		
		if(currentTimedUntertitel<0||currentTimedUntertitel>=this->untertitel.size())
		{
			ttfFonts[obertitel].calculate("");
		}
		else
		{
			if(strcmp((char*)ttfFonts[obertitel].caption.cString(),this->untertitel[currentTimedUntertitel].untertitel)!=0)
			ttfFonts[obertitel].calculate(this->untertitel[currentTimedUntertitel].untertitel);
		}
		
		lastTimedUntertitel=currentTimedUntertitel;
	}
	
	float gesamtzeit = points[frame0].timeToNextKeyframe;
	if(points[frame0].speed>0)
	{
		gesamtzeit = ((points[frame1].posWorld-points[frame0].posWorld)).getLength()/points[frame0].speed;
	}
    
    float timeInTween = timeInCameraDrive-kumTime;
	percentageInTween = timeInTween/gesamtzeit;
    
    
    camPos = points[frame0].posWorld + ((points[frame1].posWorld-points[frame0].posWorld).product(percentageInTween));
    camTarget = points[frame0].targetWorld + ((points[frame1].targetWorld-points[frame0].targetWorld).product(percentageInTween));
    
    fov = points[frame0].fov + (points[frame1].fov-points[frame0].fov) * percentageInTween;
}

void cameraDrive::rewind()
{
    timeInCameraDrive=0;
}

void cameraDrive::setToFrame(int frame)
{
}