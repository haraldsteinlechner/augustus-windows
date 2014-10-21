extern string floatToString(float f);

extern string intToString(int f);

extern string awxVectorToString(awx_vector3f &f);

class awx_tgaAnimation
{
	public:
	
	GLuint *textures;
	float currentFrame;
	int anzahl;
	int currentFrameFloor;
	int internalFPS;
	
	awx_tgaAnimation()
	{
		currentFrameFloor=0;
		currentFrame=0;
		anzahl=0;
		internalFPS=30;
	}
	
	void loadTGASeries(string name, int start, int end)
	{
		textures = new GLuint[end-start];
		int count=0;
		anzahl=end-start;
		for(int i=start;i<end;i++)
		{
			textures[count] = load_tgaAndReturnObject((char*) (name+intToString(i)+string(".tga")).c_str(),true,true);
			count++;
		}
	}
	
	void play(float fps)
	{
		currentFrame+=1.0/(fps/internalFPS);
		
		currentFrameFloor = int(floor(currentFrame));
		if(currentFrameFloor>=anzahl-1)
		{
			currentFrame=0;
			currentFrameFloor=0;
		}
	}
	
	void useCurrentTexture()
	{
		glBindTexture(GL_TEXTURE_2D,textures[currentFrameFloor]);
	}
	
	void dealloc()
	{
		for(int i=0;i<anzahl;i++)
		{
			glDeleteTextures(1,&textures[i]);
		}
		delete[] textures;
	}

	void setFPS(float u)
	{
		internalFPS=u;
	}
};