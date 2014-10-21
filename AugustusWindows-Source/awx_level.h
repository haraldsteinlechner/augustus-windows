///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   © Alphaworx Corporation //  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////

#ifndef AWX_LEVEL_H
#define AWX_LEVEL_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_vector.h"

#include "awx_project.h"
#include "globals_extern.h"

#include "awx_blob.h"

#include "awx_matrix.h"
#include "awx_cpp_utilities.h"

#include "awx_apx.h"
#include "awx_aax.h"
#include "awx_level_part.h"
#include "awx_terrain.h"


typedef struct
{
	char data[50];
} names;

typedef struct
{
	vector<int> list;
} renderList;

class gamePlayPart
{
	public:
	
		float x,y,z;
		string name;
		string beschreibung;
		string awxScript;
		bool setPos;
		
	gamePlayPart()
	{
		x=0; y=0; z=0;
		name = "";
		beschreibung = "";
		awxScript = "";
		setPos=false;
	}
};

class level: public aax
{
	public:
	
	awx_string workingdirectory;
	
	vector<cameraDrive> cameraDrives;
	
	awx_terrain terrain;
	
	// Levelobjekte
	levelpart *data;
	gamePlayPart *gameplayParts;
	int numberOfGameplayParts;
	
	GLuint fowList;
	
	renderList *optimizedRenderList;
	int objectPoolSize;
	
	GLuint waterBumpTexture;
	GLuint skyTexture;
	awx_shader *watershader;
	
	levelpart *apxPool;
	bool useCoolWater;
	
	int untertitelTextField0,untertitelTextField1;
	
	int feld;
	
	int render();
	void setPositionfv(awx_vector3f vec);
	void setPositionf(float x, float y, float z);
	int renderLevelObjects();
	
	int load_level(const char* filename);
	void renderBlobsAll();
	void checkVisibilityAll();
	int renderAllLevelObjects();
	void createFOW();
	void renderFOWBottom();
	void renderLevelObjectsOptimized(bool considerFOW);
	void openCameraFahrten(string path);
	void setUpMatriceWithCameraDrive(int i, string &script);
	int findCameraDriveByIdentifier(string identifier);
	
	void getStartpointForPlayer(string name, awx_vector3f &pos);

	void renderLevelObjectsForReflection();
	
	level()
	{
		aax::aax();
		
		useCoolWater=true;
		feld=-1;
		
		untertitelTextField0=-1;
		untertitelTextField1=-1;
	}
	
	void renderBlobsAllUnperformant();
	
	void addLevelPartFromBuilding(string name);
	
	void initUnteritelTexte()
	{
		// Create untertitelfonts
		awx_trueTypeFont tempFont;
		tempFont.setFormat(false,false,false,255,255,255,"Data/fonts/arial.ttf",14,0.7,500,1,true);
		tempFont.init();
		tempFont.calculate("\n#b Nahrung");
		tempFont.setAspectCorrection(false);
		untertitelTextField0 = (int)ttfFonts.size();
		ttfFonts.push_back(tempFont);
		
		
		awx_trueTypeFont tempFont2;
		tempFont2.setFormat(false,false,false,255,255,255,"Data/fonts/arial.ttf",14,0.7,500,1,true);
		tempFont2.init();
		tempFont2.calculate("\n#b Nahrung");
		tempFont2.setAspectCorrection(false);
		untertitelTextField1 = (int)ttfFonts.size();
		ttfFonts.push_back(tempFont2);
		cout << "Untertiteltexte = " << untertitelTextField0 << "  " << untertitelTextField1 << endl;
	}
};

#endif

level* currentLevel=NULL;

