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


#ifndef AWX_CHECKBOX_H
#define AWX_CHECKBOX_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"
#include "awx_gui_object.h"

class awx_checkbox : public awx_gui_object{
	public:
		char* title;
		int offID , onID , rollOffID , rollOnID;
		bool mouseOverFlag , pushedFlag;
		
		bool status;
		
		void render();
		void setTexture(char* texpath);
		void setTitle(char* title);
		void mouseDown(float x , float y , int button);
		void mouseUp(float x , float y , int button);
		void mouseOver(float x , float y);
		awx_checkbox();
};

#endif