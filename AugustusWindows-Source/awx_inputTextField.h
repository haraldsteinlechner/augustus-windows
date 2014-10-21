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


/*
Setup:


testinput = new awx_inputTextField;
testinput->setTexture("pressed.tga");				// Hintergrundtextur des Textfeldes festlegen
testinput->width = 0.2;
testinput->height = 0.05;
testinput->x = 0.40;
testinput->y = 0.15;
testinput->initCaption();							// Text zum rendern vorbereinten
testinput->calculatePositions(8,3);					// Positionen des Textes berechnen (x,y) = offset von buttonecke(untenlinks) in pixel wie es bei 1024 wäre
testinput->setBehavior(true,true,x);		        // Nur nummern, Backspace und Return beachten, nich weniger als x zeichen zulassen
testinput->setColors(r,g,b,a,r,g,b,a);              // RGBA von Roll over, RGBA von Aktiv
testinput->setCaption("djf");                       // INhalt setzen
cout << testinput->caption << endl;                 // Inhalt lesen


in MouseOver idlefunktion:

testinput->mouseOver(xi,yi);


in MouseDown idlefunktion:

if(testinput->mouseOverFlag)
		testinput->mouseDown(xi,yi,1);
		
		
in MouseUP idlefunktion:

	if(testinput->pushedFlag)
		testinput->mouseOver(xi,yi);
		
Externes Deaktivieren von Textfeld:
testinput->pushedFlag=false;
normalerweise wird deaktiviert bei Enter oder zweitmaligen Draufklicken (immer wenn geklickt wird deaktivieren zb)
		
Render:

testinput->render();

Cleanup:

testinput->dealloc();
delete testinput;

*/

#ifndef AWX_INPUT_TEXT_FIELD
#define AWX_INPUT_TEXT_FIELD

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_project.h"
#include "globals_extern.h"
#include "awx_gui_object.h"

class awx_inputTextField : public awx_gui_object
{
	private: 
	
		float firstHeight;

	public:
		awx_string caption;
		int ttfTextID;
		int minZeichen;
		
		bool mouseOverFlag , pushedFlag , hasRollOverState , hasPushedState;
		bool enabled;
		
		bool numbersOnly, systemHandles;
		
		float rover,gover,bover,aover;
		float ractive,gactive,bactive,aactive;
		
		GLuint texture;
		
		void setTexture(char* texpath);
		
		void setTexture(int texID);
		
		void setCaption(char* str);
		
		void setCaption(awx_string str);
		
		void render();
		
		void mouseDown(float x , float y , int button);
		
		void mouseUp(float x , float y , int button);
		void mouseDownSpecial(float x , float y , int button);
		void mouseOver(float x , float y);
		
		void addCharacter(unsigned char k);
		void initCaption();
		void calculatePositions(float abstandx,float abstandy);
		void setBehavior(bool numbersOnly, bool systemHandles, int x);
		
		void setColors(float r, float g, float b, float a, float r2, float g2, float b2, float a2);
		
		awx_inputTextField();
		
		void dealloc();
};

#endif