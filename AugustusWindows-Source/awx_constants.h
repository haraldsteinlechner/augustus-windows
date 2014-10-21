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

#ifndef AWX_CONSTANTS_H
#define AWX_CONSTANTS_H

#include "includeOpenGLAndSDL.h"

#include "awx_vector.h"

#define SektorenX 80
#define SektorenZ 80

#define AWXNull 12345678
#define AWXFalse 1234000
#define AWXTrue 4321000
#define GradRad 57.296
#define awx_g 9.81

#define AWX_AGGRESSIVE 0
#define AWX_DEFENSIVE 1
#define AWX_PASSIVE 2

#define AWX_SQUARE 0
#define AWX_TRIANGLE 1
#define AWX_NONE 2

void awx_shutdown(int code);
bool amInInterface();

int awx_createGame(int argc,char** argv);
void awx_resetScene();

#define RANDOM_FLOAT (((float)rand())/RAND_MAX)

#endif