int level::load_level(const char* filename  )
{
	FILE* input = fopen(filename,"rb");
	if(input==NULL)
	{
		return -1;
	}
	
	number_of_objects = 0;
	
	workingdirectory = filename;
	workingdirectory = workingdirectory.stringByDeletingLastPathComponent();
	
	char *header = new char[24];
	char *testheader = new char[20];
	fread(header,1,24,input);
	for(int i=0;i<16;i++)
	{
		testheader[i]=header[i];
	}
	testheader[16]='\0';
	if(strcmp(testheader,"AWXLEditorAAX-V1")==0)
	{
		cout << "Neues Levelformat" << endl;
		delete header;
		
		terrain.loadTerrainNeu((workingdirectory + "/terrain").cString());
		terrain.createVBO();

		objectPoolSize=0;
		awx_fread(&objectPoolSize,sizeof(int),1,input);
		apxPool = new levelpart[objectPoolSize+1];
		int *apxPoolIdentifier = new int[objectPoolSize+1];
		optimizedRenderList = new renderList[objectPoolSize+1];
		
		int l=0;
		char *name;
		int realID;
		awx_string pf;
		for(int i=0;i<objectPoolSize+1;i++)
		{
			if(i>=objectPoolSize)
			{
				pf = "Data/buildings/feld/part0.apx";
				feld = i;
			}
			else
			{
				awx_fread(&l,sizeof(int),1,input);
				name = new char[l+1];
				fread(name,1,l,input);
				name[l]='\0';
				pf = name;
				pf = pf.lastPathComponent();
				pf = workingdirectory + "/" + pf + "/part0.apx";
				
				awx_fread(&realID,sizeof(int),1,input);
				apxPoolIdentifier[i] = realID;
				delete name;
			}
			

			apxPool[i].load_apx(pf.cString());
			if(apxPool[i].useWaterShading) apxPool[i].fillVRAMWithBump();
			else apxPool[i].fillVRAM();

			
			apxPool[i].useStates();
			apxPool[i].renderBlobTexture(awx_vector3f(0.6,7.4,0),awx_vector3f(-0.5,0,0));
			
			if(i>=objectPoolSize)
			{
					// Spezielle no lighting states erstellen
				GLuint tempStateList = glGenLists(1);
				glNewList(tempStateList,GL_COMPILE);
				glDisable(GL_LIGHTING);
				glCallList(apxPool[i].IDs[0]);
				glEnable(GL_LIGHTING);
				glEndList();
				
				//glDeleteLists(apxPool[i].IDs[0],1);
				apxPool[i].IDs[0] = tempStateList;
			}


		}

		
		int anzahl_LevelParts=0;
		
		awx_fread(&anzahl_LevelParts,sizeof(int),1,input);
		
		
		// statische anzahl von felder (später entstehende levelparts) da realloc nur auf malloc funktioniert und malloc bei konstrukoren probleme macht.
		
		const int anzahlDerFelder = 20;
		data = new levelpart[anzahl_LevelParts+anzahlDerFelder];
		number_of_objects = anzahl_LevelParts;

		
		float x,y,z,rx,ry,rz,scalx,scaly,scalz;
		int ID,schattenErlauben,sichtbarkeitsTestErlauben;
		for(int i=0;i<anzahl_LevelParts;i++)
		{
			awx_fread(&x,sizeof(float),1,input);
			awx_fread(&y,sizeof(float),1,input);
			awx_fread(&z,sizeof(float),1,input);
			
			awx_fread(&rx,sizeof(float),1,input);
			awx_fread(&ry,sizeof(float),1,input);
			awx_fread(&rz,sizeof(float),1,input);
			
			awx_fread(&scalx,sizeof(float),1,input);
			awx_fread(&scaly,sizeof(float),1,input);
			awx_fread(&scalz,sizeof(float),1,input);
			
			awx_fread(&ID,sizeof(int),1,input);
			awx_fread(&schattenErlauben,sizeof(int),1,input);
			awx_fread(&sichtbarkeitsTestErlauben,sizeof(int),1,input);
			
			
			awx_fread(&data[i].capacity,sizeof(data[i].capacity),1,input);
			awx_fread(&data[i].platzbedarf,sizeof(data[i].platzbedarf),1,input);
			awx_fread(&data[i].abbaufaktor,sizeof(data[i].abbaufaktor),1,input);
			awx_fread(&data[i].resourceID,sizeof(data[i].resourceID),1,input);
			data[i].resourceID--;
			
			int realID=-1;
			for(realID=0;realID<objectPoolSize;realID++)
			{
				if(ID==apxPoolIdentifier[realID]) break;
			}
			
			data[i].IDs = new GLuint[apxPool[realID].anzahl_frames];
			for(int u=0;u<apxPool[realID].anzahl_frames;u++)
			{
				data[i].IDs[u] = apxPool[realID].IDs[u];
			}
			
			data[i].blobsizex = apxPool[realID].xmax-apxPool[realID].xmin;
			data[i].blobsizey = apxPool[realID].zmax-apxPool[realID].zmin;
			data[i].transOffset=0.3;
			
			optimizedRenderList[realID].list.push_back(i);
			
			data[i].x = x;
			data[i].y = y;
			data[i].z = z;
			data[i].simplerotation = //ry
			data[i].simple_rot_x=0.0;
			data[i].simple_rot_y=1.0;
			data[i].simple_rot_z=0.0;
			data[i].shadowflag=1;
			data[i].blobtexturID=apxPool[realID].blobtexturID;
			data[i].xmin = apxPool[realID].xmin;
			data[i].xmax = apxPool[realID].xmax;
			data[i].ymin = apxPool[realID].ymin;
			data[i].ymax = apxPool[realID].ymax;
			data[i].zmin = apxPool[realID].zmin;
			data[i].zmax = apxPool[realID].zmax;
			data[i].useWaterShading = apxPool[realID].useWaterShading;
			data[i].texture = apxPool[realID].texture;
			data[i].blobTexture = apxPool[realID].blobTexture;
			
			if(schattenErlauben)
				data[i].shadowflag=true;
			else data[i].shadowflag=false;
            data[i].sichttestflag=sichtbarkeitsTestErlauben;
			

			
			bool res=false;

			data[i].hitTriangle = currentLevel->terrain.hit(awx_vector3f(x,y,z),y,res,data[i].sektorX,data[i].sektorZ);

			if(data[i].hitTriangle!=-1) data[i].myHitTriangle=&currentLevel->terrain.sektoren[data[i].sektorX][data[i].sektorZ].triangles[data[i].hitTriangle];
		}
		
		//delete apxPool;
		delete apxPoolIdentifier;
		
		
		// Read gameplayParts
		numberOfGameplayParts=-1;
		awx_fread(&numberOfGameplayParts,sizeof(int),1,input);
		
		gameplayParts = new gamePlayPart[numberOfGameplayParts];
		
		for(int i=0;i<numberOfGameplayParts;i++)
		{	
			awx_fread(&gameplayParts[i].x,sizeof(float),1,input);
			awx_fread(&gameplayParts[i].y,sizeof(float),1,input);
			awx_fread(&gameplayParts[i].z,sizeof(float),1,input);
			
			int l = -1;
			
			awx_fread(&l,sizeof(int),1,input);
			char *tempText = new char[l+1];
			fread(tempText,sizeof(char),l,input);
			gameplayParts[i].name = tempText;
			delete[] tempText;
			
			awx_fread(&l,sizeof(int),1,input);
			tempText = new char[l+1];
			fread(tempText,sizeof(char),l,input);
			gameplayParts[i].beschreibung = tempText;
			delete[] tempText;
			
			awx_fread(&l,sizeof(int),1,input);
			tempText = new char[l+1];
			fread(tempText,sizeof(char),l,input);
			gameplayParts[i].awxScript = tempText;
			delete[] tempText;
			
			cout << gameplayParts[i].name << "###" << gameplayParts[i].beschreibung << gameplayParts[i].x << endl;
		}
		
		
		createFOW();
		
		#ifdef GCCDebugCompile
		#warning gehackte lösung für wasser entfernen
		#endif
		cout << "loading waterbump" << endl;
		waterBumpTexture = load_tgaAndReturnObject("Data/textures/noise.tga",true,true);
		skyTexture = load_tgaAndReturnObject("Data/textures/sky.tga",true,true);
		
		
		openCameraFahrten((workingdirectory+"/cameraFahrten").cString());
		
		return 0;
	} 
	else
	{
	/*
		delete header;
		cout << "sseeek" << endl;
		fseek(input,0,SEEK_SET);
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
        }*/
	}
	
	return 0;
}

