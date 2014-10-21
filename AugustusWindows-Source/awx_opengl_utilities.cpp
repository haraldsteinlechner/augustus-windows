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

#include "awx_opengl_utilities.h"
#include "awx_matrix.h"

extern GLuint loadingTexture;

void awx_check_ogl()
{
	char *extensions = (char*) glGetString(GL_EXTENSIONS);
	char *renderer = (char*) glGetString(GL_RENDERER);
	char *vendor = (char*) glGetString(GL_VENDOR);
	char *version = (char*) glGetString(GL_VERSION);
	
	project.renderer = renderer;
	project.version_string = version;
	
	FILE *EXT = fopen("OpenGL-Information.txt","w+");
	if(EXT==NULL)
	{
		project.error_message = "OpenGL Information k√∂nnen nicht geschrieben werden.";
		project.error_code = 1;
	}
	fprintf(EXT,"WolfEngine 1.0\n\n");
	fprintf(EXT,"\nBenutzte Hardware: %s\n",renderer);
	fprintf(EXT,"OpenGL-Version: %s\n",version);
	fprintf(EXT,"F√ºr diese OpenGL-Implementierung zust√§ndige Firma: %s\n",vendor);
	fprintf(EXT,"\nOpenGL-Extensions:\n");
	for(unsigned int loop=0;loop<strlen(extensions);loop++)
	{
		if(extensions[loop]=='G'&&extensions[loop+1]=='L')
			 fprintf(EXT,"\n%c",extensions[loop]);
		else fprintf(EXT,"%c",extensions[loop]);
	}
	fclose(EXT);
}

/*
 This is SGI sample code taken directly from OpenGL.org:
 http://www.opengl.org/developers/code/features/OGLextensions/OGLextensions.html
 */
int OpenGLExtensionIsSupported(const char* extension) {
  const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *where, *terminator;

  /* Extension names should not have spaces. */
  where = (GLubyte *) strchr(extension, ' ');
  if (where || *extension == '\0')
    return 0;
  extensions = glGetString(GL_EXTENSIONS);
  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  start = extensions;
  for (;;) {
    where = (GLubyte *) strstr((const char *) start, extension);
    if (!where)
      break;
    terminator = where + strlen(extension);
    if (where == start || *(where - 1) == ' ')
      if (*terminator == ' ' || *terminator == '\0')
        return 1;
    start = terminator;
  }
  return 0;
}
	
	awx_text::awx_text()
{
    position_x=0;
    position_y=0;
}

void awx_text::setProperties(float x, float y, char *text, float R, float G, float B)
{
    string=text;
    position_x=x;
    position_y=y;
    color[0]=R;
    color[1]=G;
    color[2]=B;
}

bool awx_text::render()
{  
    if(string==NULL) return false;
    glRasterPos2f(position_x,position_y);
    lenght=(int)strlen(string);
    
    //glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D); 
    
    glColor3fv(color);
    for(loop=0;loop<lenght;loop++)
    {
         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[loop]);
    }
    
    
    return true;
}
    

bool awx_text::set_and_render_text(float x, float y, char *text, float R, float G, float B)
{
    if(text==NULL) return false;
    glRasterPos2f(x,y);
    lenght=(int)strlen(text);
    
    glColor3f(R,G,B);
    for(loop=0;loop<lenght;loop++)
    {
         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,text[loop]);
    }
    
    return true;
}

