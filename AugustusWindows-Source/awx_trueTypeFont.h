///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   � Alphaworx Corporation //  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////

#ifndef AWX_TRUETYPEFONT_H
#define AWX_TRUETYPEFONT_H


#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_string.h"

#define awx_anzahl_moegliche_zeilen 128

#include "awx_project.h"
#include "globals_extern.h"


void SDL_GL_Enter2DMode();
static int power_of_two(int input);

GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord);

void SDL_GL_Leave2DMode();

typedef struct
{
	int x, y, w, h;
	GLfloat texMinX, texMinY;
	GLfloat texMaxX, texMaxY;
	bool leerzeile;
} geometrie;

typedef struct
{
	awx_string path;
	TTF_Font *font;
	int pt;
}fonts;

class awx_trueTypeFont
{
	public: 
	
	float width;

	private:

	int renderstyle;
	SDL_Color maincolor;
	awx_string path;
	
	TTF_Font *font;
	SDL_Surface *text;
	bool prepared;
	GLuint texture[awx_anzahl_moegliche_zeilen];
	
	GLfloat texcoord[4];
	
	int zeilenzaehler;
	
	geometrie quadDaten[awx_anzahl_moegliche_zeilen];
	GLfloat texMinX, texMinY;
	GLfloat texMaxX, texMaxY;
	
	int ptsize;
	float zeilenabstand;
	int wirklichehoehe;
	
	int dynamicScale;

	enum {
		RENDER_LATIN1,
		RENDER_UTF8,
		RENDER_UNICODE
	} rendertype;
	
	int festebreite;
	int useFormating;
	
	bool directFont;
	
	int renderPositionX;
	int renderPositionY;
	bool useAspectCorrection;
	
	
	public:
	
	awx_string caption;
	bool renderFlag;
	int x, y, w, h;
	int longestLine;
	
	awx_trueTypeFont();
	
	~awx_trueTypeFont();
	
	int calculate();
	
	void calculate(awx_string text);
	
	void calculate(const char* text);
	
	void setCaption(awx_string text);
	
	void setCaption(const char* text);
	
	awx_string getCaption();
	
	// Festebreite=0 -> Zeichenbasierende automatische Zeilenumbrüche
	// Festebreite=1 -> Pixelbasierende automatische Zeilenumbrüche
	// Festebreite=-1 -> Keine automatischen umbrüche
	
	void setFormat(bool bold, bool italic, bool underlined, unsigned char r, unsigned char g, unsigned char b,char* pathToFont, int size, float abstand, float width, int festebreite, int useFormating);
		
	int init();
		
	void render(int x, int y);
		
	void renderDirect();
		
	void renderDirect(int x, int y);

	void registerRender(float x, float y);
	
	void registerRender();

	void renderBlank();

	bool checkFormat(const char* text, char format);
	
	void cleanup();
	
	void setRenderPosition(int x, int y);
	void setRenderPositionY(int y);
	
	int getWidth();
	
	int getHeight();
	
	int getLongestLine();
	
	int getLineHeight();

	int getLineWidth(int index);
	
	void setDynamicScale(float scale);
	
	void setAspectCorrection(bool);
};


#endif