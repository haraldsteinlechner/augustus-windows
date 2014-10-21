#ifndef AWX_SLIDER_H
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

#define AWX_SLIDER_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"
#include "awx_gui_object.h"

class awx_slider : public awx_gui_object
{
	public:
		float value;
		int texID , pushID;
		
		bool start;
		bool mouseOverFlag;
		
		float startX;
		
		float railheight;
		
		void render();
		
		void mouseDown(float x , float y , int button);
		
		void mouseOver(float x, float y);
		
		void mouseUp(float x, float y , int button);
		
		void setTexture(char* texpath);
		
		awx_slider();
		
		void dealloc();
};

#endif