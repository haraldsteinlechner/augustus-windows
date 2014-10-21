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

class CEgoController
{
	public:
	
	bool forward;
	bool backwards;
	bool right;
	bool left;
	
	int mouse_x,mouse_y;
	int mouse_last_x,mouse_last_y;
	int rot_x,rot_y;
	int drag_x,drag_y;
	
	bool left_down;
	float zoomFactor;
	int zoomFlag;
	
	CEgoController()
	{
		forward=false;
		backwards=false;
		right=false;
		left=false;
		mouse_x=0;
		mouse_y=0;
		rot_y=0;
		rot_x=0;
		mouse_last_x=0;
		mouse_last_y=0;
		drag_x=0;
		drag_y=0;
		left_down=false;
		zoomFactor=1.0;
		zoomFlag=0;
	}
	
	
	void poolEvents(SDL_Event &event)
	{
		char key=0;
		switch( event.type ) 
		{
			case SDL_KEYDOWN:
	
				key=event.key.keysym.sym;
				if(key=='a')
				{
					right=true;
				}else if(key=='d')
				{
					left=true;
				}else if(key=='w')
				{
					forward=true;
				}else if(key=='s')
				{
					backwards=true;
				}else if(event.key.keysym.sym==SDLK_PAGEUP)
				{
					zoomFlag=-1;
				}
				else if(event.key.keysym.sym==SDLK_PAGEDOWN)
				{
					zoomFlag=1;
				}
				
				break;
				
			case SDL_KEYUP:
			
				key=event.key.keysym.sym;
				if(key=='a')
				{
					right=false;
				}else if(key=='d')
				{
					left=false;
				}else if(key=='w')
				{
					forward=false;
				}else if(key=='s')
				{
					backwards=false;
				}else if(event.key.keysym.sym==SDLK_PAGEUP)
				{
					zoomFlag=0;
				}
				else if(event.key.keysym.sym==SDLK_PAGEDOWN)
				{
					zoomFlag=0;
				}
				
				break;
				
			case SDL_MOUSEMOTION:
			
				/*if(controls.mouse_down)
				{
					awx_motion_mouse(event.motion.x,event.motion.y);
				}
				else awx_passive_mouse(event.motion.x,event.motion.y);*/
				
				mouse_x=event.motion.x;
				mouse_y=event.motion.y;
			
				//SDL_WarpMouse(int(project.width/2.0),int(project.height/2.0));
				
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				//awx_mouse(event.button.button,event.button.state,event.button.x,event.button.y);
				if(event.button.button==SDL_BUTTON_MIDDLE&&event.button.state==SDL_PRESSED)
				{
					left_down=true;
				}
				
				break;
			
			case SDL_MOUSEBUTTONUP:
				if(event.button.button==SDL_BUTTON_MIDDLE&&event.button.state==SDL_RELEASED)
				{
					left_down=false;
				}
				
				break;
        }
	}
	
	void idle()
	{
		drag_x = mouse_x-mouse_last_x;
		drag_y = mouse_y-mouse_last_y;
				
		mouse_last_x=mouse_x;
		mouse_last_y=mouse_y;
		
		if(left_down)
		{
			rot_y+=drag_x;
			rot_x+=drag_y;
		}
		
		zoomFactor+=zoomFlag*0.01;
	}
};
