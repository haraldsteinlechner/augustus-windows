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

#include "awx_button.h"

#include "awx_tga.h"

void awx_button::setTexture(char* texpath){
    char *temp = new char[ strlen(interfacePath) + strlen(texpath) + 10 ];
    
    sprintf(temp , "%s%s" , interfacePath , texpath);
    load_tga(temp , currentID, false, false);
    texID = currentID;
    currentID++;
	
	useOwnPressedAndRollOver=false;
	
	delete[] temp;
}

void awx_button::setTexture(int texID){
    this->texID = texID;
}

void awx_button::setTexture(char* tex, char* over, char* pressed)
{
	useOwnPressedAndRollOver=true;
	
	char *temp = new char[ strlen(interfacePath) + strlen(tex) + 10 ];
    
    sprintf(temp , "%s%s" , interfacePath , tex);
    normalT = load_tgaAndReturnObject(temp , false, false);
	
	sprintf(temp , "%s%s" , interfacePath , over);
    overT = load_tgaAndReturnObject(temp , false, false);
	cout << "overT=" << overT << endl;
	
	sprintf(temp , "%s%s" , interfacePath , pressed);
    pressedT = load_tgaAndReturnObject(temp , false, false);
	
	delete[] temp;
}

void awx_button::setTitle(char* str)
{
    title.set(str);
}

void awx_button::setTitle(awx_string str)
{
    title = str;
}

void awx_button::render()
{
	//glDisable(GL_TEXTURE_2D);

	if(!enabled)
	{
		glColor4f(1.0,1.0,1.0,0.5);
		
	}	
	
    if(!hidden){
        if(useOwnPressedAndRollOver) glBindTexture(GL_TEXTURE_2D , normalT);
		else  glBindTexture(GL_TEXTURE_2D , textur_liste[texID]);
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
        
       if(mouseOverFlag && hasRollOverState){
			if(useOwnPressedAndRollOver) glBindTexture(GL_TEXTURE_2D , overT);
            else glBindTexture(GL_TEXTURE_2D , textur_liste[rollOverID]);
            
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
        }
        else
		if(pushedFlag && hasPushedState){
			if(useOwnPressedAndRollOver) glBindTexture(GL_TEXTURE_2D , pressedT);
            else glBindTexture(GL_TEXTURE_2D , textur_liste[pressedID]);
                                
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
        }

        if(mouseOverFlag && mover){
            mouseOverFunc(x , y);
        }
    }
	
	if(!enabled)
	{
		glColor4f(1.0,1.0,1.0,1.0);
		
	}
	
}

void awx_button::mouseDown(float x , float y , int button){
    if(!hidden){
        pushedFlag = true;
        mouseOverFlag = false;
        if(mdown){
           mouseDownFunc(x , y , button);
        }
    }
}

void awx_button::mouseDownSpecial(float x , float y , int button){
    if(!hidden){
        pushedFlag = true;
        if(mdown){
            mouseDownFunc(x , y , button);
        }
    }
}

void awx_button::mouseUp(float x , float y , int button){
    if(!hidden){
        pushedFlag = false;
		
		if(!( x >= this->x && y >= this->y && x <= this->x + width && y <= this->y + height)) return;
        mouseOver(x , y);

		if(mup){
            mouseUpFunc(x , y , button);
        }
    }
}

void awx_button::mouseOver(float x , float y){
    if(!hidden&&enabled){
	
        if( x >= this->x && y >= this->y && x <= this->x + width && y <= this->y + height){
            mouseOverFlag = true;
        }
        else{
            mouseOverFlag = false;
			pushedFlag = false;
        }
    }
}

void awx_button::dealloc()
{	
	if(useOwnPressedAndRollOver)
	{
		if(glIsTexture(normalT)==GL_TRUE) glDeleteTextures(1,&normalT);
		if(glIsTexture(overT)==GL_TRUE) glDeleteTextures(1,&overT);
		if(glIsTexture(pressedT)==GL_TRUE) glDeleteTextures(1,&pressedT);
		cout << "Deallocated Texture:" << normalT << endl;
		cout << "Deallocated Texture:" << overT << endl;
		cout << "Deallocated Texture:" << pressedT << endl;
	}
	else
	{
		if(glIsTexture(textur_liste[texID])==GL_TRUE) glDeleteTextures(1,&textur_liste[texID]);
		cout << "Deallocated Texture:" << texID << endl;
	}
}

awx_button::awx_button(){
	mouseOverFlag = false;
	pushedFlag = false;
	hasRollOverState = true;
	hasPushedState = true;
		
	hidden = false;
	enabled = true;
	
	useOwnPressedAndRollOver=false;
}