int awx_ScreenShot()
{
	FILE *statusFile = fopen("Screenshots/data.awx","rb+");
	int statusNumber = 0;
	if(statusFile!=NULL)
	{
		fread(&statusNumber,sizeof(int),1,statusFile);
	
		fclose(statusFile);
		
		statusNumber++;
	} 
	
	statusFile = fopen("Screenshots/data.awx","wb+");
	if(statusFile!=NULL)
	{
		fwrite(&statusNumber,sizeof(int),1,statusFile);
		fclose(statusFile);
	}

  unsigned char *pixels;
  FILE *image;
  
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  pixels = new unsigned char[viewport[2]*viewport[3]*3];

  glReadPixels(0, 0, viewport[2], viewport[3], GL_BGR, 
                                   GL_UNSIGNED_BYTE, pixels);

  char tempstring[50];
  sprintf(tempstring,"Screenshots/screenshot_%i.tga",statusNumber);
  if((image=fopen(tempstring, "wb"))==NULL) return 1;

  unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
  
  unsigned char header[6]={((int)(viewport[2]%256)),
                   ((int)(viewport[2]/256)),
                   ((int)(viewport[3]%256)),
                   ((int)(viewport[3]/256)),24,0};

  // TGA header schreiben
  fwrite(TGAheader, sizeof(unsigned char), 12, image);
  // Header schreiben
  fwrite(header, sizeof(unsigned char), 6, image);

  fwrite(pixels, sizeof(unsigned char), 
                 viewport[2]*viewport[3]*3, image);

  fclose(image);
  delete [] pixels;

  return 0;
}

void awx_stroke_output_simple(GLfloat x, GLfloat y, GLfloat scale,  char *text)
{
	char *p;

	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(scale,scale,scale);
	for (p = text; *p; p++) glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	glPopMatrix();
}

void awx_stroke_output(GLfloat x, GLfloat y, GLfloat scale,  char *text)
{
	if(text == NULL ||strlen(text) <= 0)return;
	char *p;
	
	glLineWidth(2.0);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	glPushMatrix();
		glTranslatef(x, y + 0.003, 0);
		glScalef(scale * 0.00012,0.00012 * scale/1.3,0.00012 * scale);
		glDisable(GL_TEXTURE_2D);
		glColor3f(0,0,0);
		int i = 0;
		for (p = text; *p; p++){
			glTranslatef(10 , 0 , 0);
			if(*p == '\n'){
				i++;
				glLoadIdentity();
				glTranslatef(x, y + 0.003 , 0);
				glScalef(scale * 0.00012,0.00012 * scale/1.3,0.00012 * scale);
				glTranslatef(0 , -170 *i , 0);
			}
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		}
		glColor3f(1,1,1);
		//glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	//glEnable(GL_ALPHA_TEST);
}

void rotateTexture(int ID, float winkel, int size, bool mipmaps, bool compression)
{
	glViewport( 0, 0, size, size );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,1,0,1);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_TEXTURE);
	glRotatef(winkel,0,0,1);
	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D,textur_liste[ID]);
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0);
		glVertex3f(0.0,0.0,0.0);
	glTexCoord2f(1.0,0.0);
		glVertex3f(1.0,0.0,0.0);
	glTexCoord2f(1.0,1.0);
		glVertex3f(1.0,1.0,0.0);
	glTexCoord2f(0.0,1.0);
		glVertex3f(0.0,1.0,0.0);
	glEnd();
	
	glPopMatrix();
		
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	
	
	unsigned char *pixels = new unsigned char[size*size*3+1];
		
	glReadPixels(0,0,size,size,GL_RGB,GL_UNSIGNED_BYTE,pixels);
	
	GLint min,mag;		
	glBindTexture(GL_TEXTURE_2D,textur_liste[ID]);
	glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,&min);
	glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,&mag);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,size,size,GL_RGB,GL_UNSIGNED_BYTE,pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,min);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,mag);
	
	delete pixels;
}

void outputBitmapFont(float x, float y, char *string)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
}

