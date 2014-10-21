#include "awx_vector.h"

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

/*							AWX VectorOperationsHeader						 */


void awx_vector3f::set(float x , float y , float z){
    setX(x);
    setY(y);
    setZ(z);
}
void awx_vector3f::setX(float x){
    this->x = x;
}

void awx_vector3f::setY(float y){
    this->y = y;
}

void awx_vector3f::setZ(float z){
    this->z = z;
}

float awx_vector3f::getX(){
    return x;
}

float awx_vector3f::getY(){
    return y;
}

float awx_vector3f::getZ(){
    return z;
}

float awx_vector3f::getHorizontalLength(){
    return sqrt( pow(x,2) + pow(y,2));
}

float awx_vector3f::getHorizontalLengthXZ(){
    return sqrt( pow(x,2) + pow(z,2));
}

float awx_vector3f::getLength(){
    return sqrt( pow(x,2) + pow(y,2) + pow(z,2) );
}

awx_vector3f awx_vector3f::normalize()
{
    awx_vector3f temp;
    temp.x = x/getLength();
    temp.y = y/getLength();
    temp.z = z/getLength();

    return temp;
}
    

float awx_vector3f::getAlpha(){
    if(y > 0){
        return (float)57.29577951*atan(x/y);
    }
    else if(y < 0){
        return (float)57.29577951*atan(x/y)+180;
    }
    
    else if(x== 0 && y ==  0){
        return 0;
    }
    
    else{
        return 90;
    }
}

float awx_vector3f::getBeta(){
    if(getHorizontalLength() > 0){
        return (float)57.29577951*atan(z/getHorizontalLength());
    }
    else if(getHorizontalLength() <= 0){
        return (float)90;
    }
    
    else if(z == 0){
        return 0;
    }
    else return 0;
}

void awx_vector3f::plot(int docLevel){
    if(docLevel == 0){
        printf("(%f,%f,%f)\n",x,y,z);
    }
    else if(docLevel == 1){
        printf("(%f,%f,%f)\n",x,y,z);
        printf("(%f<%f<%f)\n",getLength(),getAlpha(),getBeta());
    }
}

void awx_vector3f::plot()
{
	plot(0);
}

awx_vector3f awx_vector3f::addition(awx_vector3f aVector){
    awx_vector3f resultVector;
    
    resultVector.setX(x + aVector.getX());
    resultVector.setY(y + aVector.getY());
    resultVector.setZ(z + aVector.getZ());
    
    return resultVector;
}

awx_vector3f awx_vector3f::substract(awx_vector3f aVector){
    awx_vector3f resultVector;
    
    resultVector.setX(x - aVector.getX());
    resultVector.setY(y - aVector.getY());
    resultVector.setZ(z - aVector.getZ());
    
    return resultVector;
}

awx_vector3f awx_vector3f::product(float lambda){
    awx_vector3f resultVector;
    
    resultVector.setX(x*lambda);
    resultVector.setY(y*lambda);
    resultVector.setZ(z*lambda);
    
    return resultVector;
}

awx_vector3f awx_vector3f::divide(float lambda){
    awx_vector3f resultVector;
    
    if(lambda != 0){
        resultVector.setX(x/lambda);
        resultVector.setY(y/lambda);
        resultVector.setZ(z/lambda);
    
        return resultVector;
    }
    else{
        cout << "Vector-Division by 0!" << endl;
        resultVector.setX(x);
        resultVector.setY(y);
        resultVector.setZ(z);
        
        return resultVector;
    }
}

float awx_vector3f::scalarProduct(awx_vector3f aVector){
    return x*aVector.getX() + y*aVector.getY() + z*aVector.getZ();
}

awx_vector3f awx_vector3f::crossProduct(awx_vector3f aVector){
    awx_vector3f resultVector;
    
    resultVector.setX(y*aVector.getZ() - z*aVector.getY());
    resultVector.setY(-(x*aVector.getZ() - z*aVector.getX()));
    resultVector.setZ(x*aVector.getY() - y*aVector.getX());
    
    return resultVector;
}

awx_vector3f::awx_vector3f(){
    x=0;
    y=0;
    z=0;
}

bool awx_vector3f::isNan()
{
	if(isnan(x)||isnan(y)||isnan(z)) return true;
	return false;
}

awx_vector3f::awx_vector3f(float x,float y,float z){
    this->x=x;
    this->y=y;
    this->z=z;
}

bool awx_vector3f::compareTo(awx_vector3f param)
{
    if(param.x==x&&param.y==y&&param.z==z) return true;
    else return false;
}

awx_vector3f operator + (awx_vector3f a , awx_vector3f b)
{
	awx_vector3f r;

	r=a.addition(b);
	return r;
}

awx_vector3f operator - (awx_vector3f a , awx_vector3f b)
{
	awx_vector3f r;

	r=a.substract(b);
	return r;
}

awx_vector3f operator * (float a, awx_vector3f b)
{
	awx_vector3f r;

	r=b.product(a);
	return r;
}

