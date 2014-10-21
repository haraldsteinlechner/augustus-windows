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

#define AWX_BLOOD_H
#ifndef AWX_BLOOD_H
#define AWX_BLOOD_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#define RANDOM_FLOAT (((float)rand())/RAND_MAX)

class awx_particle
{
	public:
	
	float x,y,z;
	
};

class awx_particleSystem
{
	public:
	
	vector<awx_particle> particles;
	float r,g,b,a;
	float dir_x,dir_y,dir_z;
	float speed;
	bool running;
	float x,y,z;
	
	awx_particleSystem();
	void render();
	void update();
	void reset();	
	~awx_particleSystem();
};

#endif
