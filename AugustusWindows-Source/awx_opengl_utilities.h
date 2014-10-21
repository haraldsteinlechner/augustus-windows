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

#ifndef AWX_OPENGL_UTILITIES_H
#define AWX_OPENGL_UTILITIES_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"


/*						Zusätzliche OpenGL Tools von AWX					 */

void awx_check_ogl();

/*
 This is SGI sample code taken directly from OpenGL.org:
 http://www.opengl.org/developers/code/features/OGLextensions/OGLextensions.html
 */
int OpenGLExtensionIsSupported(const char* extension);


// AWX Textklasse wobei die Zeichen als Bitmaps gerendert werden.
class awx_text
{
	private:
		
		int lenght;
		int loop;
	
	public:
	
	float position_x;
	float position_y;
	
	char *string;
	
	float color[3];
	
	awx_text();
	
	void setProperties(float x, float y, char *text, float R, float G, float B);
	
	bool render();
		
	
	bool set_and_render_text(float x, float y, char *text, float R, float G, float B);
		
};

int awx_ScreenShot();

void awx_stroke_output_simple(GLfloat x, GLfloat y, GLfloat scale,  char *text);

void awx_stroke_output(GLfloat x, GLfloat y, GLfloat scale,  char *text);

void rotateTexture(int ID, float winkel, int size, bool mipmaps, bool compression);

void outputBitmapFont(float x, float y, char *string);

void renderLoadingScreen(char*  val);

void debugTextur(int texture,int x, int y);

void CalcTangentSpaceVectors(const float pos1[3],
                             const float pos2[3],
                             const float pos3[3],
                             const float texCoord1[2],
                             const float texCoord2[2],
                             const float texCoord3[2],
                             awx_vector3f &tangent,
                             awx_vector3f &bitangent,
                             awx_vector3f &normal);

#endif