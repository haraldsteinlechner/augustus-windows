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



typedef enum
{
	off,
	up,
	down
}weatherStatus;


class fogPlane
{
	public:
	
	GLuint textureObject;
	GLuint list;
	GLfloat translationSpeed;
	awx_vector3f translationDirection;
	GLfloat u,v;
	GLfloat size;
	GLfloat height;
	
	GLfloat s,t,r;
	GLfloat alpha;
	GLenum blendModeSRC,blendModeDST;
	
	
	fogPlane()
	{
		size=300;
		u=1;
		v=1;
		translationDirection = awx_vector3f(1.0,0.0,0.0);
		translationSpeed = 1;
		textureObject = 0;
		height=-6;
		s=0; r=0; t=0;
		alpha=1.0;
	}
	
	void setTexture(char *path)
	{
		textureObject = load_tgaAndReturnObject(path,true,false);
	}
	
	void setTexture(GLuint texObj)
	{
		textureObject = texObj;
	}
	
	void updateSetup(float transpeed, awx_vector3f direction)
	{
		translationSpeed = transpeed;
		translationDirection = direction;
		
	}
	
	void setupRenderOptions(float u, float v, float alpha, float height, GLenum blendSRC, GLenum blendDST)
	{
		this->u=u;
		this->v=v;
		this->alpha=alpha;
		this->height=height;
		
		blendModeSRC = blendSRC;
		blendModeDST = blendDST;
	}
	
	void render()
	{
		glBindTexture(GL_TEXTURE_2D,textureObject);
		
		glBegin(GL_QUADS);
		
		glTexCoord2f(0.0*u,0.0*v);
		glVertex3f(-size,height,-size);
		
		glTexCoord2f(1.0*u,0.0*v);
		glVertex3f(size,height,-size);
		
		glTexCoord2f(1.0*u,1.0*v);
		glVertex3f(size,height,size);
		
		glTexCoord2f(0.0*u,1.0*v);
		glVertex3f(-size,height,size);
		
		glEnd();
	}
	
	void optimize(float offset)
	{
		list = glGenLists(1);
		
		glNewList(list,GL_COMPILE);
		glBlendFunc(blendModeSRC,blendModeDST);
		render();
		glEndList();
	}
	
	void renderOptimized(float globalAlpha=1.0)
	{
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glTranslatef(s,0,r);
		
		
		glColor4f(1.0,1.0,1.0,alpha*globalAlpha);
		
		glCallList(list);
	}
	
	void update(float wind=1.0)
	{
		s+=(translationDirection.x*translationSpeed*wind)/(float)runtime.fps;
		r+=(translationDirection.z*translationSpeed*wind)/(float)runtime.fps;
	}
};


class deepFog
{
	public:
	
	fogPlane planes[4];
	GLfloat alpha;
	awx_shader planeFogShader;
	GLuint samplerLocation;
	weatherStatus status;
	bool simulation;
	
	float farClippingPlane;
	
	float wind;
	
	deepFog()
	{
		simulation=false;
		alpha=0.0;
		wind=1.0;
		
		farClippingPlane = 180;
	}
	
	void setupFog()
	{
		// nebel: GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
		// Wolken: GL_DST_COLOR,GL_SRC_ALPHA);

	
		planes[0].setTexture("Data/specialEffects/wolken.tga");
		planes[0].updateSetup(0.0912,awx_vector3f(1.0,0.0,0.5));
		planes[0].setupRenderOptions(15,15,1.0,-5,GL_DST_COLOR,GL_SRC_ALPHA);
		planes[0].optimize(36);
		
		planes[1].setTexture("Data/specialEffects/fog1.tga");
		planes[1].updateSetup(0.04832,awx_vector3f(-0.5,0.0,-0.5));
		planes[1].setupRenderOptions(5,5,0.25,5,GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		planes[1].optimize(-23);
		
		planes[2].setTexture(planes[1].textureObject);
		planes[2].updateSetup(0.067,awx_vector3f(1.0,0.0,-1.0));
		planes[2].setupRenderOptions(10,10,0.4,0,GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		planes[2].optimize(5);
		
		planes[3].setTexture(planes[1].textureObject);
		planes[3].updateSetup(1.26,awx_vector3f(1.0,0.0,0.5));
		planes[3].setupRenderOptions(5,5,0.2,10,GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		planes[3].optimize(5);
		
		planeFogShader.set("Data/shaders/planeFog.vert","Data/shaders/planeFog.frag");
		samplerLocation = glGetUniformLocationARB(planeFogShader.p,"texture");

	}
	
	void installStates()
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);


		glEnable(GL_TEXTURE_2D);
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(45.0, project.aspect, 10.0, farClippingPlane);
		
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
	}
	
	void deinstallStates()
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	
		glPopAttrib();
	}
	
	void update()
	{
		if(status==up)
		{
			alpha+=0.18144/(float)runtime.fps;
			wind=alpha*1.1;
			if(alpha>=2.5) status=down;
		} else if(status==down)
		{
			alpha-=0.388/(float)runtime.fps;
			wind=alpha*1.1;
			if(alpha<=0.0)
			{
				status=off;
				alpha=0;
				wind=alpha*1.1;
				simulation=false;
			}
		}
	}
	
	void startSimulation()
	{
		simulation=true;
		status=up;
	}
	
	void render()
	{
		if(simulation)
		{
			update();
		}
	
		installStates();
		
		planes[0].renderOptimized();
		planes[0].update();
		
		
		if(alpha>0)
		{			
			for(int i=1;i<4;i++)
			{
				planes[i].renderOptimized(alpha);
				planes[i].update(wind);
			}
		}
		
		deinstallStates();
	}
};