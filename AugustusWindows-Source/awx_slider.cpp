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

#include "awx_slider.h"

#include "awx_tga.h"
#include "awx_opengl_utilities.h"

void awx_slider::render(){
    if(width > height && !hidden){
        
        if(start){
            glBindTexture(GL_TEXTURE_2D , textur_liste[pushID]);
        }
        else{
            glBindTexture(GL_TEXTURE_2D , textur_liste[texID]);
        }
        
        glPushMatrix();
                            
            glTranslatef(0 , height/(railheight) , 0);
            
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glVertex3f(x , y , 0);
                glTexCoord2f(0.1718,0.0);
                glVertex3f(x + height/railheight , y , 0);
                glTexCoord2f(0.1718,0.1718);
                glVertex3f(x + height/railheight , y + height/railheight , 0);
                glTexCoord2f(0,0.1718);
                glVertex3f(x , y + height/railheight , 0);
                glTexCoord2f(0,0);
                glVertex3f(x , y , 0);
            glEnd();
            
            glBegin(GL_POLYGON);
                glTexCoord2f(0.1718,0.0);
                glVertex3f(x + height/railheight , y , 0);
                glTexCoord2f(0.8594,0.0);
                glVertex3f(x + width - height/railheight , y , 0);
                glTexCoord2f(0.8594,0.1718);
                glVertex3f(x + width - height/railheight , y + height/railheight , 0);
                glTexCoord2f(0.1718,0.1718);
                glVertex3f(x + height/railheight , y + height/railheight , 0);
                glTexCoord2f(0.1718,0);
                glVertex3f(x + height/railheight , y , 0);
            glEnd();
            
            glBegin(GL_POLYGON);
                glTexCoord2f(0.8282,0.0);
                glVertex3f(x + width - height/railheight , y , 0);
                glTexCoord2f(0.99,0.0);
                glVertex3f(x + width, y , 0);
                glTexCoord2f(0.99,0.1718);
                glVertex3f(x + width , y + height/railheight , 0);
                glTexCoord2f(0.8282,0.1718);
                glVertex3f(x + width - height/railheight , y + height/railheight , 0);
                glTexCoord2f(0.8282,0);
                glVertex3f(x + width - height/railheight , y , 0);
            glEnd();
        glPopMatrix();
        
        glPushMatrix();
            
            glTranslatef(value * width - height/2.0 , 0 , 0);
            
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0.375);
                glVertex3f(x , y , 0);
                glTexCoord2f(0.625,0.375);
                glVertex3f(x + height , y , 0);
                glTexCoord2f(0.625,1.0);
                glVertex3f(x + height , y + height , 0);
                glTexCoord2f(0,1.0);
                glVertex3f(x , y + height , 0);
                glTexCoord2f(0,0.375);
                glVertex3f(x , y , 0);
            glEnd();
        glPopMatrix();
        
        if(mouseOverFlag && mover){
            mouseOverFunc(x , y);
        }
    }
}

void awx_slider::mouseDown(float x , float y , int button){
    if(!hidden){
        if(x <= this->x + value * width + height/2.0 && x >= this->x + value * width - height/2.0 ){
            start = true;
            startX = x;
        }
    }
}

void awx_slider::mouseOver(float x, float y){
    if(!hidden){
        if(start){
            if(x >= this->x && x <= this->x + width && value <= 1 && value >= 0){
                value += (x - startX)/ width;
                startX = x;
            }
            else if( x > this->x + width || value >= 1){
                value=1.0;
            }
            else if( x < this->x  ||  value <= 0){
                value=0.0;
            }
        }
        else{
            mouseOverFlag = true;
        }
    }
	cout << "flag=" << mouseOverFlag << endl;
}

void awx_slider::mouseUp(float x, float y , int button){
    if(!hidden){
        start = false;
        startX = x;
    }
}

void awx_slider::dealloc()
{
	glDeleteTextures(1,&textur_liste[texID]);
}

void awx_slider::setTexture(char* texpath){
    char *tempstr = new char[ strlen(interfacePath) + strlen(texpath) + 1 ];
    char *temp = new char[ strlen(interfacePath) + strlen(texpath) + 10 ];
    int tempint = 0;
    
    for(int i = (int)strlen(texpath); i > 0; i--){
        if(texpath[i] == '.'){
            tempint = i;
            break;
        }
    }
    
    for(int i = 0; i < tempint;i++){
        tempstr[i] = texpath[i];
    }
    tempstr[tempint] = '\0';
    
    sprintf(temp , "%s%s.tga" , interfacePath ,  tempstr);
    load_tga(temp , currentID, false, true);
    texID = currentID;
    currentID++;
    
    sprintf(temp , "%s%s_p.tga" , interfacePath ,  tempstr);
    load_tga(temp , currentID, false, true);
    pushID = currentID;
    currentID++;
	
	delete[] tempstr;
	delete[] temp;
}

awx_slider::awx_slider(){
    setTexture("slider.tga");
    height = 0.023;
    width = 0.13; 
    value=0.0;
    start = false;
    startX = 0;
    railheight = 3;
}