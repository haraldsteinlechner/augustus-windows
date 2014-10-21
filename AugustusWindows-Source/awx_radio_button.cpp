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

#include "awx_radio_button.h"

#include "awx_tga.h"
#include "awx_opengl_utilities.h"

void awx_radio_button::addOption(char* titel){
    titles.push_back(awx_string(titel));
    stati.push_back(false);
    mouseOverFlag.push_back(false);
}

void awx_radio_button::setTexture(char* texpath){
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
    
    sprintf(temp , "%s%s_on.tga" , interfacePath ,  tempstr);
    load_tga(temp , currentID, false, true);
    onID = currentID;
    currentID++;
    
    sprintf(temp , "%s%s_off.tga" , interfacePath , tempstr);
    load_tga(temp , currentID, false, true);
    offID = currentID;
    currentID++;
    
    sprintf(temp , "%s%s_off_r.tga" , interfacePath , tempstr);
    load_tga(temp , currentID, false, true);
    rollOffID = currentID;
    currentID++;
    
    sprintf(temp , "%s%s_on_r.tga" , interfacePath , tempstr);
    load_tga(temp , currentID ,false, true);
    rollOnID = currentID;
    currentID++;
	
	delete[] tempstr;
	delete[] temp;
}

void awx_radio_button::setSelected(int index){
    for(int i = 0; i < (int)mouseOverFlag.size();i++){
        if(i == index){
            stati[i] = true;
        }
        else{
            stati[i] = false;
        }
    }
}

void awx_radio_button::mouseOver(float x, float y){
    if(!hidden){
        for(int i = 0;i < (int)mouseOverFlag.size();i++){
            if( x >= this->x && y >= this->y + i*dif && x <= this->x + width && y <= this->y + i*dif + height){
                mouseOverFlag[i] = true;
            }
            else{
                mouseOverFlag[i] = false;
            }
        }
    }
}

void awx_radio_button::mouseDown(float x , float y , int button){
    if(!hidden){
        bool wild = false;
        for(int i = 0; i < (int)mouseOverFlag.size();i++){
            if( x >= this->x && y >= this->y + i*dif && x <= this->x + width && y <= this->y + i*dif + height){
                stati[i] = true;
                selectedIndex = i;
                wild = true;
            }
            else{
                stati[i] = false;
            }
        }
        if(!wild){
            stati[selectedIndex] = true;
        }
    }
}

void awx_radio_button::render(){
    if(!hidden){
        float y1 = y;
        for(int i = 0; i < (int)titles.size();i++){
            if(mouseOverFlag[i]){
                if(stati[i]){
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
                if(stati[i]){
                    glBindTexture(GL_TEXTURE_2D , textur_liste[onID]);
                }
                else{
                    glBindTexture(GL_TEXTURE_2D , textur_liste[offID]);
                }
            }
        
            glBegin(GL_POLYGON);
                glTexCoord2f( 0 , 0);
                glVertex3f(x , y1 , 0);
                glTexCoord2f( 1 , 0);
                glVertex3f(x + width , y1 , 0);
                glTexCoord2f( 1 , 1);
                glVertex3f(x + width , y1 + height , 0);
                glTexCoord2f( 0 , 1);
                glVertex3f(x , y1 + height , 0);
                glTexCoord2f( 0 , 0);
                glVertex3f(x , y1 , 0);
            glEnd();	
        
            awx_stroke_output(x + width + 0.003,y1 , 1 ,titles[i].cString());
        
            y1 += dif;
        }
    }
}

awx_radio_button::awx_radio_button(char* item){
    addOption(item);
    setSelected(0);
    width = 0.0625 / 3.0;
    height = 0.0625 / 3.0;
    dif = 0.005333333333 + height;
    selectedIndex = 0;
    setTexture("radio_button.tga");
}