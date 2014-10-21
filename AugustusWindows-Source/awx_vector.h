///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   ¬© Alphaworx Corporation //  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////

#ifndef AWX_VECTOR_H
#define AWX_VECTOR_H

/*							AWX VectorOperationsHeader						 */

#include <math.h>
#include <iostream>

#ifdef _WIN32
	#ifndef isnan
	#define isnan(x) ((x)!=(x))
	#endif
#endif

using namespace std;

class awx_vector3f{
	public:
		float x;
		float y;
		float z;
		
	public:
		
		void set(float x , float y , float z);
		void setX(float x);
		void setY(float y);
		void setZ(float z);
		
		float getX();
		float getY();
		float getZ();
		float getHorizontalLength();
		float getHorizontalLengthXZ();
		float getLength();
		float getAlpha();
		float getBeta();
		
		awx_vector3f normalize();
		awx_vector3f addition(awx_vector3f aVector);
		awx_vector3f substract(awx_vector3f aVector);
		awx_vector3f product(float lambda);
		awx_vector3f divide(float lambda);
		float scalarProduct(awx_vector3f aVector);
		awx_vector3f crossProduct(awx_vector3f aVector);
		
		awx_vector3f();
		awx_vector3f(float x,float y,float z);
		
		bool compareTo(awx_vector3f param);
		void plot(int docLevel);
		void plot();
	
		bool isNan();
};

awx_vector3f operator + (awx_vector3f a , awx_vector3f b);
awx_vector3f operator - (awx_vector3f a , awx_vector3f b);
awx_vector3f operator * (float a, awx_vector3f b);
awx_vector3f operator * (awx_vector3f b , float a);
awx_vector3f operator / (awx_vector3f b , float a);
awx_vector3f operator * (awx_vector3f b , awx_vector3f a );
float operator % (awx_vector3f b , awx_vector3f a );
ostream & operator<<  (ostream & out, awx_vector3f a);



//==============================================================================================================================
//														awx_vector2f
//==============================================================================================================================



class awx_vector2f{
	public:
		float x;
		float y;		
	public:
		
		void set(float x , float y);
		void setX(float x);
		void setY(float y);
		
		float getX();
		float getY();
		float getLength();
		float getAngle();
		
		awx_vector2f normalize();
		awx_vector2f addition(awx_vector2f aVector);
		awx_vector2f substract(awx_vector2f aVector);
		awx_vector2f product(float lambda);
		awx_vector2f divide(float lambda);
		float scalarProduct(awx_vector2f aVector);
		awx_vector2f crossProduct();
		
		awx_vector2f();
		awx_vector2f(float x,float y);
		
		bool compareTo(awx_vector3f param);
		awx_vector2f &awx_vector2f::operator=(float value[2]);
		void plot();

};

awx_vector2f operator + (awx_vector2f a , awx_vector2f b);
awx_vector2f operator - (awx_vector2f a , awx_vector2f b);
awx_vector2f operator * (float a, awx_vector2f b);
awx_vector2f operator * (awx_vector2f b , float a);
awx_vector2f operator / (awx_vector2f b , float a);
float operator % (awx_vector2f b , awx_vector2f a );
ostream & operator<<  (ostream & out, awx_vector2f a);

extern bool closeEnough(float f1, float f2);
extern bool closeEnough(float f1, float f2, float EPSILON);

#endif