void level::openCameraFahrten(string path)
{
		FILE *file = fopen(path.c_str(),"rb");
		if(file==NULL) 
		{
			cout << "Camerafahrt: " << path << " not found" << endl;
			return;
		}
		
		int n=0;
		awx_fread(&n,sizeof(int),1,file);
		
		cameraDrives.clear();
		
		for(int i=0;i<n;i++)
		{
			cameraDrive temp;
			
			temp.readBinaryFromFile(file);
			
			cout << "loaded Camerafahrt: " << temp.bezeichnung << endl;
			cameraDrives.push_back(temp);
		}
		
		fclose(file);
		
		
		cout << "Camerafahrten: " << path << " loaded." << endl;
}

void level::setUpMatriceWithCameraDrive(int i, string &script)
{
	if(i>=0&&i<cameraDrives.size())
	{
		awx_vector3f eye;
		awx_vector3f tar;
		
		float fov;
		
		cameraDrives[i].updateCameraDrivePosition(eye,tar,fov,untertitelTextField0,untertitelTextField1,script);
		

		if(untertitelTextField0!=-1) 
		{
			float w0=ttfFonts[untertitelTextField0].getLongestLine()*0.5;
			ttfFonts[untertitelTextField0].registerRender(project.width*0.5-w0,project.height*0.1);
		}
		if(untertitelTextField1!=-1) 
		{
			float w1=ttfFonts[untertitelTextField1].getLongestLine()*0.5;
			ttfFonts[untertitelTextField1].registerRender(project.width*0.5-w1,project.height*0.9);
		}
			
				/*	if(untertitelTextField0!=-1) 
			ttfFonts[untertitelTextField0].registerRender(project.width*0.5-ttfFonts[untertitelTextField0].getLineWidth(0)*0.5,project.height*0.9);
		if(untertitelTextField1!=-1) 
			ttfFonts[untertitelTextField1].registerRender(project.width*0.5-ttfFonts[untertitelTextField1].getLineWidth(0)*0.5,project.height*0.1);*/
		
		gluLookAt(eye.x,eye.y,eye.z,
				  tar.x,tar.y,tar.z,
				  0,1,0);
				  
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fov,project.aspect,2.0f,8000.0f);
		
		glMatrixMode(GL_MODELVIEW);
				  
	}
}

