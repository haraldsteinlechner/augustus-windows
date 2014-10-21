class awxSkyBox
{
	private:
	
	float size;
	
	GLuint myList;
	GLuint myTextureSet[6];
	
	public:
	
	awxSkyBox()
	{
		size=9000;
	}
	
	void initSkyBox()
	{
		myList = glGenLists(1);
		
		if(1==1)
		{
			myTextureSet[0] = load_tgaAndReturnObject("Data/sky/sunset_north.tga",true,true);
			myTextureSet[1] = load_tgaAndReturnObject("Data/sky/sunset_south.tga",true,true);
			myTextureSet[2] = load_tgaAndReturnObject("Data/sky/sunset_west.tga",true,true);		
			myTextureSet[3] = load_tgaAndReturnObject("Data/sky/sunset_east.tga",true,true);
		
			myTextureSet[4] = load_tgaAndReturnObject("Data/sky/sunset_up.tga",true,true);
			myTextureSet[5] = load_tgaAndReturnObject("Data/sky/sunset_down.tga",true,true);
		}
		
		else
		{
		
			myTextureSet[0] = load_tgaAndReturnObject("Data/sky/reef_north.tga",true,true);
			myTextureSet[1] = load_tgaAndReturnObject("Data/sky/reef_south.tga",true,true);
			myTextureSet[2] = load_tgaAndReturnObject("Data/sky/reef_west.tga",true,true);		
			myTextureSet[3] = load_tgaAndReturnObject("Data/sky/reef_east.tga",true,true);
		
			myTextureSet[4] = load_tgaAndReturnObject("Data/sky/reef_up.tga",true,true);
			myTextureSet[5] = load_tgaAndReturnObject("Data/sky/reef_down.tga",true,true);
		}
		
		float sizehalf=size*0.5;
		
		glNewList(myList,GL_COMPILE);
		
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
			
			glDisable(GL_LIGHTING);
			
			glColor4f(1.0,1.0,1.0,1.0);
		
			// -z
			glBindTexture(GL_TEXTURE_2D,myTextureSet[0]);
			
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			
			glBegin(GL_QUADS);
		
				// -z
				glTexCoord2f(0.0,0.0);
				glVertex3f(-sizehalf,0,-sizehalf);
		
				glTexCoord2f(1.0,0.0);
				glVertex3f(sizehalf,0,-sizehalf);
		
				glTexCoord2f(1.0,1.0);
				glVertex3f(sizehalf,size,-sizehalf);
		
				glTexCoord2f(0.0,1.0);
				glVertex3f(-sizehalf,size,-sizehalf);
				
			glEnd();
				
			// +z
			glBindTexture(GL_TEXTURE_2D,myTextureSet[1]);
			
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			
			glBegin(GL_QUADS);
				
				glTexCoord2f(1.0,0.0);
				glVertex3f(-sizehalf,0,sizehalf);
		
				glTexCoord2f(0.0,0.0);
				glVertex3f(sizehalf,0,sizehalf);
		
				glTexCoord2f(0.0,1.0);
				glVertex3f(sizehalf,size,sizehalf);
		
				glTexCoord2f(1.0,1.0);
				glVertex3f(-sizehalf,size,sizehalf);
		
			glEnd();
			
			// -x
			glBindTexture(GL_TEXTURE_2D,myTextureSet[2]);
			
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			
			glBegin(GL_QUADS);
				
				glTexCoord2f(1.0,0.0);
				glVertex3f(-sizehalf,0,-sizehalf);
		
				glTexCoord2f(0.0,0.0);
				glVertex3f(-sizehalf,0,sizehalf);
		
				glTexCoord2f(0.0,1.0);
				glVertex3f(-sizehalf,size,sizehalf);
		
				glTexCoord2f(1.0,1.0);
				glVertex3f(-sizehalf,size,-sizehalf);
		
			glEnd();
			
			// +x
			glBindTexture(GL_TEXTURE_2D,myTextureSet[3]);
			
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			
			glBegin(GL_QUADS);
				
				glTexCoord2f(0.0,0.0);
				glVertex3f(sizehalf,0,-sizehalf);
		
				glTexCoord2f(1.0,0.0);
				glVertex3f(sizehalf,0,sizehalf);
		
				glTexCoord2f(1.0,1.0);
				glVertex3f(sizehalf,size,sizehalf);
		
				glTexCoord2f(0.0,1.0);
				glVertex3f(sizehalf,size,-sizehalf);
		
			glEnd();
			
			
			// oben
			
			glBindTexture(GL_TEXTURE_2D,myTextureSet[4]);
			
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			
			glBegin(GL_QUADS);
				
				glTexCoord2f(1.0,1.0);
				glVertex3f(-sizehalf,size,-sizehalf);
		
				glTexCoord2f(1.0,0.0);
				glVertex3f(-sizehalf,size,sizehalf);
		
				glTexCoord2f(0.0,0.0);
				glVertex3f(sizehalf,size,sizehalf);
		
				glTexCoord2f(0.0,1.0);
				glVertex3f(sizehalf,size,-sizehalf);
				
									
			glEnd();

			
			glBindTexture(GL_TEXTURE_2D,myTextureSet[5]);
			
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			
			glBegin(GL_QUADS);
				
				glTexCoord2f(1.0,0.0);
				glVertex3f(-sizehalf,0,-sizehalf);
		
				glTexCoord2f(1.0,1.0);
				glVertex3f(-sizehalf,0,sizehalf);
		
				glTexCoord2f(0.0,1.0);
				glVertex3f(sizehalf,0,sizehalf);
		
				glTexCoord2f(0.0,0.0);
				glVertex3f(sizehalf,0,-sizehalf);
		
			glEnd();			
			
			
			glPopAttrib();
		
		
		glEndList();
	}
	
	void killSkyBox()
	{
		glDeleteLists(myList,1);
		cout << "Deallocating Skyboxtextures" << endl;
		glDeleteTextures(6,myTextureSet);
	}
	
	~awxSkyBox()
	{
		killSkyBox();
	}
	
	void render()
	{		
		glTranslatef(0,-4500,0);
		glCallList(myList);
	}
};