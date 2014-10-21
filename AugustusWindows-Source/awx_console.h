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

#ifndef AWX_CONSOLE_H
#define AWX_CONSOLE_H

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"
#include "awx_gui_object.h"


class awx_console : public awx_gui_object{
	public:
		char* value;
		char newLine[128];
		int depth;
		
		vector <awx_string> history;
		
		void render();
		
        void awx_command(char * command);
		
		void keyDown(int code);
		
		void getusername(char *&username);		
		awx_console();
};

#endif