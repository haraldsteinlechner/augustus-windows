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


#ifndef AWX_TGA_H
#define AWX_TGA_H


#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_vector.h"

#include "awx_project.h"
#include "globals_extern.h"


/*

much of this code is based on NeHe's 
TGA Loading code nehe.gamedev.net

*/


typedef	struct									
{
	GLubyte	* imageData;									// Datenbereich bis 32 bit
	GLuint	bpp;											// Farbtiefe Bits Per Pixel
	GLuint	width;											// L√§nge
	GLuint	height;											// H√∂he
	GLuint	texID;											// TexturID f√ºr Liste
	GLuint	type;											// GL_RGB, GL_RGBA
	char type_s[8];											// Typ in Stringform
}Texture;

typedef struct
{
	GLubyte Header[12];									// TGA Header
} TGAHeader;


typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;					
	GLuint		imageSize;								
	GLuint		temp;							
	GLuint		type;	
	GLuint		Height;									// H√∂he des Bildes
	GLuint		Width;									// Breite des Bildes
	GLuint		Bpp;									// Bits per Pixel
} TGA;


bool LoadUncompressedTGA(Texture *, char *, FILE *);
bool LoadTGA(Texture * texture, char * filename);
bool LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA);
void load_tga(char *file_name, int current, bool mipmaps, bool allowCompression);
GLuint load_tgaAndReturnObject(char *file_name, bool mipmaps, bool allowCompression);

#endif