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

#include "awx_aax.h"

aax::aax()
{
	for(loop=0;loop<16;loop++) modelview[loop] = 0;
	for(loop=0;loop<16;loop++) modelview2[loop] = 0;
	
	modelview2[0]=1;
	modelview2[5]=1;
	modelview2[10]=1;
	
	modelview[0]=1;
	modelview[5]=1;
	modelview[10]=1;
	modelview[15]=1;
	
	for(loop=0;loop<16;loop++) resultmatrix[loop] = 0;
	
	resultmatrix[0]=1;
	resultmatrix[5]=1;
	resultmatrix[10]=1;
	
	for(loop=0;loop<16;loop++) resultmatrix2[loop] = 0;
	
	resultmatrix2[0]=1;
	resultmatrix2[5]=1;
	resultmatrix2[10]=1;
	
	position.x=0;
	position.y=0;
	position.z=0;
	
	hasShadowVolume = false;
}


void aax::printTransformations()
{
	for(int i=0;i<number_of_objects;i++)
	{
		printf("\nTransformation:");
		printf("		%.6f ",objects[i].modelview[0]);
		printf("%.6f ",objects[i].modelview[1]);
		printf("%.6f ",objects[i].modelview[2]);
		
		printf("\nRotation:");
		printf("			%.6f ",objects[i].modelview[3]);
		printf("%.6f ",objects[i].modelview[4]);
		printf("%.6f ",objects[i].modelview[5]);
		
		printf("\nScale:");
		printf("			%.6f ",objects[i].modelview[6]);
		printf("%.6f ",objects[i].modelview[7]);
		printf("%.6f ",objects[i].modelview[8]);
	
	}
}

int aax::openFile(const char* filename)
{
	cout << "Open AAX with: " << filename << endl;
	FILE *input = fopen(filename,"rb");
	
	if(input==NULL)
	{
		project.error_code=5;
		sprintf(project.error_message,"Datei (AAX): %s konnte nicht ge‚Äö√†√∂‚Äö√†√áffnet werden.",filename);
		project.getError();
		return 0;
	}
	
	cout << "Starte Lesevorgang von: " << filename << endl << endl;

	int version;
	awx_fread(&version,sizeof(version),1,input);
	
	if(version<10) return-1;
	
	awx_fread(&number_of_objects,sizeof(number_of_objects),1,input);
	objects = new transformations[number_of_objects];
	data = new apx[number_of_objects];
	
	for(int loop=0;loop<number_of_objects;loop++)
	{
		fread(&objects[loop].modelview,sizeof(float),16,input);
			
		if(getCPUArchitecture()==AWXx86)
			for(int u=0;u<16;u++) reverseByteOrder(&objects[loop].modelview[u],sizeof(float),1);
		
		
		awx_string tempstring = filename;
		awx_string tempstring2 = tempstring.stringByDeletingLastPathComponent();
		char name[50];
		sprintf(name,"part%i.apx",loop);
		tempstring = tempstring2.stringByAppendingPathComponent(*(new awx_string(name)));


		data[loop].load_apx(tempstring.cString());

		for(int i=0;i<16;i++)
		{
			data[loop].modelview[i] = objects[loop].modelview[i];
		}
		
		
		// Herausfinden ob diese textur schon geladen wurde
		GLuint loadedTexture=0;
		for(int u=0;u<=loop;u++)
		{
			if(u==loop) continue;
			if(strcmp(data[loop].texture_table[0].pfad,data[u].texture_table[0].pfad)==0) 
			{
				loadedTexture = data[u].texture;
				break;
			}
		}
		
		data[loop].fillVRAM(awx_vector3f(10.0,10.0,-10.0),hasShadowVolume,rot_y,loadedTexture);
	}
	
	return 0;
}

void aax::posGetter()
{
}

int aax::render()
{
	glPushMatrix();

	glMultMatrixf(modelview);

	//glTranslatef(x,y,z);

	//glRotatef(rotation,rot_x,rot_y,rot_z);

	for(u=0;u<number_of_objects;u++)
	{
		data[u].render();
	}
	
	glPopMatrix();
	
	// GetCoords
	//position = getPositionOfVector(watch);
	
	//posGetter();
	
	return 0;
}

void aax::renderBlank()
{
	glDisable(GL_LIGHTING);
	glColor4f(1.0,1.0,1.0,0.8);
	for(u=0;u<number_of_objects;u++)
	{
		data[u].render();
	}

	glEnable(GL_LIGHTING);
}

