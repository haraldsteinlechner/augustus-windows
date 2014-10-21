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

#ifndef AWX_PROGRESS_H
#define AWX_PROGRESS_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"

#include "awx_gui_object.h"


class awx_progress : public awx_gui_object{
	public:
		float value;
		int texID;

		void render();
		void setTexture(char* texpath);
		void setProgress(float progress);
		awx_progress();
		
};

#endif