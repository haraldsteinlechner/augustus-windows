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


#include "awx_blob.h"

awx_blob::awx_blob()
{
    winkel_y=0;
    blobsizex=4;
    blobsizey=4;
	transOffset=0;
}

void awx_blob::loadBlobs(char* path, float size)
{
//	awx_string temps;
//	temps = workingdirectory + "/blob.tga";
    load_tga(path,currentID,true,true);
    blobtexturID = currentID;
    currentID++;
    blobsizex=size;
    blobsizey=size;
}


void awx_blob::renderBlobs(float winkel_y, awx_vector3f position,int ID)
{
    glPushMatrix();

    glTranslatef(position.x+transOffset,position.y,position.z);
    glRotatef(winkel_y,0.0,1.0,0.0);
    glTranslatef(-blobsizex/2.0,0,-blobsizey/2.0);
//Here 
    glBindTexture(GL_TEXTURE_2D,textur_liste[ID]);

    glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(blobsizex,0.0,0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(blobsizex,0.0,blobsizey);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0,0.0,blobsizey);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
    glEnd();
    
    glPopMatrix();
}

void awx_blob::renderBlobWithTextureObject(float winkel_y, awx_vector3f position,GLuint ID)
{
    glPushMatrix();

    glTranslatef(position.x+transOffset,position.y,position.z);
    glRotatef(winkel_y,0.0,1.0,0.0);
    glTranslatef(-blobsizex/2.0,0,-blobsizey/2.0);
//Here 
    glBindTexture(GL_TEXTURE_2D,ID);

    glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(blobsizex,0.0,0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(blobsizex,0.0,blobsizey);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0,0.0,blobsizey);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
    glEnd();
    
    glPopMatrix();
}

void awx_blob::renderBlobs(float modelview[],int ID)
{
    glPushMatrix();

    glMultMatrixf(modelview);
    glScalef(1.2,1.2,1.2);
    glTranslatef(-blobsizex/2.0,0,-blobsizey/2.0);

    glBindTexture(GL_TEXTURE_2D,textur_liste[ID]);

    glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(blobsizex,0.0,0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(blobsizex,0.0,blobsizey);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0,0.0,blobsizey);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
    glEnd();
    
    glPopMatrix();
}

void awx_blob::renderBlobs(awx_vector3f position,int ID, float size)
{
    float tempx = blobsizex;
    float tempy = blobsizey;
    blobsizex=size;
    blobsizey=size;
    renderBlobs(winkel_y,position,ID);
    blobsizex=tempx;
    blobsizey=tempy;
}

void awx_blob::renderBlobs(awx_vector3f position,int ID, float sizex,float sizey,float winkel)
{
    float tempx = blobsizex;
    float tempy = blobsizey;
    blobsizex=sizex;
    blobsizey=sizey;
    renderBlobs(winkel_y,position,ID);
    blobsizex=tempx;
    blobsizey=tempy;
    winkel_y=winkel;
}

void awx_blob::renderBlobs(awx_vector3f position)
{
    renderBlobs(winkel_y,position,blobtexturID);
}

void awx_blob::setBlob(int i)
{
    blobtexturID=i;
}

void awx_blob::setRange(float x1,float x2,float x3,float x4)
{
	xmin=x1;
	xmax=x2;
	zmin=x3;
	zmax=x4;
}

void awx_blob::setSize(float x,float y)
{
	blobsizex=x;
	blobsizey=y;
}
