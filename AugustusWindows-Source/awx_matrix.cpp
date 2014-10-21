#include "awx_matrix.h"
#define GradRad 57.296

///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   ¬¨¬®¬¨¬© Alphaworx Corporation //  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////

/*						 AWX Matrix-Operationsheader						 */

awx_matrix awx_matrix::rotateX(float alpha){
    awx_matrix x;
    x.a[1][1] = (float)cos(alpha/GradRad);
    x.a[1][2] = (float)sin(alpha/GradRad);
    x.a[2][1] = (float)-sin(alpha/GradRad);
    x.a[2][2] = (float)cos(alpha/GradRad);

    return x;
}

void awx_matrix::rotateX_self(float alpha){
    awx_matrix x;
    x.a[1][1] = (float)cos(alpha/GradRad);
    x.a[1][2] = (float)sin(alpha/GradRad);
    x.a[2][1] = (float)-sin(alpha/GradRad);
    x.a[2][2] = (float)cos(alpha/GradRad);

    setMatrix(product(x));
}

awx_matrix awx_matrix::rotateY(float alpha){
    awx_matrix x;
    x.a[0][0] = (float)cos(alpha/GradRad);
    x.a[0][2] = (float)-sin(alpha/GradRad);
    x.a[2][0] = (float)sin(alpha/GradRad);
    x.a[2][2] = (float)cos(alpha/GradRad);

    return x;
}

void awx_matrix::rotateY_self(float alpha){
    awx_matrix x;
    x.a[0][0] = (float)cos(-alpha/GradRad);
    x.a[0][2] = (float)-sin(-alpha/GradRad);
    x.a[2][0] = (float)sin(-alpha/GradRad);
    x.a[2][2] = (float)cos(-alpha/GradRad);

    setMatrix(product(x));
}

awx_matrix awx_matrix::rotateZ(float alpha){
    awx_matrix x;
    x.a[0][0] = (float)cos(alpha/GradRad);
    x.a[1][0] = (float)sin(alpha/GradRad);
    x.a[0][1] = (float)-sin(alpha/GradRad);
    x.a[1][1] = (float)cos(alpha/GradRad);

    return x;
}

void awx_matrix::rotateZ_self(float alpha){
    awx_matrix x;
    x.a[0][0] = (float)cos(alpha/GradRad);
    x.a[1][0] = (float)sin(alpha/GradRad);
    x.a[0][1] = (float)-sin(alpha/GradRad);
    x.a[1][1] = (float)cos(alpha/GradRad);

    setMatrix(x.product(a));
}

awx_matrix::awx_matrix(){
    for(int x = 0; x < 4;x++){
        for(int y = 0; y < 4;y++){
            a[x][y] = 0.0;
        }
    }
    
    for(int x = 0; x < 4;x++){
        a[x][x] = 1.0;
    }
}

awx_matrix::awx_matrix(float matrix[16]){
    setMatrixf(matrix);
}

void awx_matrix::setMatrix(awx_matrix b){
    for(int i = 0; i < 3;i++){
        for(int u = 0; u < 3;u++){
            a[i][u] = b.a[i][u];
        }
    }
}

void awx_matrix::setMatrix(float modelview[16])
{
    setMatrixf(modelview);
}


void awx_matrix::setMatrixf(float modelview[16]){
    a[0][0] = modelview[0];
    a[0][1] = modelview[4];
    a[0][2] = modelview[8];
    a[0][3] = modelview[12];
    
    a[1][0] = modelview[1];
    a[1][1] = modelview[5];
    a[1][2] = modelview[9];
    a[1][3] = modelview[13];
    
    a[2][0] = modelview[2];
    a[2][1] = modelview[6];
    a[2][2] = modelview[10];
    a[2][3] = modelview[14];
    
    a[3][0] = modelview[3];
    a[3][1] = modelview[7];
    a[3][2] = modelview[11];
    a[3][3] = modelview[15];
}

void awx_matrix::setMatrixd(double modelview[16]){
    a[0][0] = (float) modelview[0];
    a[0][1] =  (float)modelview[4];
    a[0][2] =  (float)modelview[8];
    a[0][3] =  (float)modelview[12];
    
    a[1][0] =  (float)modelview[1];
    a[1][1] =  (float)modelview[5];
    a[1][2] =  (float)modelview[9];
    a[1][3] =  (float)modelview[13];
    
    a[2][0] = (float) modelview[2];
    a[2][1] = (float) modelview[6];
    a[2][2] =  (float)modelview[10];
    a[2][3] =  (float)modelview[14];
    
    a[3][0] = (float) modelview[3];
    a[3][1] =  (float)modelview[7];
    a[3][2] =  (float)modelview[11];
    a[3][3] = (float) modelview[15];
}

