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

#ifndef AWX_GUI_OBJECT_H
#define AWX_GUI_OBJECT_H

class awx_gui_object
{

	public:
	
	//Eigenschaften:
	
		float x;
		float y;
		
		float width;
		float height;
		
		int align;
		
		bool resize;
		
		void render();
		
		bool kdown;
		bool kup;
		bool mdown;
		bool mup;
		bool mover;
		
		bool hidden;
		
		
	//Aktionen:
		
		void (*mouseDownFunc)(float x , float y , int button);
		void (*mouseUpFunc)(float x , float y , int button);
		void (*keyDownFunc)(int code);
		void (*keyUpFunc)(int code);
		void (*mouseOverFunc)(float x , float y);
		
		void setMouseDownAction( void (*down)(float x , float y , int button));
		
		void setMouseUpAction( void (*up)(float x , float y , int button));
		
		void setKeyDownAction( void (*kdown)(int code));
		
		void setMouseOverAction( void (*over)(float x , float y));
		
		void setKeyUpAction( void (*kup)(int code));
		
		void mouseOver(float x , float y);
		
		void mouseDown(float x , float y , int button);
		
		void mouseUp(float x , float y , int button);
		
		void keyDown(int code);
		
		void keyUp(int code);
		
		
		awx_gui_object();
		
};

#endif