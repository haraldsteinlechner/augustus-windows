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

/*						 AWX Matrix-Operationsheader						 */

#include "awx_vector.h"

#ifndef AWX_MATRIX_H
#define AWX_MATRIX_H

#include "coolMatrixStuff.h"

class awx_matrix {
	
	public:

		float a[4][4];
		
		awx_matrix();
		awx_matrix(float matrix[16]);
		
		void setMatrix(awx_matrix b);
		void setMatrix(float modelview[16]);
		void setMatrixf(float modelview[16]);
		void setMatrixd(double modelview[16]);
		awx_matrix addition(awx_matrix b);

		awx_matrix product(awx_matrix b);
		awx_matrix product(float b[4][4]);
		void product_self(awx_matrix b);
		
		awx_vector3f vectorProduct(awx_vector3f aVector);
		awx_vector3f vectorProduct4(awx_vector3f aVector);
		
		void rotatef(float alpha , float x , float y , float z);
		void translatef(awx_vector3f position);
		void scalef(float factor);
		void scalefv(float x , float y , float z);
		
		//KÃ¶nnen sehr unkoordiniertes Verhalten verursachen:
		awx_matrix rotateX(float alpha);
		void rotateX_self(float alpha);
		awx_matrix rotateY(float alpha);
		void rotateY_self(float alpha);
		awx_matrix rotateZ(float alpha);
		void rotateZ_self(float alpha);
		
		void getModelview(float modelview[16]);
		void plot();

		void loadIdentity();
		awx_matrix transpose();
		
		float determinant();
		awx_matrix inverse();
		awx_matrix inverse4x4();
		void inverse4x4Gauss();
};

awx_matrix operator + (awx_matrix a , awx_matrix b );
awx_vector3f operator * (awx_matrix b , awx_vector3f a );
awx_vector3f operator * (awx_vector3f a , awx_matrix b);
awx_matrix operator * (awx_matrix a , awx_matrix b);
ostream & operator<<  (ostream & out, awx_matrix a);

#endif
