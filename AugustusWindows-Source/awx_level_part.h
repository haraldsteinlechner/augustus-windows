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

	// HOLZ = 1
	// GOLD = 0
	// STEIN = 2
	// ESSEN = 3

#ifndef AWX_LEVEL_PART_H
#define AWX_LEVEL_PART_H
	
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
#include "awx_aax.h"
#include "awx_level_part.h"
#include "awx_dictionary.h"
#include "awx_visibilityCheck.h"
#include "awx_triangle.h"
#include "awx_shader.h"


class levelpart: public apx, public visibilityCheck
{
	public:
	
	GLdouble tempmodelview[16];
	
	int resourceID;
	float capacity;
	float platzbedarf;
	float abbaufaktor;
	
	awx_dictionary resourceAttributes;
	int sektorX,sektorZ;
	int hitTriangle;
	awx_triangle *myHitTriangle;
	
	float scaleTex;
	GLuint stateChangeList;
	
	
	levelpart();
	
	void load_apx(const char *filename);	
	void checkVisibility();
	
	void reinit(levelpart &part);
	void init();
	
	void render();
	
	void renderWithoutTest();
	
	bool updateResourceStats();
	
	bool abbau(float abbauspeed);
	
	bool isAbbaubar();

	bool boundingCollision(awx_vector3f position);
	
	bool getVisibility();
	
	void renderWater(GLuint normalmap,GLuint sky, awx_shader *shader);
	
	void fillVRAM();
	void useStates();
};

#endif