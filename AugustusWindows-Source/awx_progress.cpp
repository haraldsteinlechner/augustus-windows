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

#include "awx_progress.h"

#include "awx_tga.h"

void awx_progress::render()
{
	if(value >= 0.988){
        value=0.988;
    }

    glBindTexture(GL_TEXTURE_2D , textur_liste[texID]);
    glBegin(GL_POLYGON);
        glTexCoord2f(0,0.765625);
        glVertex3f(x , y , 0);
        glTexCoord2f(0.1171875,0.765625);
        glVertex3f(x + height/2.0 , y , 0);
        glTexCoord2f(0.1171875,1);
        glVertex3f(x + height/2.0 , y + height , 0);
        glTexCoord2f(0,1);
        glVertex3f(x , y + height , 0);
        glTexCoord2f(0,0.765625);
        glVertex3f(x , y , 0);
    glEnd();
    
    glBegin(GL_POLYGON);
        glTexCoord2f(0.1171875,0.765625);
        glVertex3f(x + height/2.0 , y , 0);
        glTexCoord2f(0.8828125,0.765625);
        glVertex3f(x - height/2.0 + width , y , 0);
        glTexCoord2f(0.8828125,1);
        glVertex3f(x - height/2.0 + width , y + height , 0);
        glTexCoord2f(0.1171875,1);
        glVertex3f(x + height/2.0 , y + height , 0);
        glTexCoord2f(0.1171875,0.765625);
        glVertex3f(x + height/2.0 , y , 0);
    glEnd();
    
    glBegin(GL_POLYGON);
        glTexCoord2f(0.8828125,0.765625);
        glVertex3f(x - height/2.0 + width , y , 0);
        glTexCoord2f(1,0.765625);
        glVertex3f(x + width , y , 0);
        glTexCoord2f(1,1);
        glVertex3f(x + width , y + height , 0);
        glTexCoord2f(0.8828125,1);
        glVertex3f(x - height/2.0 + width , y + height , 0);
        glTexCoord2f(0.8828125,0.765625);
        glVertex3f(x - height/2.0 + width , y , 0);
    glEnd();
    
    
    //Fortschritt:
    if(value<0.05) return;
    
    glBegin(GL_POLYGON);
        glTexCoord2f(0,0);
        glVertex3f(x + 0.0015 , y , 0);
        glTexCoord2f(0.1171875,0);
        glVertex3f(x + 0.0015 + height/2.0 , y , 0);
        glTexCoord2f(0.1171875,0.234375);
        glVertex3f(x + 0.0015 + height/2.0 , y + height , 0);
        glTexCoord2f(0,0.234375);
        glVertex3f(x + 0.0015 , y + height , 0);
        glTexCoord2f(0,0);
        glVertex3f(x + 0.0015 , y , 0);
    glEnd();
    
    glBegin(GL_POLYGON);
        glTexCoord2f(0.1171875,0);
        glVertex3f(x + 0.0015 + height/2.0 , y , 0);
        glTexCoord2f(0.8828125,0);
        glVertex3f(x + 0.0015 - height/2.0 + value*width , y , 0);
        glTexCoord2f(0.8828125,0.234375);
        glVertex3f(x + 0.0015 - height/2.0 + value*width , y + height , 0);
        glTexCoord2f(0.1171875,0.234375);
        glVertex3f(x + 0.0015 + height/2.0 , y + height , 0);
        glTexCoord2f(0.1171875,0);
        glVertex3f(x + 0.0015 + height/2.0 , y , 0);
    glEnd();
    
    glBegin(GL_POLYGON);
        glTexCoord2f(0.8828125,0);
        glVertex3f(x + 0.0015 - height/2.0 + value*width , y , 0);
        glTexCoord2f(1,0);
        glVertex3f(x + 0.0015 + value*width , y , 0);
        glTexCoord2f(1,0.234375);
        glVertex3f(x + 0.0015 + value*width , y + height , 0);
        glTexCoord2f(0.8828125,0.234375);
        glVertex3f(x + 0.0015 - height/2.0 + value*width , y + height , 0);
        glTexCoord2f(0.8828125,0);
        glVertex3f(x + 0.0015 - height/2.0 + value*width , y , 0);
    glEnd();
}

void awx_progress::setTexture(char* texpath){
    char* temp = new char[strlen(interfacePath) + strlen(texpath) + 1];
    sprintf(temp , "%s%s" , interfacePath , texpath);
    load_tga(temp , currentID, false, false);
    texID = currentID;
    currentID++;
	delete[] temp;
}

void awx_progress::setProgress(float progress){
    value = progress;
}

awx_progress::awx_progress(){
    value = 0.0;
    width = 0.15;
    height = 0.012;
}