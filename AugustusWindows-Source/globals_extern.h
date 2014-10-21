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

#ifndef GLOBALS_EXTERN_H
#ifndef GLOBALS
#define GLOBALS_EXTERN_H


#include "awx_constants.h"
#include "awx_trueTypeFont.h"
#include "awx_time.h"
#include "awx_inputTextField.h"
class awx_building;
class awx_unit;

extern char awx_temp_string[120];

// Gesammter GeometrieListen z√§ehler
extern int currentID;


// Pfade
extern char partPath[];
extern char assemblePath[];
extern char texturePath[];
extern char textureSuffix[];
extern char interfacePath[];

extern char modelSuffix[];
extern char assembleSuffix[];


extern Project project;

class _camera;
extern _camera camera;
extern _runtime runtime;
extern _awx_utilities awx_utilities;

extern GLuint textur_liste[];

extern bool visibitilityCheckInNextFrame;


//awx_shader terrainshader, shadowshader, throughputshader;
extern GLint AWXExtrudorLocation,AWXBegehbarkeitsAttrib,AWXAlphaAttrib,AWXBegehbarkeitsAttrib,AWXFogOfWarAttrib,AWXBegehbarkeitsAttrib;
extern GLint AWXTexture0, AWXTexture1,AWXTexture3,AWXTexture2;
extern int pressedID;
extern int rollOverID;

extern int rendered_objects;
extern int iAmInInterface;

extern double mvmatrix[16];
extern double wx,wy,wz;
extern float window_z;	

extern bool hitTestBool;

extern int kartentextur;

extern int selectedID;
extern int selectedQuadID;
extern GLubyte colorOverTerrain[3];
extern GLubyte colorOverInterface[3];

extern bool hastToCheckVisibility;	// Bei allen einheiten wird physik gerechnet und falls sie ihre position Šndern wird diese Variable auf true gesetzt und daraufhin wird sichtbarkeitstest durchgefŸfhrt


extern int nahrungsTextID,steinTextID,goldTextID,holzTextID;
extern int aggressivTextID, defensivTextID, passivTextID;

extern int controlIndex;

void setCurrentPlayer(int ID);

extern int rohstoffwarnungTexID;
extern int rohstoffwarnungMeldungID;
extern int steuerFensterID;

extern bool startCounter;


// Threads
extern SDL_Thread *kiTimer;
extern SDL_Thread *clockTimer;
extern SDL_Thread *particleSystemTimer;
extern Uint32 clockTimerRate;
extern Uint32 kiClockRate;
extern Uint32 particleSystemRate;
extern Uint32 kiGegnerDenkclockrate;
extern bool killThreadsInNextRate;


// Sonne
extern awx_vector3f sunPosition;
extern awx_vector3f sunDirection;
extern int progressTexID;


extern awx_time startTime;
extern awx_time mainTime;
extern awx_time playedTime;
extern float zeitfaktor;

extern float einkommenssteuer;
extern vector <awx_string> produkte;
extern int pressedTextFieldID;
extern int buergerButtonID;
extern int buergerHereButtonID;

extern vector <awx_building> loadedBuildings;
extern vector <awx_unit> loadedUnits;
extern int getLoadedIndex(awx_string name);
extern int getLoadedUnitIndex(awx_string name);

#endif
#endif
