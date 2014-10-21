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

#ifndef GLOBALS
#ifndef GLOBALS_EXTERN
#define GLOBALS

char awx_temp_string[120];

// Gesammter GeometrieListen z√§ehler
int currentID=1;


// Pfade
char partPath[] = "Data/APX/";
char assemblePath[] = "Data/AAX/";
char texturePath[] = "Data/ATX/";
char textureSuffix[] = ".tga";
char interfacePath[] = "Data/interface/";

char modelSuffix[] = ".apx";
char assembleSuffix[] = ".aax";

GLuint textur_liste[3000];

Project project;
_runtime runtime;
_camera camera;
_awx_utilities awx_utilities;

bool visibitilityCheckInNextFrame = false;

awx_shader terrainshader, shadowshader, throughputshader;
GLint AWXExtrudorLocation,AWXAlphaAttrib,AWXBegehbarkeitsAttrib,AWXFogOfWarAttrib;
GLint AWXTexture0, AWXTexture1,AWXTexture3,AWXTexture2;

int pressedID;
int rollOverID;

int rendered_objects=0;
int iAmInInterface;

double mvmatrix[16];
double wx,wy,wz;
float window_z;

bool hitTestBool;

int kartentextur=0;

int selectedID;
int selectedQuadID=0;
GLubyte colorOverTerrain[3];
GLubyte colorOverInterface[3];
float bla;

bool hastToCheckVisibility=false;	// Bei allen einheiten wird physik gerechnet und falls sie ihre position Šndern wird diese Variable auf true gesetzt und daraufhin wird sichtbarkeitstest durchgefŸfhrt

int nahrungsTextID,steinTextID,goldTextID,holzTextID;
int aggressivTextID, defensivTextID, passivTextID;

int controlIndex = 0;

int rohstoffwarnungTexID;
int rohstoffwarnungMeldungID;
int steuerFensterID;

bool startCounter=false;


// Threads
SDL_Thread *kiTimer;
SDL_Thread *clockTimer;
SDL_Thread *particleSystemTimer;
Uint32 clockTimerRate = 1000;
Uint32 kiClockRate = 500;
Uint32 particleSystemRate = 50;
Uint32 kiGegnerDenkclockrate = 4000;

bool killThreadsInNextRate=false;


// Sonne
awx_vector3f sunPosition = awx_vector3f(10.0,10.0,-10.0);
awx_vector3f sunDirection = sunPosition.normalize();

int progressTexID;

awx_time startTime;
awx_time mainTime;
awx_time playedTime;
float zeitfaktor = 60*10;

float einkommenssteuer = 0.3;
vector <awx_string> produkte;

int pressedTextFieldID;
int buergerButtonID = -1;
int buergerHereButtonID = -1;

vector <awx_building> loadedBuildings;
vector <awx_unit> loadedUnits;

int getLoadedUnitIndex(awx_string name)
{
	for(int i = 0; i < loadedUnits.size();i++)
	{
		if(loadedUnits[i].workingdirectory.lastPathComponent() == name)
		{
			return i;
		}
	}
	return -1;
}

int getLoadedIndex(awx_string name)
{
	for(int i = 0; i < loadedBuildings.size();i++)
	{
		if(loadedBuildings[i].workingdirectory.lastPathComponent() == name)
		{
			return i;
		}
	}
	return -1;
}


#endif
#endif