awx_matrix awx_matrix::addition(awx_matrix b){
    awx_matrix temp;
    
    for(int u = 0; u  <  4;u++){
        for(int v = 0; v  <  4;v++){
            temp.a[u][v] = a[u][v] + b.a[u][v];
        }
    }
    
    return temp;
}

awx_matrix awx_matrix::product(awx_matrix b){
    awx_matrix returnMatrix;
    
    returnMatrix.a[0][0] = a[0][0] * b.a[0][0] + a[0][1]*b.a[1][0] + a[0][2]*b.a[2][0];
    returnMatrix.a[0][1] = a[0][0] * b.a[0][1] + a[0][1]*b.a[1][1] + a[0][2]*b.a[2][1];
    returnMatrix.a[0][2] = a[0][0] * b.a[0][2] + a[0][1]*b.a[1][2] + a[0][2]*b.a[2][2];
    returnMatrix.a[1][0] = a[1][0] * b.a[0][0] + a[1][1]*b.a[1][0] + a[1][2]*b.a[2][0];
    returnMatrix.a[1][1] = a[1][0] * b.a[0][1] + a[1][1]*b.a[1][1] + a[1][2]*b.a[2][1];
    returnMatrix.a[1][2] = a[1][0] * b.a[0][2] + a[1][1]*b.a[1][2] + a[1][2]*b.a[2][2];
    returnMatrix.a[2][0] = a[2][0] * b.a[0][0] + a[2][1]*b.a[1][0] + a[2][2]*b.a[2][0];
    returnMatrix.a[2][1] = a[2][0] * b.a[0][1] + a[2][1]*b.a[1][1] + a[2][2]*b.a[2][1];
    returnMatrix.a[2][2] = a[2][0] * b.a[0][2] + a[2][1]*b.a[1][2] + a[2][2]*b.a[2][2];
    
    return returnMatrix;
}

awx_matrix awx_matrix::product(float b[4][4]){
    awx_matrix returnMatrix;
    
    returnMatrix.a[0][0] = a[0][0] * b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0];
    returnMatrix.a[0][1] = a[0][0] * b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1];
    returnMatrix.a[0][2] = a[0][0] * b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2];
    returnMatrix.a[1][0] = a[1][0] * b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0];
    returnMatrix.a[1][1] = a[1][0] * b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1];
    returnMatrix.a[1][2] = a[1][0] * b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2];
    returnMatrix.a[2][0] = a[2][0] * b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0];
    returnMatrix.a[2][1] = a[2][0] * b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1];
    returnMatrix.a[2][2] = a[2][0] * b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2];
    
    return returnMatrix;
}

void awx_matrix::product_self(awx_matrix b){
    a[0][0] = a[0][0] * b.a[0][0] + a[0][1]*b.a[1][0] + a[0][2]*b.a[2][0];
    a[0][1] = a[0][0] * b.a[0][1] + a[0][1]*b.a[1][1] + a[0][2]*b.a[2][1];
    a[0][2] = a[0][0] * b.a[0][2] + a[0][1]*b.a[1][2] + a[0][2]*b.a[2][2];
    a[1][0] = a[1][0] * b.a[0][0] + a[1][1]*b.a[1][0] + a[1][2]*b.a[2][0];
    a[1][1] = a[1][0] * b.a[0][1] + a[1][1]*b.a[1][1] + a[1][2]*b.a[2][1];
    a[1][2] = a[1][0] * b.a[0][2] + a[1][1]*b.a[1][2] + a[1][2]*b.a[2][2];
    a[2][0] = a[2][0] * b.a[0][0] + a[2][1]*b.a[1][0] + a[2][2]*b.a[2][0];
    a[2][1] = a[2][0] * b.a[0][1] + a[2][1]*b.a[1][1] + a[2][2]*b.a[2][1];
    a[2][2] = a[2][0] * b.a[0][2] + a[2][1]*b.a[1][2] + a[2][2]*b.a[2][2];
}

awx_vector3f awx_matrix::vectorProduct(awx_vector3f aVector){
    awx_vector3f returnVector;
    
    returnVector.setX(a[0][0]*aVector.getX()+a[0][1]*aVector.getY()+a[0][2]*aVector.getZ());
    returnVector.setY(a[1][0]*aVector.getX()+a[1][1]*aVector.getY()+a[1][2]*aVector.getZ());
    returnVector.setZ(a[2][0]*aVector.getX()+a[2][1]*aVector.getY()+a[2][2]*aVector.getZ());
    
    return returnVector;
    
}

awx_vector3f awx_matrix::vectorProduct4(awx_vector3f aVector){
    awx_vector3f returnVector;
    
    returnVector.setX((a[0][0]*aVector.getX()+a[0][1]*aVector.getY()+a[0][2]*aVector.getZ())+a[0][3]);
    returnVector.setY((a[1][0]*aVector.getX()+a[1][1]*aVector.getY()+a[1][2]*aVector.getZ())+a[1][3]);
    returnVector.setZ((a[2][0]*aVector.getX()+a[2][1]*aVector.getY()+a[2][2]*aVector.getZ())+a[2][3]);
    
    return returnVector;
}


