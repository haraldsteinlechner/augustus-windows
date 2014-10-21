awx_fbo *mainMenuFBO,*refractionFBO;
awx_shader *reflectShader,*projectShader,*simpleReflectShader;
GLuint localReflectionTexture=0;
GLuint normalMapTexture=0;
GLuint timerLocation=0;
GLuint skyReflectionTexLocation=0;
GLuint refractionTexture=0;
GLuint waterHeightLoc=0;
GLuint simpleReflectTexture0=0;
GLuint simpleReflectTextureWaterLoc=0;
float waterHeight=-9.179;

awx_tgaAnimation *normalmapAnimation;


void initUltraRender()
{
	mainMenuFBO = new awx_fbo;

	mainMenuFBO->init(1024,1024);

	refractionFBO = new awx_fbo;

	refractionFBO->init(512,512);

	reflectShader = new awx_shader;
	reflectShader->set("Data/water/reflect.vert","Data/water/reflect.frag");
	reflectShader->install();
	waterHeightLoc = glGetUniformLocationARB(reflectShader->p,"waterHeight");
	reflectShader->deinstall();

	projectShader = new awx_shader;
	projectShader->set("Data/water/project.vert","Data/water/project.frag");

	projectShader->install();
	localReflectionTexture = glGetUniformLocationARB(projectShader->p,"localReflection");
	normalMapTexture = glGetUniformLocationARB(projectShader->p,"normalMap");
	timerLocation = glGetUniformLocationARB(projectShader->p,"timer");
	skyReflectionTexLocation = glGetUniformLocationARB(projectShader->p,"skyRelction");
	refractionTexture = glGetUniformLocationARB(projectShader->p,"refraction");
	projectShader->deinstall();

	simpleReflectShader = new awx_shader;
	simpleReflectShader->set("Data/water/reflectSimple.vert","Data/water/reflectSimple.frag");
	simpleReflectShader->install();
	simpleReflectTexture0 = glGetUniformLocationARB(simpleReflectShader->p,"texture0");
	simpleReflectTextureWaterLoc = glGetUniformLocationARB(simpleReflectShader->p,"waterHeight");
	simpleReflectShader->deinstall();

	normalmapAnimation = new awx_tgaAnimation;
	normalmapAnimation->loadTGASeries("Data/water/Water",1,21);
}

void deallocUltraRender()
{
	mainMenuFBO->dealloc();
	refractionFBO->dealloc();

	delete reflectShader;
	delete projectShader;
	delete simpleReflectShader;

	normalmapAnimation->dealloc();
	delete normalmapAnimation;

	delete mainMenuFBO;
	delete refractionFBO;
}

int plotTextureToHD(GLuint texture)
{
  unsigned char *pixels;
  FILE *image;

  float w,h;
  int width,height;
  
  glBindTexture(GL_TEXTURE_2D,texture);
  glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

  width=(int)w;
  height=(int)h;

  pixels = new unsigned char[width*height*3];



  glGetTexImage(GL_TEXTURE_2D,0,GL_BGR,GL_UNSIGNED_BYTE,pixels);

  if((image=fopen("debug.tga", "wb"))==NULL) return 1;

  unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
  
  unsigned char header[6]={((int)(width%256)),
                   ((int)(width/256)),
                   ((int)(height%256)),
                   ((int)(height/256)),24,0};

  // TGA header schreiben
  fwrite(TGAheader, sizeof(unsigned char), 12, image);
  // Header schreiben
  fwrite(header, sizeof(unsigned char), 6, image);

  fwrite(pixels, sizeof(unsigned char), 
                 width*height*3, image);

  fclose(image);
  delete [] pixels;

  return 0;
}