void renderLoadingScreen(char*  val)
{
	float size_x=project.aspect,size_y=project.aspect*0.5;

	glViewport(0,0,project.width,project.height);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
    gluOrtho2D(0.0,project.aspect,0.0,1.0);
	
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
    glLoadIdentity();
	
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0,1.0,1.0,1.0);
	outputBitmapFont(0.0,0.008,val);
	
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,loadingTexture);
	
	glTranslatef(0.0,0.5-size_y*0.5,0.0);
	
	glScalef(size_x,size_y,1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(1.0,0.0,-1.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(1.0,1.0,-1.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(0.0,1.0,0.0);
	glEnd();


	glPopAttrib();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	SDL_GL_SwapBuffers();

}



void debugTextur(int texture,int x, int y)
{
	/*GLubyte *pixels = new GLubyte[256*64*3];
	glGetTexImage(GL_TEXTURE_2D,0,GL_BGR,GL_UNSIGNED_BYTE,pixels);

  FILE *image;
  GLint viewport[4];
  viewport[2]=256;
  viewport[3]=64;

  image=fopen("temp.tga", "wb");

  unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
  
  unsigned char header[6]={((int)(viewport[2]%256)),
                   ((int)(viewport[2]/256)),
                   ((int)(viewport[3]%256)),
                   ((int)(viewport[3]/256)),24,0};

  // TGA header schreiben
  fwrite(TGAheader, sizeof(unsigned char), 12, image);
  // Header schreiben
  fwrite(header, sizeof(unsigned char), 6, image);

  fwrite(pixels, sizeof(unsigned char), 
                 viewport[2]*viewport[3]*3, image);

  fclose(image);
  delete [] pixels;*/
}



void CalcTangentSpaceVectors(const float pos1[3],
                             const float pos2[3],
                             const float pos3[3],
                             const float texCoord1[2],
                             const float texCoord2[2],
                             const float texCoord3[2],
                             awx_vector3f &tangent,
                             awx_vector3f &bitangent,
                             awx_vector3f &normal)
{
    // Given the 3 vertices (position and texture coordinate) of a triangle
    // calculate and return the triangle's tangent space basis vectors:
    // tangent, bitangent, and normal vectors.

    // Create 2 vectors in object space.
    //
    // edge1 is the vector from vertex positions pos1 to pos2.
    // edge2 is the vector from vertex positions pos1 to pos3.
    awx_vector3f edge1(pos2[0] - pos1[0], pos2[1] - pos1[1], pos2[2] - pos1[2]);
    awx_vector3f edge2(pos3[0] - pos1[0], pos3[1] - pos1[1], pos3[2] - pos1[2]);

    edge1 = edge1.normalize();
    edge2 = edge2.normalize();

    // Create 2 vectors in tangent (texture) space that point in the same
    // direction as edge1 and edge2 (in object space).
    //
    // texEdge1 is the vector from texture coordinates texCoord1 to texCoord2.
    // texEdge2 is the vector from texture coordinates texCoord1 to texCoord3.
    awx_vector3f texEdge1(texCoord2[0] - texCoord1[0], texCoord2[1] - texCoord1[1],0);
    awx_vector3f texEdge2(texCoord3[0] - texCoord1[0], texCoord3[1] - texCoord1[1],0);

    texEdge1 = texEdge1.normalize();
    texEdge2 = texEdge2.normalize();

    // These 2 sets of vectors form the following system of equations:
    //
    //  edge1 = (texEdge1.x * tangent) + (texEdge1.y * bitangent)
    //  edge2 = (texEdge2.x * tangent) + (texEdge2.y * bitangent)
    //
    // Using matrix notation this system looks like:
    //
    //  [ edge1 ]     [ texEdge1.x  texEdge1.y ]  [ tangent   ]
    //  [       ]  =  [                        ]  [           ]
    //  [ edge2 ]     [ texEdge2.x  texEdge2.y ]  [ bitangent ]
    //
    // The solution is:
    //
    //  [ tangent   ]        1     [ texEdge2.y  -texEdge1.y ]  [ edge1 ]
    //  [           ]  =  -------  [                         ]  [       ]
    //  [ bitangent ]      det A   [-texEdge2.x   texEdge1.x ]  [ edge2 ]
    //
    //  where:
    //        [ texEdge1.x  texEdge1.y ]
    //    A = [                        ]
    //        [ texEdge2.x  texEdge2.y ]
    //
    //    det A = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x)
    //
    // From this solution the basis vectors are:
    //
    //  tangent   = (1 / det A) * ( texEdge2.y * edge1 - texEdge1.y * edge2)
    //  bitangent = (1 / det A) * (-texEdge2.x * edge1 + texEdge1.x * edge2)
    //  normal    = tangent X bitangent
    //
    // These 3 basis vectors currently form the following matrix:
    //
    //                       [ tangent.x  bitangent.x  normal.x ]
    //  M                  = [ tangent.y  bitangent.y  normal.y ]
    //   tangent->object     [ tangent.z  bitangent.z  normal.z ]
    //
    // This rotation matrix M is used to transform vectors in tangent space into
    // object space.
    //
    // For tangent space bump mapping we want the opposite. We want a rotation
    // matrix that will transform vectors in object space into tangent space.
    //
    // Inverting matrix M will give us the rotation matrix we need. The tangent
    // basis vectors stored in the rotation matrix M may not be orthonormal. So
    // just taking the transpose of the rotation matrix M will not give us the
    // inverse.
    //
    // The following web site explains how to invert a 3x3 square matrix:
    // http://mathworld.wolfram.com/MatrixInverse.html.
    //
    // After the rotation matrix M is inverted the resulting tangent basis
    // vectors will be arranged as follows:
    //
    //                       [ tangent.x    tangent.y    tangent.z   ]
    //  M                 =  [ bitangent.x  bitangent.y  bitangent.z ]
    //   object->tangent     [ normal.x     normal.y     normal.z    ]
    //
    // Each row of this matrix contains the tangent space basis vectors.

    float detA = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x);

    if (closeEnough(detA, 0.0f))
    {
        tangent.set(1.0f, 0.0f, 0.0f);
        bitangent.set(0.0f, 1.0f, 0.0f);
        normal.set(0.0f, 0.0f, 1.0f);
    }
    else
    {
        awx_vector3f t((1.0f / detA) * (texEdge2.y * edge1 - texEdge1.y * edge2));
        awx_vector3f b((1.0f / detA) * (-texEdge2.x * edge1 + texEdge1.x * edge2));

        t = t.normalize();
        b = b.normalize();

        awx_vector3f n(t.crossProduct(b));


		float m[16] = {t.x,t.y,t.z,0,
					   b.x,b.y,b.z,0,
					   n.x,n.y,n.z,0,
					   0,0,0,1};
		
		/*float m[16] = {t.x, b.x, n.x,0,
					   t.y, b.y, n.y,0,
					   t.z, b.z, n.z,0,
						0,0,0,1};*/
						
					   
        /*awx_matrix tbnMatrix(t.x, b.x, n.x,
                             t.y, b.y, n.y,
                             t.z, b.z, n.z);*/
		
		awx_matrix tbnMatrix(m);

        tbnMatrix = tbnMatrix.inverse();

        /*
		oginal
		tangent.set(tbnMatrix[0][0], tbnMatrix[0][1], tbnMatrix[0][2]);
        bitangent.set(tbnMatrix[1][0], tbnMatrix[1][1], tbnMatrix[1][2]);
        normal.set(tbnMatrix[2][0], tbnMatrix[2][1], tbnMatrix[2][2]);*/
		
		/*
		inversed
		tangent.set(tbnMatrix.a[0][0], tbnMatrix.a[1][0], tbnMatrix.a[2][0]);
        bitangent.set(tbnMatrix.a[0][1], tbnMatrix.a[1][1], tbnMatrix.a[2][1]);
        normal.set(tbnMatrix.a[0][2], tbnMatrix.a[1][2], tbnMatrix.a[2][2]);*/
		
		/*
		oginal*/
		tangent.set(tbnMatrix.a[0][0], tbnMatrix.a[0][1], tbnMatrix.a[0][2]);
        bitangent.set(tbnMatrix.a[1][0], tbnMatrix.a[1][1], tbnMatrix.a[1][2]);
        normal.set(tbnMatrix.a[2][0], tbnMatrix.a[2][1], tbnMatrix.a[2][2]);
		
		
        tangent = tangent.normalize();
        bitangent = bitangent.normalize();
        normal = normal.normalize();
    }
}
