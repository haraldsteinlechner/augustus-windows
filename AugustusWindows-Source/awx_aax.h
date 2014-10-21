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

#ifndef AWX_AAX_H
#define AWX_AAX_H

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


typedef struct
{
    float modelview[16];
} transformations;

typedef struct
{
	float winkel;
	float x,y,z;
	float xPos, yPos, zPos;
} rotations_type;

class aax
{
	public:
	
	transformations *objects;
	int ID;
	awx_vector3f position;
	awx_vector3f watch;

	unsigned int loop;
	int u;
	
	// Grafische Transformationen
	float rotation;
	float rot_x,rot_y,rot_z;
	float x,y,z;
	
	// Modelview
	float modelview[16];
	float modelview2[16];
	float identity[16];
	float resultmatrix[16];
	float resultmatrix2[16];
	
	awx_string workingdirectory;
	awx_dictionary attributes;
	
	public:
	
	int number_of_objects;
	apx *data;
	
	
	GLdouble winx,winy,winz;
	bool hasShadowVolume;
	
	aax();
	int openFile(const char* filename);
	void printTransformations();
	int render();
	void loadIdentity();
	
	// Transformationen f
	void translatef(float x, float y, float z);
	void rotate_um_mittelpunktf(float winkel, float x, float y, float z);
	void rotate_um_achsef(float winkel, float x, float y, float z, float x2, float y2, float z2);
	void rotate_um_achse_absf(float winkel, float x, float y, float z, float x2, float y2, float z2);
	void setPositionf(float x, float y, float z);
	// Transformationen fv
	void translatefv(awx_vector3f vec);
	void rotate_um_mittelpunktfv(float winkel, awx_vector3f vec);
	void rotate_um_achsefv(float winkel, awx_vector3f vec1, awx_vector3f vec2);
	void rotate_um_achse_absfv(float winkel, awx_vector3f vec1, awx_vector3f vec2);
	void setPositionfv(awx_vector3f vec1);
	void rotate_um_achse_worldf(float winkel, float x, float y, float z, float x2, float y2, float z2);
	void posGetter();
	void translate_to_worldf(float x, float y, float z);
	int playAll();
	awx_vector3f getPositionOfVector_res(awx_vector3f vec);
	void renderBlank();
	
	awx_vector3f getPositionOfVector(awx_vector3f vec);
	awx_vector3f getPositionOfVector2(awx_vector3f vec);
	awx_vector3f getPositionOfVector_ress(awx_vector3f vec);

};

#endif