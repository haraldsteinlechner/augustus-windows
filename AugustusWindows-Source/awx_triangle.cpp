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

#include "awx_triangle.h"

#include "awx_vector.h"
#include "awx_matrix.h"

#include "includeOpenGLAndSDL.h"


awx_triangle::awx_triangle()
{
	for(int loop=0;loop<=15;loop++)
	{
		matrix[loop]=0;
	}

	matrix[0]=1;
	matrix[5]=1;
	matrix[10]=1;
	matrix[15]=1;
	
	data=-1;
	triangleSeen=false;		
	timeSinceLastView=-1;
	allwaysVisible=false;
}

/**** Set-Funktionen ****/

void awx_triangle::setP1f(float x , float y , float z)
{
    p1.setX(x);
    p1.setY(y);
    p1.setZ(z);
}

void awx_triangle::setP2f(float x , float y , float z)
{
    p2.setX(x);
    p2.setY(y);
    p2.setZ(z);
}

void awx_triangle::setP3f(float x , float y , float z)
{
    p3.setX(x);
    p3.setY(y);
    p3.setZ(z);
}

void awx_triangle::setP1fv(awx_vector3f p1n)
{
    p1 = p1n;
}

void awx_triangle::setP2fv(awx_vector3f p2n)
{
    p2 = p2n;
}

void awx_triangle::setP3fv(awx_vector3f p3n)
{
    p3 = p3n;
}

void awx_triangle::setfv(awx_vector3f p1 , awx_vector3f p2 , awx_vector3f p3)
{
    setP1fv(p1);
    setP2fv(p2);
    setP3fv(p3);
}

void awx_triangle::setf(float x1 , float y1 , float z1,
          float x2 , float y2 , float z2,
          float x3 , float y3 , float z3)
          
{
    setP1f(x1 , y1 , z1);
    setP2f(x2 , y2 , z2);
    setP3f(x3 , y3 , z3);
}

/**** Get-Funktionen ****/

awx_vector3f awx_triangle::getP1()
{
    return p1;
}

awx_vector3f awx_triangle::getP2()
{
    return p1;
}

awx_vector3f awx_triangle::getP3()
{
    return p1;
}

float awx_triangle::getXRotation(char* type)
{
    if(type == "rad"){
        return (float)-asin((float)matrix[9]);
    }
    else{
        return (float)-asin((float)matrix[9])*(float)57.296;
    }
}

float awx_triangle::getYRotation(char* type)
{
    if(type == "rad"){
        return (float)asin((float)matrix[8]);
    }
    else{
        return (float)asin((float)matrix[8])*(float)57.296;
    }
}

float awx_triangle::getZRotation(char* type)
{
    if(type == "rad"){
        return - (float)asin((float)matrix[4]);
    }
    else{
        return - (float)asin((float)matrix[4])*(float)57.296;
    }
}
                        
/**** Spezielle Funktionen ****/

void awx_triangle::calc2PointRotationMatrix(awx_matrix modelmatrix)
{
        awx_vector3f u = p2.substract(p1);
        awx_vector3f v = p3.substract(p1);
        
        awx_vector3f w = u.crossProduct(v);
        modelmatrix = modelmatrix.transpose();
        w = modelmatrix.vectorProduct(w);

        awx_vector3f x1;
        x1.setX(1);
    
        awx_vector3f z1 = x1.crossProduct(w);
        awx_vector3f y1 = z1.crossProduct(x1);
    
        matrix[0] = x1.getX()/x1.getLength();
        matrix[1] = x1.getY()/x1.getLength();
        matrix[2] = x1.getZ()/x1.getLength();

        matrix[4] = y1.getX()/y1.getLength();
        matrix[5] = y1.getY()/y1.getLength();
        matrix[6] = y1.getZ()/y1.getLength();

        matrix[8] = z1.getX()/z1.getLength();
        matrix[9] = z1.getY()/z1.getLength();
        matrix[10] = z1.getZ()/z1.getLength();
}

void awx_triangle::calc4PointRotationMatrix(awx_matrix modelmatrix)
{
    //No totaler TOPFEN: (nit hernemmen)
        awx_vector3f u = p2.substract(p1);
        awx_vector3f v = p3.substract(p1);
        
        awx_vector3f w = u.crossProduct(v);
        modelmatrix = modelmatrix.transpose();
        w = modelmatrix.vectorProduct(w);
        u = modelmatrix.vectorProduct(u);
        v = modelmatrix.vectorProduct(v);

        awx_vector3f x1 = u.divide(u.getLength());
        x1.setZ(0);

        awx_vector3f z1 = v.divide(v.getLength());
        z1.setX(0);
        
        if(z1.getZ() < 0){
            z1 = z1.product(-1);
        }
        
        if(x1.getZ() < 0){
            x1 = x1.product(-1);
        }
        
        awx_vector3f y1 = w;
    
        matrix[0] = x1.getX()/x1.getLength();
        matrix[1] = x1.getY()/x1.getLength();
        matrix[2] = x1.getZ()/x1.getLength();

        matrix[4] = y1.getX()/y1.getLength();
        matrix[5] = y1.getY()/y1.getLength();
        matrix[6] = y1.getZ()/y1.getLength();

        matrix[8] = z1.getX()/z1.getLength();
        matrix[9] = z1.getY()/z1.getLength();
        matrix[10] = z1.getZ()/z1.getLength();
}

void awx_triangle::calcLineRotationMatrix(awx_matrix modelview,awx_vector3f dif)
{
        modelview = modelview.transpose();
        dif = modelview.vectorProduct(dif);
    
        awx_vector3f x1;
        awx_vector3f y1;
        awx_vector3f z1;
    
        x1.setX(1);
    
        z1 = dif.divide(dif.getLength());
        y1 = z1.crossProduct(x1);
    
        matrix[0] = x1.getX()/x1.getLength();
        matrix[1] = x1.getY()/x1.getLength();
        matrix[2] = x1.getZ()/x1.getLength();

        matrix[4] = y1.getX()/y1.getLength();
        matrix[5] = y1.getY()/y1.getLength();
        matrix[6] = y1.getZ()/y1.getLength();

        matrix[8] = z1.getX()/z1.getLength();
        matrix[9] = z1.getY()/z1.getLength();
        matrix[10] = z1.getZ()/z1.getLength();
    
}

void awx_triangle::multMatrix(){
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    //calcRotationMatrix();
    glMultMatrixf(matrix);
    

    glBegin(GL_LINES);
        glColor3f(1,1,1);
        glVertex3f(0,0,0);
        glColor3f(1,0,0);
        glVertex3f(1,0,0);
    glEnd();
    
    glBegin(GL_LINES);
        glColor3f(1,1,1);
        glVertex3f(0,0,0);
        glColor3f(0,1,0);
        glVertex3f(0,1,0);
    glEnd();
    
    glBegin(GL_LINES);
        glColor3f(1,1,1);
        glVertex3f(0,0,0);
        glColor3f(0,0,1);
        glVertex3f(0,0,1);
    glEnd();
    
        awx_matrix test;
        test.setMatrix(matrix);
        
        test.plot();
}