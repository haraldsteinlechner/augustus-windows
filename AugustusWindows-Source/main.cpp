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


// ANSI C - Dateien
#include <cstdlib>
#include <iostream> 
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>
#include <float.h>

using namespace std;


/*			Linux/Mac - Header Files			*/

#ifndef _WIN32
	
	#ifndef GCCDebugCompile
	#define GCCDebugCompile
	#endif

	
#endif


/*			Win32 Spezifische Header Files		*/

#ifdef _WIN32
	
	#define for if(0); else for
	
	#ifdef GCCDebugCompile
	#undef GCCDebugCompile
	#endif

	#ifndef isnan
	#define isnan(x) ((x)!=(x))
	#endif
	
#endif

#include "includeOpenGLAndSDL.h"



/*			         AWX Dateien				 */


// Mathematik-AWXDateien
#include "awx_vector.h"
#include "awx_constants.h" 
#include "awx_matrix.h"
#include "awx_string.h"



/*  				 AWXSetup					*/
#define aStarDebug 
#undef aStarDebug

#define aStarPathDebug
//#undef aStarPathDebug
#ifdef aStarPathDebug
vector<awx_vector3f> drawPath;
#endif

#define allowReadPixels
#undef allowReadPixels

#ifndef NVIDIACompileError
#define NVIDIACompileError
#endif

// Alphaworx-Dateien
#include "awx_triangle.h"
#include "awx_time.h"
#include "awx_array.h"
#include "awx_filesystem.h"
#include "awx_dictionary.h"
#include "awx_cpp_utilities.h"
#include "awx_hitTest.h"
#include "awx_camera.h"
#include "awx_project.h"
#include "awx_trueTypeFont.h"

#include "awx_tga.h"
#include "awx_fbo.h"
#include "awx_textureAnimationManager.h"
#include "awx_opengl_utilities.h"
#include "awx_shader.h"

#include "awx_gui_object.h"
#include "awx_button.h"
#include "awx_inputTextField.h"
#include "awx_checkbox.h"
#include "awx_radio_button.h"
#include "awx_slider.h"
#include "awx_progress.h"
#include "awx_map.h"
#include "awx_window.h"

#include "awx_blood.h"

#ifndef _WIN32
#include "awx_fuzzy_logic/fuzzy.h"
#endif
#ifdef _WIN32
#include "awx_fuzzy_rule_container.h"
#endif

#include "awx_random.h"
#include "awx_renderGlobals.h"

#include "awx_frustumCulling.h"
#include "awx_atmosphere.h"
#include "awx_cameraDrive.h"
 

#include "awx_ki.h"
#include "awx_decision_network.h"

#include "awx_functionDictionary.h" 
#include "awx_intersections.h"
#include "awx_controls.h"
#include "awx_visibilityCheck.h"
#include "awx_blob.h"
#include "awx_apx.h"
#include "awx_aax.h"
#include "awx_terrain.h"
#include "awx_level_part.h"
#include "awx_level.h"
#include "awx_unit.h"
#include "awx_building.h"
#include "awx_player.h"
#include "awx_AIEnemy.h"
#include "awx_AIController.h"
#include "awx_AStar.h"
#include "awx_unit_functions.h"
#include "awx_building_functions.h"
#include "awx_console.h"

#include "awx_mouse_functions.h"

#include "awx_gui.h"

#include "awx_storyController.h"
#include "awx_skyBoxDome.h"
#include "awx_rendering.h"
#include "awx_selection.h"
#include "awx_controlManager.h"


#include "awx_threadFuncs.h"
#include "globals.h"
#include "awx_stencilShadow.h"


#include "awx_main.h"


int SDL_main(int argc,char** argv)
{
	awx_createGame(argc,argv);
	
	return 0;
}