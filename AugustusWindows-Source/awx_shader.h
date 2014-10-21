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

#ifndef AWX_SHADER_H
#define AWX_SHADER_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"
#include "awx_opengl_utilities.h"

#define printOpenGLError() printOglError(__FILE__, __LINE__)

char *textFileRead(char *fn);
int textFileWrite(char *fn, char *s);
int printOglError(char *file, int line);
void printInfoLog(GLhandleARB obj);


class awx_shader
{
	public:
	
	GLhandleARB v,f,p;
	GLuint texture0,texture1,texture2,texture3;
	GLint uniform0;
	
	void set(char* vertexshader, char* fragmentshader);
	
	void install();
	void deinstall();
};

#endif