awx_vector3f operator * (awx_vector3f b , float a)
{
	awx_vector3f r;

	r=b.product(a);
	return r;
}


awx_vector3f operator / (awx_vector3f b , float a)
{
	awx_vector3f r;

	r = b.divide(a);

	return r;
}

awx_vector3f operator * (awx_vector3f b , awx_vector3f a )
{
	awx_vector3f r;

	r = b.crossProduct(a);

	return r;
}

float operator % (awx_vector3f b , awx_vector3f a )
{
	float r;

	r = b.scalarProduct(a);

	return r;
}

ostream & operator<<  (ostream & out, awx_vector3f a){
	a.plot(0);
	return out;
}




//==============================================================================================================================
//														awx_vector2f
//==============================================================================================================================






void awx_vector2f::set(float x , float y){
    setX(x);
    setY(y);
}
void awx_vector2f::setX(float x){
    this->x = x;
}

void awx_vector2f::setY(float y){
    this->y = y;
}

float awx_vector2f::getX(){
    return x;
}

float awx_vector2f::getY(){
    return y;
}

float awx_vector2f::getLength(){
    return sqrt( pow(x,2) + pow(y,2) );
}

awx_vector2f awx_vector2f::normalize()
{
    awx_vector2f temp;
    temp.x = x/getLength();
    temp.y = y/getLength();

    return temp;
}
    

float awx_vector2f::getAngle(){
    if(y > 0){
        return (float)57.29577951*atan(x/y);
    }
    else if(y < 0){
        return (float)57.29577951*atan(x/y)+180;
    }
    
    else if(x== 0 && y ==  0){
        return 0;
    }
    
    else{
        return 90;
    }
}

void awx_vector2f::plot(){
        printf("(%f,%f)\n",x,y);
}

awx_vector2f awx_vector2f::addition(awx_vector2f aVector){
    awx_vector2f resultVector;
    
    resultVector.setX(x + aVector.getX());
    resultVector.setY(y + aVector.getY());
    
    return resultVector;
}

awx_vector2f awx_vector2f::substract(awx_vector2f aVector){
    awx_vector2f resultVector;
    
    resultVector.setX(x - aVector.getX());
    resultVector.setY(y - aVector.getY());
    
    return resultVector;
}

awx_vector2f awx_vector2f::product(float lambda){
    awx_vector2f resultVector;
    
    resultVector.setX(x*lambda);
    resultVector.setY(y*lambda);
    
    return resultVector;
}

awx_vector2f awx_vector2f::divide(float lambda){
    awx_vector2f resultVector;
    
    if(lambda != 0){
        resultVector.setX(x/lambda);
        resultVector.setY(y/lambda);
    
        return resultVector;
    }
    else{
        cout << "Vector-Division by 0!" << endl;
        resultVector.setX(x);
        resultVector.setY(y);
        
        return resultVector;
    }
}

float awx_vector2f::scalarProduct(awx_vector2f aVector){
    return x*aVector.getX() + y*aVector.getY();
}

awx_vector2f awx_vector2f::crossProduct(){
    awx_vector2f resultVector;
    
    resultVector.setX(-y);
    resultVector.setY(x);
    
    return resultVector;
}

awx_vector2f::awx_vector2f(){
    x=0;
    y=0;
}

awx_vector2f::awx_vector2f(float x,float y){
    this->x=x;
    this->y=y;
}

bool awx_vector2f::compareTo(awx_vector3f param)
{
    if(param.x == x && param.y == y) return true;
    else return false;
}



awx_vector2f &awx_vector2f::operator=(float value[2])
{
	set(value[0] , value[1]);
	return *this;
}

awx_vector2f operator + (awx_vector2f a , awx_vector2f b)
{
	awx_vector2f r;

	r=a.addition(b);
	return r;
}

awx_vector2f operator - (awx_vector2f a , awx_vector2f b)
{
	awx_vector2f r;

	r=a.substract(b);
	return r;
}

awx_vector2f operator * (float a, awx_vector2f b)
{
	awx_vector2f r;

	r=b.product(a);
	return r;
}

awx_vector2f operator * (awx_vector2f b , float a)
{
	awx_vector2f r;

	r=b.product(a);
	return r;
}


awx_vector2f operator / (awx_vector2f b , float a)
{
	awx_vector2f r;

	r = b.divide(a);

	return r;
}

float operator % (awx_vector2f b , awx_vector2f a )
{
	float r;

	r = b.scalarProduct(a);

	return r;
}

ostream & operator<<  (ostream & out, awx_vector2f a){
	a.plot();
	return out;
}

bool closeEnough(float f1, float f2)
{
	float EPSILON = 0.01;
	// Determines whether the two floating-point values f1 and f2 are
	// close enough together that they can be considered equal.

	return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
}

bool closeEnough(float f1, float f2, float EPSILON)
{
	// Determines whether the two floating-point values f1 and f2 are
	// close enough together that they can be considered equal.

	return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
}

