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


#ifndef AWX_TRIANGLE_H
#define AWX_TRIANGLE_H

#include "awx_vector.h"
#include "awx_matrix.h"

class awx_triangle{

	public:
		awx_vector3f p1;
		awx_vector3f p2;
		awx_vector3f p3;
		float matrix[16];
		int data;
		
		bool triangleSeen;				// Dreick schon mal gesehen
		float timeSinceLastView;		// Zeit seitem Dreick gesehen wurde. wenn Zeit grš§er x ist, sieht man einheiten nicht
		bool allwaysVisible;			// Wenn true ist wird dreicek nicht nach bestimmter zeit unsichtbar
		
	public:
	
		awx_triangle();
	
		/**** Set-Funktionen ****/
		
			void setP1f(float x , float y , float z);
			
			void setP2f(float x , float y , float z);
			void setP3f(float x , float y , float z);
		
			void setP1fv(awx_vector3f p1n);
			void setP2fv(awx_vector3f p2n);
		
			void setP3fv(awx_vector3f p3n);
			void setfv(awx_vector3f p1 , awx_vector3f p2 , awx_vector3f p3);
			
			void setf(float x1 , float y1 , float z1,
					  float x2 , float y2 , float z2,
					  float x3 , float y3 , float z3);
		
		/**** Get-Funktionen ****/
		
			awx_vector3f getP1();
		
			awx_vector3f getP2();
		
			awx_vector3f getP3();
			
			float getXRotation(char* type);
			
			float getYRotation(char* type);
			
			float getZRotation(char* type);
									
		/**** Spezielle Funktionen ****/
		
			void calc2PointRotationMatrix(awx_matrix modelmatrix);
			void calc4PointRotationMatrix(awx_matrix modelmatrix);
			
			void calcLineRotationMatrix(awx_matrix modelview,awx_vector3f dif);
			
			void multMatrix();
	
};

#endif