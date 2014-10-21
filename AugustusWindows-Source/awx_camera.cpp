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

#include "awx_camera.h"

_camera::_camera()
{
    setup();
}

void _camera::setup()
{
    for(int i=0;i<16;i++) modelview[i]=0;
    for(int i=0;i<16;i++) rotMatrix[i]=0;
    
    position.setX(-143.0);
    position.setY(56.0);
    position.setZ(62.0);
	direction.setX(-143);
    direction.setY(-6.4);
    direction.setZ(76.14);
	
    
    winkel_x=130;
    winkel_y=24;
}

float _camera::calcRotAngle(float distance)
{
    return distance/10;
}

void _camera::setCamera()
{	
    gluLookAt(position.getX(),position.getY(),position.getZ(),
            direction.getX(),direction.getY(),direction.getZ(),
            0,1,0);
		
}

void _camera::setPosition(awx_vector3f x){
    position = position-direction;
    direction = awx_vector3f(x.x * 400/*level10.terrain.xmax*/ , 0 , x.z * 400/*level10.terrain.zmax*/);
    position = position + direction;
    visibitilityCheckInNextFrame = true;
}

int _camera::writeToFile(FILE *file)
{
	return fprintf(file,"%f;%f;%f;;%f;%f;%f;;%f;;%f",position.x,position.y,position.z,direction.x,direction.y,direction.z,winkel_x,winkel_y);
}

int _camera::loadFromFile(FILE *file)
{
	return fscanf(file,"%f;%f;%f;;%f;%f;%f;;%f;;%f",&position.x,&position.y,&position.z,&direction.x,&direction.y,&direction.z,&winkel_x,&winkel_y);
}