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

#ifndef AWX_BUTTON_H
#define AWX_BUTTON_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"
#include "awx_gui_object.h"

class awx_button : public awx_gui_object
{
	public:
		awx_string title;
		int texID;
		bool mouseOverFlag , pushedFlag , hasRollOverState , hasPushedState;
		bool enabled;
		bool useOwnPressedAndRollOver;
		int pressID,overID;
		GLuint normalT,pressedT,overT;
		
		void setTexture(char* texpath);
		
		void setTexture(int texID);
		
		void setTitle(char* str);
		
		void setTitle(awx_string str);
		
		void render();
		
		void mouseDown(float x , float y , int button);
		
		void mouseUp(float x , float y , int button);
		void mouseDownSpecial(float x , float y , int button);
		
		void mouseOver(float x , float y);
		
		void awx_button::setTexture(char* tex, char* over, char* pressed);
		
		awx_button();
		
		void dealloc();
};

#endif