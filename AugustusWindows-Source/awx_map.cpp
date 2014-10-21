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

#include "awx_map.h"


awx_map::awx_map()
{
    texID=-1;
}

void awx_map::render()
{
    if(texID==-1) return;

    glTranslatef(x,y,0);
    glRotatef(camera.real_winkel , 0 , 0 , 1);
    if(!hidden){
        glBindTexture(GL_TEXTURE_2D , textur_liste[texID]);
        glBegin(GL_POLYGON);
            glTexCoord2f(0,0);
            glVertex3f( - width/2.0 ,  - height/2.0 , 0);
            glTexCoord2f(1,0);
            glVertex3f( + width/2.0 ,  - height/2.0 , 0);
            glTexCoord2f(1,1);
            glVertex3f( + width/2.0 ,  + height/2.0 , 0);
            glTexCoord2f(0,1);
            glVertex3f( - width/2.0 ,  + height/2.0 , 0);
            glTexCoord2f(0,0);
            glVertex3f( - width/2.0 ,  - height/2.0 , 0);
        glEnd();
    }
}

void awx_map::mouseDown(float x, float y , int button){

	return;
    awx_matrix temp;
    awx_vector3f mouse(x - this->x , y - this->y ,0);
    temp.rotatef(-camera.real_winkel,0,0,1);
    mouse = temp * mouse;
    
    if(mouse.x <= width/2.0 && mouse.x >= -width/2.0 && mouse.y <= height/2.0 && mouse.y >= -height/2.0){
        camera.setPosition(awx_vector3f(-mouse.x/width , 0 , mouse.y/width));
    }
}