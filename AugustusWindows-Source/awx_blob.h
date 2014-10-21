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

#ifndef AWX_BLOB_H
#define AWX_BLOB_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_vector.h"

#include "awx_project.h"
#include "globals_extern.h"

#include "awx_tga.h"

class awx_blob
{
	public:
	
	int blobtexturID;
	float blobsizex;
	float blobsizey;
	float winkel_y;
	float transOffset;
	
	float xmin,xmax,zmin,zmax;
	
	awx_blob();

	void loadBlobs(char* path, float size);
	
	void renderBlobs(float winkel_y, awx_vector3f position,int ID);
	
	void renderBlobs(float modelview[],int ID);
	
	void renderBlobs(awx_vector3f position,int ID, float size);
	
	void renderBlobs(awx_vector3f position,int ID, float sizex,float sizey,float winkel);
	
	void renderBlobs(awx_vector3f position);
	
	void renderBlobWithTextureObject(float winkel_y, awx_vector3f position,GLuint ID);
	
	void setBlob(int i); 
	
	void setRange(float x1,float x2,float x3,float x4);
	
	void setSize(float x,float y);
};

#endif