void awx_matrix::rotatef(float alpha , float x , float y , float z){
    awx_matrix mx;
    awx_matrix my;
    awx_matrix mz;
    
    awx_matrix result;
    
    //Gar nicht:
    if(alpha*x != 0 || alpha*y != 0 || alpha*z != 0){
    
        //Nur um X-Achse:
        if(alpha*y == 0 && alpha*z == 0){
            rotateX_self(alpha*x);
        }
    
        //Nur um Y-Achse:
        else if(alpha*x == 0 && alpha*z == 0){
            rotateY_self(alpha*y);
        }
    
        //Nur um Z-Achse:
        else if(alpha*x == 0 && alpha*y == 0){
            rotateZ_self(alpha*z);
        }
    
        //Um mehr als eine Achse:
        else{
            float c = (float)cos(alpha/GradRad);
            float s = (float)sin(alpha/GradRad);
            float one_c = (float)1.0 - c;
        
            float temp = sqrt( x*x + y*y + z*z);
        
            x /= temp;
            y /= temp;
            z /= temp;
        
            float xy = x * y;
            float yz = y * z;
            float zx = z * x;
            float xs = x * s;
            float ys = y * s;
            float zs = z * s;

            result.a[0][0] = (one_c * x * x) + c;
            result.a[0][1] = (one_c * xy) - zs;
            result.a[0][2] = (one_c * zx) + ys;

            result.a[1][0] = (one_c * xy) + zs;
            result.a[1][1] = (one_c * y * y) + c;
            result.a[1][2] = (one_c * yz) - xs;

            result.a[2][0] = (one_c * zx) - ys;
            result.a[2][1] = (one_c * yz) + xs;
            result.a[2][2] = (one_c * z * z) + c;
        
            setMatrix(product(result));
        }
    }
}

void awx_matrix::translatef(awx_vector3f position){			
    awx_vector3f delta;
    delta.set(position.x,position.y,position.z);
    
    delta = vectorProduct(delta);
    
    a[0][3] += delta.getX();
    a[1][3] += delta.getY();
    a[2][3] += delta.getZ();
}

void awx_matrix::scalef(float factor){
    for(int u = 0; u <  3; u++){
        for(int v = 0; v < 3;v++){
            a[u][v] *= factor;
        }
    }
}

void awx_matrix::scalefv(float x , float y , float z){
    awx_matrix scalem;
    
    awx_vector3f scalev(x,y,z);
    
    scalem.a[0][0] = scalev.getX();
    scalem.a[1][1] = scalev.getY();
    scalem.a[2][2] = scalev.getZ();
    
    product_self(scalem);
}

void awx_matrix::getModelview(float modelview[16]){
    modelview[0] = a[0][0];
    modelview[1] = a[1][0];
    modelview[2] = a[2][0];
    modelview[3] = a[3][0];
    modelview[4] = a[0][1];
    modelview[5] = a[1][1];
    modelview[6] = a[2][1];
    modelview[7] = a[3][1];
    modelview[8] = a[0][2];
    modelview[9] = a[1][2];
    modelview[10] = a[2][2];
    modelview[11] = a[3][2];
    modelview[12] = a[0][3];
    modelview[13] = a[1][3];
    modelview[14] = a[2][3];
    modelview[15] = a[3][3];
}

void awx_matrix::plot()
{
    char string[10];
    
        for(int i = 0; i < 4;i++){
            for(int e = 0; e < 4;e++){
            
                if(a[e][i] >= 0){
                    sprintf(string , " %f" , a[i][e]);
                }
                else{
                    sprintf(string , "%f" , a[i][e]);
                }
                cout << string << "   ";
                
            }
            cout << endl;
        }
}

void awx_matrix::loadIdentity(){
    for(int x = 0; x < 4;x++){
        for(int y = 0; y < 4;y++){
            a[x][y] = 0.0;
        }
    }
    
    for(int x = 0; x < 4;x++){
        a[x][x] = 1.0;
    }
}

awx_matrix awx_matrix::transpose()
{
    awx_matrix tranMatrix;
    
    tranMatrix.a[0][0] = a[0][0];
    tranMatrix.a[0][1] = a[1][0];
    tranMatrix.a[0][2] = a[2][0];
    tranMatrix.a[0][3] = a[3][0];
    
    tranMatrix.a[1][0] = a[0][1];
    tranMatrix.a[1][1] = a[1][1];
    tranMatrix.a[1][2] = a[2][1];
    tranMatrix.a[1][3] = a[3][1];
    
    tranMatrix.a[2][0] = a[0][2];
    tranMatrix.a[2][1] = a[1][2];
    tranMatrix.a[2][2] = a[2][2];
    tranMatrix.a[2][3] = a[3][2];
    
    tranMatrix.a[3][0] = a[0][3];
    tranMatrix.a[3][1] = a[1][3];
    tranMatrix.a[3][2] = a[2][3];
    tranMatrix.a[3][3] = a[3][3];
    
    return tranMatrix;
    
}

