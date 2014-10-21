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

#ifndef AWX_PROJECT_H
#define AWX_PROJECT_H

#include "awx_dictionary.h"
#include <cmath>
#include <math.h>
#include <cstdio>
#include <ctime>

#include "includeOpenGLAndSDL.h"

#include <SDL/SDL.h>


typedef enum
{
	AWXStaticVolume,
	AWXDynamicVolume
} shadowMode;



// Projektdaten, Umbegunbsvariable, Infos
class Project
{
	public:
	
	float version;
	bool fullscreen;
	char *name;
	int x,y;
	int width, height;
	char *renderer, *vendor, *version_string;
	char *error_message;
	int error_code;
	int doclevel;
	int fps_int;
	float fps;
	int bpp;
	float aspect;
	bool render_mirror;
	GLint viewport[4];
	double projmatrix[16];
	double mvmatrix[16];
	
	bool hardwareCompression;
	
	// Spiegelgenerierung
	int mirror_size;
	
	float anisotropicamount;
	bool softblobs;
	int antialiasing;
	int texturedetail;
	int vertexshader,fragmentshader;
	bool shadowvolumes;
	bool perPixelLighting;
	shadowMode shadowvolumeMode;
	int planarShadowSize;
	bool instandFOWUpdate;
			
	Project()
	{
		for(int i=0;i<16;i++)
		{
			projmatrix[i]=0;
			mvmatrix[i]=0;
		}
		for(int i=0;i<4;i++)
		{
			viewport[i]=0;
		}
		mirror_size=128;
		version=4.0;
		width = 1024;
		height = 768;
		bpp=32;
		hardwareCompression=true;
		instandFOWUpdate=false;
		softblobs=false;
		fullscreen=false;
		perPixelLighting = false;
		texturedetail=1;
		name = "Augustus -- Entwicklerversion 24.Feber 2007";
		x = 0;
		y = 0;
		doclevel = 0;
		error_code=0;
		render_mirror=false;
		error_message = new char[120];
		aspect = float(width)/float(height);
		shadowvolumes = true;
		shadowvolumeMode = AWXStaticVolume;//AWXDynamicVolume  AWXStaticVolume
		planarShadowSize = 256;
		
		
		awx_dictionary dict;
		if(dict.initWithFile("settings.plist"))
		{

			awx_string res = dict.getObjectForKey("_resolution");
			sscanf(res.cString(),"%dx%d",&width,&height);
			res = dict.getObjectForKey("_depth");
			sscanf(res.cString(),"%d bpp",&bpp);
			res = dict.getObjectForKey("_softshadow");
			sscanf(res.cString(),"%d",&softblobs);
			res = dict.getObjectForKey("_fullscreen");
			sscanf(res.cString(),"%d",&fullscreen);
			res = dict.getObjectForKey("_compression");
			sscanf(res.cString(),"%d",&hardwareCompression);
			res = dict.getObjectForKey("_anisotropicamount");
			sscanf(res.cString(),"%f",&anisotropicamount);
			res = dict.getObjectForKey("_antialiasing");
			sscanf(res.cString(),"%i",&antialiasing);
			res = dict.getObjectForKey("_texturedetail");
			sscanf(res.cString(),"%i",&texturedetail);
			res = dict.getObjectForKey("_fragmentshader");
			sscanf(res.cString(),"%i",&fragmentshader);
			res = dict.getObjectForKey("_vertexshader");
			sscanf(res.cString(),"%i",&vertexshader);
			
			res = dict.getObjectForKey("_shadowtype");
			int shadowType;
			sscanf(res.cString(),"%i",&shadowType);
			cout << "Settingsdatei geoeffnet. \nNeue Einstellungen geladen:" << endl;
			if(shadowType==0)
			{
				shadowvolumes = false;
				cout << "Verwendet keine shadowvolumes" << endl;
			} else if(shadowType==1)
			{
				cout << "Verwendet keine shadowvolumes" << endl;
				shadowvolumes = false;
			} else if(shadowType==2)
			{
				shadowvolumes = true;
				shadowvolumeMode = AWXStaticVolume;
				cout << "Verwendet Stat shadowvolumes" << endl;
			} else if(shadowType==3)
			{
				shadowvolumes = true;
				shadowvolumeMode = AWXDynamicVolume;
				cout << "Verwendet Dyn shadowvolumes" << endl;
			}
			
			res = dict.getObjectForKey("_perPixelLighting");
			sscanf(res.cString(),"%i",&perPixelLighting);
			
			res = dict.getObjectForKey("_shadowtexsize");
			sscanf(res.cString(),"%i",&planarShadowSize);
			int o = 6;//int(log2f(planarShadowSize));
			planarShadowSize = int(pow(2.0,double(o+1)));
			
			cout << width << "x" << height << ":" << bpp << endl;
			cout << "Softshadow: " << softblobs << endl;
			cout << "Fullscreen: " << fullscreen << endl;
			cout << "HW-Texturcompression: " << hardwareCompression << endl;
			cout << "Anisotropie: " <<  anisotropicamount << endl;
			cout << "MultiSampling: " << antialiasing << endl;
			cout << "ShadowTexSize: " << planarShadowSize << endl;
			cout << "ShadowType: " << shadowType << endl;
			aspect = float(width)/float(height);
		}
		else cout << "Settingsdatei konnte nicht gešffnet werden. Es werden Standardeinstellungen verwendet." << endl;
	}
	
	// Methode welche Fenster erstellt
	void create_window()
	{
		glutInitWindowSize(width,height);						// FenstergrÃ¶ÃŸe festlegen
		glutInitWindowPosition(x,y);							// Fensterposition festlegen
		glutCreateWindow(name);									// Fenster erstellen
	}
	
	int getError()
	{
		if(error_code!=0)
		{
			printf("\n\nEin Laufzeitfehler ist aufgetreten. Fehlercode ist %i",error_code);
			printf("\nFehlerstring: %s\n",error_message);
			getchar();
			exit(0);
		}
		return 0;
	}
	
	
};

// Runtimebetreffende Methoden wie genFPS
class _runtime
{
	public:
	int fps, fps_r;
	int passiveFPS;
	long int last;
	char physikinfo[40];
	int messungen;
	
	Uint32 lastTime;
	Uint32 lastMessung;
	
	_runtime()
	{
		fps=15;
		fps_r=1;
		strcpy(physikinfo,"100 km/h Gang: 3 n=6000");
	}
	
	void genFPS()
	{
		fps_r++;	

		if((time(NULL)-last)>=1)
		{
			fps=fps_r;
			fps_r=0;
			last = time(NULL);
			if(messungen<3) messungen++;
		}
		if(messungen<3) fps = 20;
	}
	
	void genFPSSDL()
	{
		Uint32 now = SDL_GetTicks();
		Uint32 diff = now - lastTime;
		if((now-lastMessung)>50)
		{
			fps = (int)((1.0/double(diff))*1000);
			lastMessung=now;
		}
		lastTime = now;
	}
};

class _awx_utilities
{
	public:
	
	void strich()
	{
		printf("====================");
	}
};

#endif