int aax::playAll()
{
	glPushMatrix();

	//glMultMatrixf(modelview);

	glTranslatef(x,y,z);

	//glRotatef(rotation,rot_x,rot_y,rot_z);
	
	for(u=0;u<number_of_objects;u++)
	{
		data[u].play(data[u].current_frame);
	}
	
	glPopMatrix();
	
	return 0;
	
}


void aax::loadIdentity()
{
	*modelview = *identity;
}

void aax::translatef(float x, float y, float z)
{
	glPushMatrix();
	glLoadMatrixf(modelview);
	glTranslatef(x,y,z);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();
}

void aax::rotate_um_mittelpunktf(float winkel, float x, float y, float z)
{
	glPushMatrix();
	glLoadMatrixf(modelview);
	glRotatef(winkel,x,y,z);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();
}

void aax::rotate_um_achse_worldf(float winkel, float x, float y, float z, float x2, float y2, float z2)
{
	float modelview2[16];
	
	glPushMatrix();
	//glLoadMatrixf(modelview);
	glLoadIdentity();
	glTranslatef(x,y,z);
	glRotatef(winkel,x2,y2,z2);
	glTranslatef(-x,-y,-z);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview2);
	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview2);
	glLoadMatrixf(modelview2);
	glMultMatrixf(modelview);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	glPopMatrix();
}


void aax::rotate_um_achsef(float winkel, float x, float y, float z, float x2, float y2, float z2)
{
	glPushMatrix();
	glLoadIdentity();
	glLoadMatrixf(modelview);
	glTranslatef(x,y,z);
	glRotatef(winkel,x2,y2,z2);
	glTranslatef(-x,-y,-z);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();
}


void aax::translate_to_worldf(float x, float y, float z)
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x,y,z);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview2);
	glPopMatrix();
}
	

void aax::rotate_um_achse_absf(float winkel, float x, float y, float z, float x2, float y2, float z2)
{
	glPushMatrix();
	glLoadMatrixf(modelview);
	glTranslatef(x,y,z);
	glRotatef(winkel,x2-x,y2-y,z2-z);
	glTranslatef(-x,-y,-z);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();
}

void aax::setPositionf(float x, float y, float z)
{
	glPushMatrix();
	loadIdentity();
	glTranslatef(x,y,z);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();
}

void aax::translatefv(awx_vector3f vec)
{
	glPushMatrix();
	glLoadMatrixf(modelview);
	glTranslatef(vec.getX(),vec.getY(),vec.getZ());
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();
}

void aax::rotate_um_mittelpunktfv(float winkel, awx_vector3f vec)
{
	glPushMatrix();
	glLoadMatrixf(modelview);
	glRotatef(winkel,vec.getX(),vec.getY(),vec.getZ());
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();
}

void aax::rotate_um_achsefv(float winkel, awx_vector3f vec1, awx_vector3f vec2)
{
	glPushMatrix();
	glLoadMatrixf(modelview);
	glTranslatef(vec1.getX(),vec1.getY(),vec1.getZ());
	glRotatef(winkel,vec2.getX(),vec2.getY(),vec2.getZ());
	glTranslatef(-vec1.getX(),-vec1.getY(),-vec1.getZ());
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();    
}

void aax::rotate_um_achse_absfv(float winkel, awx_vector3f vec1, awx_vector3f vec2)
{
	glPushMatrix();
	glLoadMatrixf(modelview);
	glTranslatef(vec1.getX(),vec1.getY(),vec1.getZ());
	glRotatef(winkel,vec2.getX()-vec1.getX(),vec2.getY()-vec1.getY(),vec2.getZ()-vec1.getZ());
	glTranslatef(-vec1.getX(),-vec1.getY(),-vec1.getZ());
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();
}

void aax::setPositionfv(awx_vector3f vec1)
{
	glPushMatrix();
	loadIdentity();
	glTranslatef(vec1.getX(),vec1.getY(),vec1.getZ());
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glPopMatrix();
}

awx_vector3f aax::getPositionOfVector(awx_vector3f vec)
{
	awx_matrix matrix;
	matrix.setMatrix(modelview);
	return matrix.vectorProduct4(vec);
}

awx_vector3f aax::getPositionOfVector2(awx_vector3f vec)
{
	awx_matrix matrix;
	matrix.setMatrix(modelview);
	return matrix.vectorProduct(vec);
}


awx_vector3f aax::getPositionOfVector_res(awx_vector3f vec)
{
	awx_matrix matrix;
	matrix.setMatrix(resultmatrix2);
	return matrix.vectorProduct4(vec);
}

awx_vector3f aax::getPositionOfVector_ress(awx_vector3f vec)
{
	awx_matrix matrix;
	matrix.setMatrix(resultmatrix);
	return matrix.vectorProduct4(vec);
}