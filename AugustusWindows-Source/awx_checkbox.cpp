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

#include "awx_checkbox.h"

#include "awx_tga.h"
#include "awx_opengl_utilities.h"

void awx_checkbox::render(){
    if(!hidden){
        if(mouseOverFlag){
            if(status){
                glBindTexture(GL_TEXTURE_2D , textur_liste[rollOnID]);
            }
            else{
                glBindTexture(GL_TEXTURE_2D , textur_liste[rollOffID]);
            }
            if(mover){
                mouseOverFunc(x , y);
            }
        }
        else{
            if(status){
                glBindTexture(GL_TEXTURE_2D , textur_liste[onID]);
            }
            else{
                glBindTexture(GL_TEXTURE_2D , textur_liste[offID]);
            }
        }
        glBegin(GL_POLYGON);
            glTexCoord2f( 0 , 0);
            glVertex3f(x , y , 0);
            glTexCoord2f( 1 , 0);
            glVertex3f(x + width , y , 0);
            glTexCoord2f( 1 , 1);
            glVertex3f(x + width , y + height , 0);
            glTexCoord2f( 0 , 1);
            glVertex3f(x , y + height , 0);
            glTexCoord2f( 0 , 0);
            glVertex3f(x , y , 0);
        glEnd();
    
        awx_stroke_output(x + width - 0.007,y , 1 ,title);
    
        if(mover && mouseOverFlag){
            mouseOverFunc(x , y);
        }
    }
}

void awx_checkbox::setTexture(char* texpath){
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
    
    sprintf(temp , "%s%s_on.tga" , interfacePath , tempstr);
    load_tga(temp , currentID, false, true);
    onID = currentID;
    currentID++;
    
    sprintf(temp , "%s%s_off.tga" , interfacePath ,  tempstr);
    load_tga(temp , currentID, false, true);
    offID = currentID;
    currentID++;
    
    sprintf(temp , "%s%s_off_r.tga" , interfacePath ,  tempstr);
    load_tga(temp , currentID, false, true);
    rollOffID = currentID;
    currentID++;
    
    sprintf(temp , "%s%s_on_r.tga" , interfacePath , tempstr);
    load_tga(temp , currentID, false, true);
    rollOnID = currentID;
    currentID++;
	
	delete[] tempstr;
	delete[] temp;
}

void awx_checkbox::setTitle(char* title){
    this->title = new char[strlen(title)];
    
    strcpy(this->title , title);
}

void awx_checkbox::mouseDown(float x , float y , int button){
    if(!hidden){
        if(!status)status = true;
        else status = false;
        if(mdown){
            mouseDownFunc(x , y , button);
        }
    }
}

void awx_checkbox::mouseUp(float x , float y , int button){
    if(!hidden){
        pushedFlag = false;
        mouseOver(x , y);
    }
}

void awx_checkbox::mouseOver(float x , float y){
    if(!hidden){
        if( x >= this->x && y >= this->y && x <= this->x + width && y <= this->y + height){
            mouseOverFlag = true;
        }
        else{
            mouseOverFlag = false;
        }
    }
}

awx_checkbox::awx_checkbox(){
    width = 0.0625 / 2.0;
    height = 0.0625 / 2.0;
    
    mouseOverFlag = false;
    pushedFlag = false;
    status = false;
    setTexture("tickbox.tga");
}
