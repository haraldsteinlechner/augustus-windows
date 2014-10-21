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


apx fahne_apx;
Uint32 lastFOWUpdate=0;
Uint32 lastFOWDowngrade=0;

void setCameraToEnemy();
void setFogOfWar(float value);

int firstFrame=0;

typedef enum
{
	AWXMenu,
	AWXGame
} currentModeStruct;

currentModeStruct currentMode=AWXMenu;

// Funktionspointer
void (*eventFKTPointer)();
void (*renderFKTPointer)();
void (*idleFKTPointer)();


#include "awx_inGame.h"
#include "awx_GameController.h"


// Einstellen der Buffer und Render-Optionen
void initOpenGL()
{
	float light_position[]={0.0,10.0,50.0,1.0};
	//float direction[] = {0.0,1.0,0.0};
	float diffuse[] = {255.0/255.0,200.0/255.0,200.0/255.0,1};
	float ambient[] = {1.0,1.0,1.0,1.0};
	//float diffuse[] = {1,1,1,0.5};

	GLfloat lmodel_ambient[] = {1,1,1,1.0};

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient); 
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
	//glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, direction);
	glLightfv(GL_LIGHT0,GL_SPECULAR,ambient);
	glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION, 0);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
		
	glDepthFunc(GL_LEQUAL);	
	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.0,0.0,0.0,0.0);	
	//glClearColor(0.3,0.5,1.0,0.0);					
	glShadeModel(GL_SMOOTH);
			
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.5);
	
	glEnable(GL_NORMALIZE);
	
	//glEnable(GL_MULTISAMPLE);

	//glGenTextures(1500, textur_liste);
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	
}

// Funktion, welche Grafik, Fenster, Fullscreen einstellt
void awx_runtime_environment(void)
{
	const SDL_VideoInfo* info = NULL;
    int width = project.width;
    int height = project.height;
    int bpp = project.bpp;
	int flags;
	
	if(project.shadowvolumes&&bpp==16)
	{
		cout << "Um Shadowvolumes zu benutzen sind 32bit Farbtiefe erforderlich. Farbtiefe wird angepasst." << endl;
		bpp=32;
	}
	
	if(project.fullscreen)
			flags = SDL_OPENGL | SDL_FULLSCREEN;
	else	flags = SDL_OPENGL;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        fprintf( stderr, "SDL-Init fehlgeschlagen %s\n", SDL_GetError());
        awx_shutdown(1);
    }

    info = SDL_GetVideoInfo( );

    if( !info ) {
        fprintf(stderr, "Videoinformationen konnten nicht gelesen werden: %s\n", SDL_GetError());
        awx_shutdown(1);
    }
	
	SDL_WM_SetCaption(project.name,NULL);

    //bpp = info->vfmt->BitsPerPixel; // Derzeitige bpp

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	if(project.shadowvolumes) SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8);
	
	/*SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 5);
	SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 5);
	SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 5);
	SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 5);*/
	
	if(project.antialiasing!=0)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,2);
		//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
	}

    if( SDL_SetVideoMode( width, height, bpp, flags) == 0 )
	{
		printf("Auflösung zurückgesetzt\n");
		width=1024;
		height=768;
		bpp=16;
		
		project.width=width;
		project.height=height;
		project.bpp=bpp;
		
		if( SDL_SetVideoMode( width, height, bpp, flags) == 0 )
		{
			fprintf( stderr, "Videomode konnte nicht gesetzt werden %s\n",SDL_GetError());
			awx_shutdown(1);
		}
    }
		
	
	awx_check_ogl();																	// OpenGL Version √ºberpr√ºfen
	printf("\nGrafikhardware: %s\n",project.renderer);
	printf("OpenGL-Version: %s\n",project.version_string);
	
	static GLint buf[1], sbuf[1];

	glGetIntegerv (GL_SAMPLE_BUFFERS_ARB, buf);
	printf ("\nAnzahl der Multisamplebuffer %d\n", buf[0]);
	glGetIntegerv (GL_SAMPLES_ARB, sbuf);
	printf ("Anzahl der Samples %d\n\n", sbuf[0]);
}

// Funktion die immer ausgef√ºhrt



void awx_mainLoop()
{
	while(1)
	{
		(*eventFKTPointer)();
		(*renderFKTPointer)();
		(*idleFKTPointer)();
    }
}


#include "awx_mainMenu.h"

// AWX-Main
int awx_createGame(int argc,char** argv)
{
	printf("\n%s- %f  AWX\n\n\n",project.name,project.version);
	
	printf("\nSDL initialisieren, Umgebung erzeugen....\n");	

	glutInit(&argc,argv);//freeglut requires this also for text rendering etc
	
	awx_runtime_environment();			// Fenster erzeugen, Fullscreen passieren usw.

	eventFKTPointer = processEventsMainMenu;
	renderFKTPointer = renderMainMenu;
	idleFKTPointer = idleMainMenu;
	
	awx_mainLoop();

	
	return 0;
}