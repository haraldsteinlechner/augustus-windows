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

/*					Cameraklasse + CameraUtilitityKit						 */

#ifndef AWX_CAMERA_H
#define AWX_CAMERA_H

#include "includeOpenGLAndSDL.h"

#include "awx_project.h"
#include "globals_extern.h"

#include "awx_vector.h"

class _camera
{
	public:
	
	float modelview[16];
	awx_vector3f position;
	awx_vector3f direction;
	float winkel_x;
	float winkel_y;
	float rotMatrix[16];
	float real_winkel;
	
	vector<int> terrainLODVector;
	float frustum[6][4];

	awx_vector3f transformedCameraWorldSpace,transformedForwardVectorWorldSpace;
	float rotationMatrixForSpecialEffects[16];
	
	_camera();
	
	void setup();
	
	float calcRotAngle(float distance);
	
	void setCamera();
	
	void setPosition(awx_vector3f x);
	
	int writeToFile(FILE *file);
	int loadFromFile(FILE *file);
};

#endif