void awx_matrix::inverse4x4Gauss()
{ 
	double source[4][10];
	
	for(int i=0;i<4;i++)
	{
		for(int u=0;u<4;u++)
		{
			source[i][u]=a[i][u];
		}
	}
	
	double target[4][10];
	
	Inverse(source,target,4);
	
	for(int i=0;i<4;i++)
	{
		for(int u=0;u<4;u++)
		{
			a[i][u]=target[i][u];
		}
	}

}


inline float awx_matrix::determinant()
{
    return (a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]))
        - (a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]))
        + (a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]));
}

awx_matrix awx_matrix::inverse()
{
    // If the inverse doesn't exist for this matrix, then the identity
    // matrix will be returned.

    awx_matrix tmp;
    float d = determinant();

    if (closeEnough(d, 0.0f))
    {
        tmp.loadIdentity();
    }
    else
    {
        d = 1.0f / d;

        tmp.a[0][0] = d * (a[1][1] * a[2][2] - a[1][2] * a[2][1]);
        tmp.a[0][1] = d * (a[0][2] * a[2][1] - a[0][1] * a[2][2]);
        tmp.a[0][2] = d * (a[0][1] * a[1][2] - a[0][2] * a[1][1]);

        tmp.a[1][0] = d * (a[1][2] * a[2][0] - a[1][0] * a[2][2]);
        tmp.a[1][1] = d * (a[0][0] * a[2][2] - a[0][2] * a[2][0]);
        tmp.a[1][2] = d * (a[0][2] * a[1][0] - a[0][0] * a[1][2]);

        tmp.a[2][0] = d * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
        tmp.a[2][1] = d * (a[0][1] * a[2][0] - a[0][0] * a[2][1]);
        tmp.a[2][2] = d * (a[0][0] * a[1][1] - a[0][1] * a[1][0]);
    }

    return tmp;
}

awx_matrix awx_matrix::inverse4x4()
{
    // If the inverse doesn't exist for this matrix, then the identity
    // matrix will be returned.

    awx_matrix tmp;
    float d = determinant();

    if (closeEnough(d, 0.0f))
    {
        tmp.loadIdentity();
    }
    else
    {
        d = 1.0f / d;

        tmp.a[0][0] = d * (a[1][1] * a[2][2] - a[1][2] * a[2][1]);
        tmp.a[0][1] = d * (a[0][2] * a[2][1] - a[0][1] * a[2][2]);
        tmp.a[0][2] = d * (a[0][1] * a[1][2] - a[0][2] * a[1][1]);

        tmp.a[1][0] = d * (a[1][2] * a[2][0] - a[1][0] * a[2][2]);
        tmp.a[1][1] = d * (a[0][0] * a[2][2] - a[0][2] * a[2][0]);
        tmp.a[1][2] = d * (a[0][2] * a[1][0] - a[0][0] * a[1][2]);

        tmp.a[2][0] = d * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
        tmp.a[2][1] = d * (a[0][1] * a[2][0] - a[0][0] * a[2][1]);
        tmp.a[2][2] = d * (a[0][0] * a[1][1] - a[0][1] * a[1][0]);
		
		tmp.a[0][3] = -((a[0][3] * tmp.a[0][0]) + (a[1][3] * tmp.a[1][0]) + (a[2][3] * tmp.a[2][0]))*d;
		tmp.a[1][3] = -((a[0][3] * tmp.a[0][1]) + (a[1][3] * tmp.a[1][1]) + (a[2][3] * tmp.a[2][1]))*d;
		tmp.a[2][3] = -((a[0][3] * tmp.a[0][2]) + (a[1][3] * tmp.a[1][2]) + (a[2][3] * tmp.a[2][2]))*d;
    }

    return tmp;
}


awx_matrix operator + (awx_matrix a , awx_matrix b )
{
    awx_matrix r;

    r =	a.addition(b);

    return r;
}

awx_vector3f operator * (awx_matrix b , awx_vector3f a )
{
    awx_vector3f r;

    r = b.vectorProduct(a);

    return r;
}

awx_vector3f operator * (awx_vector3f a , awx_matrix b)
{
    awx_vector3f r;

    r = b.vectorProduct(a);

    return r;
}

awx_matrix operator * (awx_matrix a , awx_matrix b)
{
    awx_matrix r;

    r = a.product(b);

    return r;
}

ostream & operator<<  (ostream & out, awx_matrix a){
    a.plot();
    return out;
}