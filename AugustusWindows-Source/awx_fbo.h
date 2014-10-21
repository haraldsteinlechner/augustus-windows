#define CHECK_FRAMEBUFFER_STATUS() \
	{\
	GLenum status; \
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);\
	switch(status)\
	{\
	case GL_FRAMEBUFFER_COMPLETE_EXT:\
	break;\
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:\
	cout << "FBO unsupported.." << endl;\
	break;\
	default: \
	cout << "FBO programming error." << endl;\
	}\
	}

class awx_fbo
{
public:

	GLuint framebuffer, depth_rb, texture;
	int width,height;

	awx_fbo(): framebuffer(0), depth_rb(0), texture(0)
	{

	}

	void init(int width, int height)
	{
		this->width=width;
		this->height=height;

		glGenFramebuffersEXT(1,&framebuffer);
		glGenRenderbuffersEXT(1,&depth_rb);
		glGenTextures(1,&texture);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);

		glBindTexture(GL_TEXTURE_2D,texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);

		CHECK_FRAMEBUFFER_STATUS();

		
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	}

	void renderIntoMe()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);

		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0,0,width,height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	void stopRenderingToMe()
	{
		glPopAttrib();


		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	void useMe()
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void dontUseMe()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void dealloc()
	{
		glDeleteFramebuffersEXT(1,&framebuffer);
		glDeleteRenderbuffersEXT(1,&depth_rb);
		glDeleteTextures(1,&texture);
	}
};