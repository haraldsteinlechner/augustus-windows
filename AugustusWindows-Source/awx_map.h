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

#ifndef AWX_MAP_H
#define AWX_MAP_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"
#include "awx_button.h"

#include "awx_vector.h"
#include "awx_matrix.h"
#include "awx_camera.h"

class awx_map : public awx_button
{
	public:
	
		awx_map();
	
		void render();
		
		void mouseDown(float x, float y , int button);
};

#endif