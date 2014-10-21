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

/*						 Headerdatei für Eingabegeräte						 */

awx_window infoWindow;

class _controls
{
	public:
	
	// Steuerungsflags
	bool forward_flag;
	bool backward_flag;
	bool left_flag;
	bool right_flag;
	bool up_flag;
	bool down_flag;
	bool navigate;
	
	char f_key;
	char b_key;
	char r_key;
	char l_key;
	char up_key;
	char down_key;
	
	
	// Maus
	bool mouse_down;
	int mouse_start_x,mouse_start_y;
	int drag_dist_x, drag_dist_y;
	int x,y;
	
	bool auswahlRechteck;
	
	bool middle;
	
	awx_vector3f move_right, move_left;
	awx_vector3f move_up, move_down;
	
	float scrollspeed;
	awx_matrix matrix;
	
	awx_vector3f move;
	
	// speichert ID die mit Maus kolidiert
	vector<int> selection;
	vector <int> oldselection;
	int preselection;
	int player_preselection;
	
	int currentDragID;
	int currentDragPlayer;
	float winkel_y;
	
	// ID des Levelparts unter der Maus
	int secondarySelection;
	
	bool draggingFlag;			// Sammelpunkt wird gedragt.
	
	int debugshader;			// Flag ob debushader verwendet wird
	
	bool egoMode;				// Flag ob man in Ego-Mode ist
	int egoModeUnitIndex;		// Index der Einheit f�r ego-mode
	
	float lastWinZ;
	
	string *currentLevelPartDragName;
	
	_controls()
	{
		f_key = 'w';
		b_key = 's';
		r_key = 'd';
		l_key = 'a';
		up_key = '.';
		down_key = ',';
		
		selection.clear();
		currentDragID=-1;
		currentDragPlayer=-1;
		
		egoMode=false;			
		egoModeUnitIndex=0;
		
		middle = false;
		
		x=100;
		y=100;
		
		winkel_y=0;
		
		forward_flag=false;
		backward_flag=false;
		left_flag=false;
		right_flag=false;
		draggingFlag=false;
		
		debugshader=false;
		
		
		mouse_down=false;
		
		scrollspeed = 50;
		
		move_right.setX(-scrollspeed);
		move_left.setX(scrollspeed);
		move_up.setZ(scrollspeed);
		move_down.setZ(-scrollspeed);
		
		auswahlRechteck = false;
		
		lastWinZ=0.0;
		
		currentLevelPartDragName = NULL;
	}
	
	bool isSelected(int player , int testzahl)
	{
		for(int i=0;i<(int)selection.size();i++)
		{
			if(testzahl==selection[i] && player == controlIndex)
			{
				return true;
			}
		}
		
		return false;
	}
	
} controls;

void intersectViewRayWithTerrain();


inline void mouse_locate()
{	
	glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev (GL_MODELVIEW_MATRIX, project.mvmatrix);
	
	/*#ifdef GCCDebugCompile
	#ifdef allowReadPixels
	#warning glFinish found
	#endif
	glReadPixels(controls.x,project.height - controls.y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&window_z);
	#endif

	gluUnProject ((GLdouble) controls.x, project.height - (GLdouble) controls.y,window_z, 
			mvmatrix, project.projmatrix, project.viewport, &wx, &wy, &wz); 

			
	if(window_z==1.0) window_z=controls.lastWinZ;
	controls.lastWinZ=window_z;

	*/
}

void awx_show_info(awx_string title , awx_string description)
{
	glPushMatrix();
	glLoadIdentity();
	infoWindow.width = 0.26;
	infoWindow.height = 0.15;
	
	infoWindow.x = 0.02;
	infoWindow.y = 0.25;
	
	infoWindow.hidden = false;
	
	infoWindow.render();
	
	//awx_stroke_output(infoWindow.x + 0.03 , infoWindow.y + 0.1 , 1 , title.value);
	ttfFonts[1].registerRender((infoWindow.x + 0.03)*project.width,project.height-( infoWindow.y + 0.1)*project.height);
	
	glPopMatrix();
}

void awx_show_info(int myID)
{
	glPushMatrix();
	glLoadIdentity();
	infoWindow.width = 0.26;
	infoWindow.height = 0.15;
	
	infoWindow.x = 0.02;
	infoWindow.y = 0.25;
	
	infoWindow.hidden = false;
	
	infoWindow.render();
	
	//awx_stroke_output(infoWindow.x + 0.03 , infoWindow.y + 0.1 , 1 , title.value);
	ttfFonts[myID].registerRender((infoWindow.x + 0.024)*project.width,project.height-( infoWindow.y + 0.117)*project.height);
	
	glPopMatrix();
}