int level::render()
{
	terrain.render();
	//terrain.renderRect();
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

void level::renderLevelObjectsForReflection()
{
	for(int i=1;i<objectPoolSize+1;i++)
	{
		GLuint object = apxPool[i].IDs[0];
		
		if(glIsList(apxPool[i].stateChangeList))
		glCallList(apxPool[i].stateChangeList);
	
		for(int u=0;u<optimizedRenderList[i].list.size();u++)
		{ 
			
			int dataIndex = optimizedRenderList[i].list[u];
			
			glPushMatrix();
			
			glTranslatef(data[dataIndex].x,data[dataIndex].y,data[dataIndex].z);
			glRotatef(data[dataIndex].simple_rot_y,0.0,1.0,0.0);
			
					
			
			glCallList(object);
	
			glPopMatrix();

		}
	}
}

void level::renderLevelObjectsOptimized(bool considerFOW)
{
	for(int i=0;i<objectPoolSize+1;i++)
	{
		GLuint object = apxPool[i].IDs[0];
		
		if(glIsList(apxPool[i].stateChangeList))
		glCallList(apxPool[i].stateChangeList);
	
		for(int u=0;u<optimizedRenderList[i].list.size();u++)
		{ 
			if(data[optimizedRenderList[i].list[u]].sichttestflag && considerFOW && !data[optimizedRenderList[i].list[u]].getVisibility()) continue;
			
			int dataIndex = optimizedRenderList[i].list[u];
			
			glPushMatrix();
			
			glTranslatef(data[dataIndex].x,data[dataIndex].y,data[dataIndex].z);
			glRotatef(data[dataIndex].simple_rot_y,0.0,1.0,0.0);
			
					
			if(data[u].capacity<=0&&data[u].resourceID!=-1) 1;
				//glCallList(data[optimizedRenderList[i].list[u]].IDs[0]+1); Renderbaumstumpf
			else if(data[dataIndex].useWaterShading)
			{
				if(useCoolWater) data[dataIndex].renderWater(waterBumpTexture,skyTexture,watershader);
				else 
				{
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					glEnable(GL_BLEND);
					glBlendFunc(GL_DST_COLOR,GL_SRC_ALPHA);
					glBindTexture(GL_TEXTURE_2D,data[dataIndex].texture);
					glCallList(data[dataIndex].IDs[0]);
					glPopAttrib();
				}
			}
			else
			{
				glCallList(object);
			}
			
			glPopMatrix();
			
			rendered_objects++;
		}
	}
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
		if(data[u].myHitTriangle==NULL)continue;
		if(data[u].shadowflag==1&&data[u].visible&&data[u].current_frame==0&&data[u].myHitTriangle->triangleSeen)
		//data[u].renderBlobs(awx_vector3f(data[u].modelview[12],data[u].modelview[13],data[u].modelview[14]),data[u].blobtexturID);
		//cout << data[u].blobtexturID << endl;
		data[u].renderBlobWithTextureObject(data[u].winkel_y,awx_vector3f(data[u].x,data[u].y,data[u].z),data[u].blobTexture);
	}
}

