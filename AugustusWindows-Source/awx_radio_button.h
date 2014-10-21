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

#ifndef AWX_RADIO_BUTTON_H
#define AWX_RADIO_BUTTON_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"
#include "awx_gui_object.h"


class awx_radio_button : public awx_gui_object{
	public:
		vector <awx_string> titles;
		vector <bool> stati;
		vector <bool> mouseOverFlag;
		int selectedIndex;
		
		float dif;
		
		int onID , offID , rollOnID , rollOffID;
		
		void addOption(char* titel);
		
		void setTexture(char* texpath);
		
		void setSelected(int index);
		
		void mouseOver(float x, float y);
		
		void mouseDown(float x , float y , int button);
		
		void render();
		
		awx_radio_button(char* item);
		
};

#endif