void preRenderUltra()
{
	mainMenuFBO->renderIntoMe();
	
	reflectShader->install();

		GLuint AWXTexture0 = glGetUniformLocationARB(reflectShader->p,"texture0");
		GLuint AWXTexture1 = glGetUniformLocationARB(reflectShader->p,"texture1");
		GLuint AWXTexture2 = glGetUniformLocationARB(reflectShader->p,"texture2");
		GLuint AWXTexture3 = glGetUniformLocationARB(reflectShader->p,"texture3");
		GLuint AWXAlphaAttrib = glGetAttribLocationARB(reflectShader->p,"alphaData");
		GLuint AWXFogOfWarAttrib = glGetAttribLocationARB(reflectShader->p,"fogOfWar");

		glUniform1f(waterHeightLoc,waterHeight);

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[currentLevel->terrain.IDs[0]+currentLevel->terrain.textureTable[0].ID]);
		glUniform1iARB(AWXTexture0,currentLevel->terrain.textureTable[0].ID);
		
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[currentLevel->terrain.IDs[0]+currentLevel->terrain.textureTable[1].ID]);
		glUniform1iARB(AWXTexture1,currentLevel->terrain.textureTable[1].ID);
		
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[currentLevel->terrain.IDs[0]+currentLevel->terrain.textureTable[2].ID]);
		glUniform1iARB(AWXTexture2,currentLevel->terrain.textureTable[2].ID);
	
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[currentLevel->terrain.IDs[0]+currentLevel->terrain.textureTable[3].ID]);
		glUniform1iARB(AWXTexture3,currentLevel->terrain.textureTable[3].ID);
	
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableVertexAttribArrayARB(AWXAlphaAttrib);
		glEnableVertexAttribArrayARB(AWXFogOfWarAttrib);
        
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,currentLevel->terrain.vboVertex);
        glVertexPointer(3,GL_FLOAT,0,NULL);
		
            
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,currentLevel->terrain.vboNormal);
        glNormalPointer(GL_FLOAT,0,NULL);
            
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,currentLevel->terrain.vboTexCoord);
        glTexCoordPointer(2,GL_FLOAT,0,NULL);
		
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,currentLevel->terrain.vboTextureWeights);
        glVertexAttribPointerARB(AWXAlphaAttrib,4,GL_FLOAT,0,0,NULL);

		
		glPushMatrix();
		glScalef(1.0,-1.0,1.0);
		glTranslatef(0,-waterHeight*2,0);
        glDrawArrays(GL_TRIANGLES,0,currentLevel->terrain.part[0].planezahl*3);
		glPopMatrix();

            
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableVertexAttribArrayARB(AWXAlphaAttrib);
		glDisableVertexAttribArrayARB(AWXFogOfWarAttrib);
		
		for(int i=4;i>=1;i--)
		{
			glActiveTexture(GL_TEXTURE0+i);
			glDisable(GL_TEXTURE_2D);
		}
		
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

	reflectShader->deinstall();



	//simpleReflectShader->install();

	//glUniform1i(simpleReflectTexture0,0);
	//glUniform1f(simpleReflectTextureWaterLoc,waterHeight);
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_CLIP_PLANE0);
	GLdouble equ[] = {0.0,-1.0,0.0,waterHeight};
	glClipPlane(GL_CLIP_PLANE0,equ);
	
	glPushMatrix();
	glScalef(1.0,-1.0,1.0);
	glTranslatef(0,-waterHeight*2,0);
	//currentLevel->renderLevelObjectsForReflection();
	//glutSolidCube(1);
	currentLevel->renderLevelObjectsForReflection();
	glPopMatrix();


	glPopAttrib();


	//simpleReflectShader->deinstall();




	mainMenuFBO->stopRenderingToMe();


	refractionFBO->renderIntoMe();


	currentLevel->render();


	refractionFBO->stopRenderingToMe();
}

extern int menuTexID;

void renderUltraRender()
{
	currentLevel->render();

	currentLevel->renderLevelObjectsForReflection();

	preRenderUltra();

	glDisable(GL_ALPHA_TEST);

	//plotTextureToHD(mainMenuFBO->texture);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


	projectShader->install();

	static float move=0;
	move+=1.0/(float)runtime.fps;
	glUniform2f(timerLocation,0.0,move);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	normalmapAnimation->useCurrentTexture();
	glUniform1i(normalMapTexture,1);

	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,textur_liste[menuTexID]);
	glUniform1i(skyReflectionTexLocation,2);

	glActiveTexture(GL_TEXTURE3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,refractionFBO->texture);
	glUniform1i(refractionTexture,3);

	
	normalmapAnimation->play(runtime.fps);
	normalmapAnimation->setFPS(15.0);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,mainMenuFBO->texture);
	glUniform1i(localReflectionTexture,0);


	float mv[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,mv);

	awx_matrix modelviewMatrix = mv;
	modelviewMatrix.inverse4x4Gauss();

	float proj[16];
	glGetFloatv(GL_PROJECTION_MATRIX,proj);

	float inverseModelView[16];
	modelviewMatrix.getModelview(inverseModelView);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.5,0.5,0.5);
	glScalef(0.5,0.5,0.5);
	glMultMatrixf(proj);
	glMultMatrixf(mv);
	glMultMatrixf(inverseModelView);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	GLfloat size=1000;

	/*glBegin(GL_QUADS);

	glTexCoord2f(0.0,0.0);
	glVertex3f(-size,waterHeight,-size);

	glTexCoord2f(1.0,0.0);
	glVertex3f(size,waterHeight,-size);

	glTexCoord2f(1.0,1.0);
	glVertex3f(size,waterHeight,size);

	glTexCoord2f(0.0,1.0);
	glVertex3f(-size,waterHeight,size);

	glEnd();*/

	currentLevel->data[0].renderAtOrigin();

	glPopMatrix();



	glMatrixMode(GL_TEXTURE);
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);


	projectShader->deinstall();

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE3);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);

}