void level::renderBlobsAllUnperformant()
{
	for(u=0;u<number_of_objects;u++)
	{
		if(data[u].shadowflag!=1) continue;
		data[u].renderBlobWithTextureObject(data[u].winkel_y,awx_vector3f(data[u].x,data[u].y,data[u].z),data[u].blobTexture);
	}
}

void level::checkVisibilityAll()
{
	for(u=0;u<number_of_objects;u++)
	{
		data[u].checkVisibility();
	}
	
	terrain.cull();
}

void level::createFOW()
{
	return;

	load_tga("Data/textures/fow.tga",currentID, true, true);
	
	float patternX = 20;
	float patternY = (terrain.v/terrain.u)*patternX;
	
	float yP = terrain.ymin-10;
	float sizeFactor=2.0;
	
	fowList = glGenLists(1);
	glNewList(fowList,GL_COMPILE);
	
	glBindTexture(GL_TEXTURE_2D,textur_liste[currentID]);
	
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	
		glBegin(GL_QUADS);
			glTexCoord2f(0.0,0.0);
				glVertex4f(terrain.xmin*sizeFactor,yP,terrain.zmin*sizeFactor,1.0);
			glTexCoord2f(1.0*patternX,0.0);
				glVertex4f(terrain.xmax*sizeFactor,yP,terrain.zmin*sizeFactor,1.0);
			glTexCoord2f(1.0*patternX,1.0*patternY);
				glVertex4f(terrain.xmax*sizeFactor,yP,terrain.zmax*sizeFactor,1.0);
			glTexCoord2f(0.0,1.0*patternY);
				glVertex4f(terrain.xmin*sizeFactor,yP,terrain.zmax*sizeFactor,1.0);
		glEnd();
	glEndList();
	
	
	currentID++;
}

void level::renderFOWBottom()
{	
	glCallList(fowList);
}

int level::findCameraDriveByIdentifier(string identifier)
{
	for(int i=0;i<cameraDrives.size();i++)
	{
		if(cameraDrives[i].bezeichnung==identifier)
		{
			return i;
		}
	}
	
	cout << "ERROR: Cameradrive mit identifier:" << identifier << " nicht gefunden." << endl;
	
	return -1;
}

