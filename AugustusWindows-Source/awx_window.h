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

#ifndef AWX_WINDOW_H
#define AWX_WINDOW_H

#include "includeOpenGLAndSDL.h"
#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"
#include "awx_gui_object.h"
#include "awx_button.h"
#include "awx_checkbox.h"
#include "awx_radio_button.h"
#include "awx_slider.h"
#include "awx_progress.h"
#include "awx_map.h"
#include "awx_inputTextField.h"

class awx_window : public awx_gui_object
{		
	public:
		
		vector <awx_button> buttons;
		vector <awx_checkbox> checkboxes;
		vector <awx_radio_button> radio_buttons;
		vector <awx_slider> sliders;
		vector <awx_progress> progress_indicators;
		vector <awx_inputTextField> textFields;
		awx_map map;
		
		float aspect;
		
		int texID;
		
		bool specialTexture;
		
		void setTexture(char* texpath);
		
		void setSpecialTexture(char * texpath);
		
		void render();
		
		void mouseOver(float x , float y);
		
		void mouseDown(float x , float y , int button);
		
		void mouseUp(float x , float y , int button);
		
		void dealloc();
};

#endif