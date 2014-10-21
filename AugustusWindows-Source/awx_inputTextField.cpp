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

#include "awx_inputTextField.h"

#include "awx_tga.h"

extern vector<awx_trueTypeFont> ttfFonts;

void awx_inputTextField::setTexture(char* texpath){
    char *temp = new char[ strlen(interfacePath) + strlen(texpath) + 10 ];
    
    sprintf(temp , "%s%s" , interfacePath , texpath);
    texture = load_tgaAndReturnObject(temp, false,false);
		
	delete[] temp;
}

void awx_inputTextField::setTexture(int texID){
    texture = texID;
}

void awx_inputTextField::setCaption(char* str)
{
    caption.set(str);
}

void awx_inputTextField::setCaption(awx_string str)
{
    caption = str;
}

void awx_inputTextField::render()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	if(enabled) glColor4f(1.0,1.0,1.0,1.0);
	else glColor4f(1.0,1.0,1.0,0.5);
	
    if(!hidden){
		
		if(pushedFlag) glColor4f(ractive,gactive,bactive,aactive);
		else if(mouseOverFlag) glColor4f(rover,gover,bover,aover);
		
			
		if(ttfTextID>0&&ttfTextID<ttfFonts.size())
		ttfFonts[ttfTextID].registerRender();
		else cout << "Fehler beim Rendern des input textfields (ttfFontrender post konnte nicht durchgeführtwerden" << endl;
        
		glBindTexture(GL_TEXTURE_2D , texture);
		glBegin(GL_POLYGON);
            glTexCoord2f(0,0);
            glVertex3f(x , y , 0);
            glTexCoord2f(1,0);
            glVertex3f(x + width , y , 0);
            glTexCoord2f(1,1);
            glVertex3f(x + width , y + height , 0);
            glTexCoord2f(0,1);
            glVertex3f(x , y + height , 0);
            glTexCoord2f(0,0);
            glVertex3f(x , y , 0);
        glEnd();
        
   

        if(mouseOverFlag && mover){
            mouseOverFunc(x , y);
        }
    }
	
	glPopAttrib();
	
}

void awx_inputTextField::mouseDown(float x , float y , int button){
    if(!hidden&&mouseOverFlag){
		if(pushedFlag) 
		{
			pushedFlag=false;
			return;
		}
        pushedFlag = true;
        mouseOverFlag = false;
        if(mdown){
           mouseDownFunc(x , y , button);
        }
    }
}

void awx_inputTextField::mouseDownSpecial(float x , float y , int button){
    if(!hidden){
        pushedFlag = true;
        if(mdown){
            mouseDownFunc(x , y , button);
        }
    }
}

void awx_inputTextField::mouseUp(float x , float y , int button){
    if(!hidden){
        //pushedFlag = false;
		
		if(!( x >= this->x && y >= this->y && x <= this->x + width && y <= this->y + height)) return;
        mouseOver(x , y);

		if(mup){
            mouseUpFunc(x , y , button);
        }
    }
}

void awx_inputTextField::mouseOver(float x , float y){
    if(!hidden&&enabled){
	
        if( x >= this->x && y >= this->y && x <= this->x + width && y <= this->y + height){
            mouseOverFlag = true;
        }
        else{
            mouseOverFlag = false;
        }
    }
}

void awx_inputTextField::dealloc()
{	
	if(glIsTexture(texture)==GL_TRUE) glDeleteTextures(1,&texture);
}

awx_inputTextField::awx_inputTextField(){
	mouseOverFlag = false;
	pushedFlag = false;
	hasRollOverState = true;
	hasPushedState = true;
		
	hidden = false;
	enabled = true;
	
	ttfTextID = -1;
	texture = 0;
	

	rover=0.9;
	gover=0.9;
	bover=1.0;
	aover=1.0;
	
	ractive=1.0;
	gactive=0.5;
	bactive=0.5;
	aactive=1.0;
	
	minZeichen=0;
}

void awx_inputTextField::addCharacter(unsigned char k)
{
	cout << (int)k << endl;
	if(systemHandles)
	{
		int u = (int)k;
		if(u==8&&caption.length()>minZeichen) // BAckspace
		{
			if(caption.length()==1)
			{
				caption="";
			}
			else caption = caption.substring(0,caption.length()-2);
			
			
			if(ttfTextID>0&&ttfTextID<ttfFonts.size())
			ttfFonts[ttfTextID].calculate(caption);
			else cout << "Fehler beim generieren des Textinhaltes eines inputTextFields." << endl;
			
			return;
		}
		else if(u==13)
		{
			pushedFlag=false;
			return;
		}
	}
	
	if(numbersOnly)
	{
		int u = (int)k;
		if((u<48||u>57)&&k!='.') return;
	}
	
	char temp[2] = {k,'\0'};
	caption = caption + temp;
	
	if(ttfTextID>0&&ttfTextID<ttfFonts.size())
	ttfFonts[ttfTextID].calculate(caption);
	else cout << "Fehler beim generieren des Textinhaltes eines inputTextFields." << endl;

}

void awx_inputTextField::initCaption()
{
	if(ttfTextID>0&&ttfTextID<ttfFonts.size()) 
	{
		cout << "Fehler beim initialisieren des inputTextFields: schon initialisiert" << endl;
		return;
	}
	
	awx_trueTypeFont tempFont;
	tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,150,1,true);
	tempFont.init();
	tempFont.calculate("T");
	firstHeight = tempFont.getLineHeight();
	tempFont.calculate("");
	ttfTextID = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);
}

void awx_inputTextField::calculatePositions(float abstandx,float abstandy)
{
	if(ttfTextID>0&&ttfTextID<ttfFonts.size())
	{
		ttfFonts[ttfTextID].setAspectCorrection(false);
		float heighti = (ttfFonts[ttfTextID].getLineHeight()==0) ? firstHeight : ttfFonts[ttfTextID].getLineHeight();
		ttfFonts[ttfTextID].setRenderPosition(project.width*(x/project.aspect)+abstandx,project.height-project.height*y-heighti-abstandy);
	} else cout << "Fehler beim berechnen der Position für Eingabetext: zuerst initCaption aufrufen" << endl;
}

void awx_inputTextField::setBehavior(bool numbersOnly, bool systemHandles, int min)
{
	this->numbersOnly=numbersOnly;
	this->systemHandles=systemHandles;
	minZeichen = min;
}

void awx_inputTextField::setColors(float r, float g, float b, float a, float r2, float g2, float b2, float a2)
{
	rover=r;
	gover=g;
	bover=b;
	aover=a;
	
	ractive=r2;
	gactive=g2;
	bactive=b2;
	aactive=a2;
}