void level::addLevelPartFromBuilding(string name)
{
	// Realloc funktioniert nicht. siehe warning bei load method
	/*

	number_of_objects++;
	
	data = (levelpart*) realloc(data,number_of_objects * sizeof(levelpart));
	
	if(data==NULL)
	{
		cout << "realloc of new levelpart failed" << endl;
		exit(-1);
	}*/
	
	//data[number_of_objects-1].reinit(data[number_of_objects-2]);
	
        
        data[number_of_objects].capacity = apxPool[feld].capacity;
        data[number_of_objects].platzbedarf = apxPool[feld].platzbedarf;
        data[number_of_objects].abbaufaktor = apxPool[feld].abbaufaktor;
        data[number_of_objects].resourceID = apxPool[feld].resourceID-1;
		
		cout <<  data[number_of_objects].capacity << "  " << data[number_of_objects].platzbedarf << "   " <<  data[number_of_objects].abbaufaktor << "  " <<  data[number_of_objects].resourceID << endl;
	
        
		int realID = feld;
        data[number_of_objects].IDs = new GLuint[apxPool[realID].anzahl_frames];
        for(int u=0;u<apxPool[realID].anzahl_frames;u++)
        {
            data[number_of_objects].IDs[u] = apxPool[realID].IDs[u];
        }
        
        data[number_of_objects].blobsizex = apxPool[realID].xmax-apxPool[realID].xmin;
        data[number_of_objects].blobsizey = apxPool[realID].zmax-apxPool[realID].zmin;
        data[number_of_objects].transOffset=0.3;
        
        optimizedRenderList[realID].list.push_back(number_of_objects);
        
        data[number_of_objects].x = wx;
        data[number_of_objects].y = wy;
        data[number_of_objects].z = wz;
		float x = data[number_of_objects].x;
		float y = data[number_of_objects].y;
		float z = data[number_of_objects].z;
        data[number_of_objects].simple_rot_x=0.0;
        data[number_of_objects].simple_rot_y=1.0;
        data[number_of_objects].simple_rot_z=0.0;
        data[number_of_objects].shadowflag=1;
        data[number_of_objects].blobtexturID=apxPool[realID].blobtexturID;
        data[number_of_objects].xmin = apxPool[realID].xmin;
        data[number_of_objects].xmax = apxPool[realID].xmax;
        data[number_of_objects].ymin = apxPool[realID].ymin;
        data[number_of_objects].ymax = apxPool[realID].ymax;
        data[number_of_objects].zmin = apxPool[realID].zmin;
        data[number_of_objects].zmax = apxPool[realID].zmax;
        data[number_of_objects].useWaterShading = false;
        data[number_of_objects].texture = apxPool[realID].texture;
        data[number_of_objects].blobTexture = apxPool[realID].blobTexture;
        
        //if(schattenErlauben)
            data[number_of_objects].shadowflag=false;
        //else data[number_of_objects].shadowflag=false;
        data[number_of_objects].sichttestflag=true;
        
        bool res=false;

        data[number_of_objects].hitTriangle = currentLevel->terrain.hit(awx_vector3f(x,y,z),y,res,data[number_of_objects].sektorX,data[number_of_objects].sektorZ);
        if(data[number_of_objects].hitTriangle!=-1) data[number_of_objects].myHitTriangle=&currentLevel->terrain.sektoren[data[number_of_objects].sektorX][data[number_of_objects].sektorZ].triangles[data[number_of_objects].hitTriangle];
		else cout << "fehler bei hit" << endl;
		
		
	
	number_of_objects++;
	
	/*
	data[0].y=wy;
	data[0].x=wx;
	data[0].z=wz;
	cout << wx << endl;*/
	//optimizedRenderList[0].list.push_back(number_of_objects-1);
	
	
}

void level::getStartpointForPlayer(string name, awx_vector3f &pos)
{
	for(int i=0;i<numberOfGameplayParts;i++)
	{
		if(gameplayParts[i].name==name)
		{
			pos.set(gameplayParts[i].x,gameplayParts[i].y,gameplayParts[i].z);
			return;
		}
	}
	cout << "ERROR: Startpunkt für: " << name << " in gameplayParts nicht gefunden." << endl;
}