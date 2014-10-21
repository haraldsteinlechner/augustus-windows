#ifndef AWX_PARTICLESYSTEM
#define AWX_PARTICLESYSTEM

#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_vector.h"
#include "awx_matrix.h"

#include "awx_random.h"
#include "awx_dictionary.h"

class awx_particleSystem;

#include "awx_particleSystemHandler.h"

class particleColor
{
	public:
	
	float r,g,b,a;
	
	particleColor(float r,float g,float b,float a)
	{
		this->r=r;
		this->g=g;
		this->b=b;
		this->a=a;
	}
	
	particleColor()
	{
	}
	
	particleColor operator-(particleColor a)
	{
		particleColor r;

		r.r=this->r-a.r;
		r.g=this->g-a.g;
		r.b=this->b-a.b;
		r.a=this->a-a.a;
		
		/*r.r=a.r-this->r;
		r.g=a.g-this->g;
		r.b=a.b-this->b;
		r.a=a.a-this->a;*/
		
		return r;
	}
	
	particleColor operator+(particleColor a)
	{
		particleColor r;

		/*r.r=this->r-a.r;
		r.g=this->g-a.g;
		r.b=this->b-a.b;
		r.a=this->a-a.a;*/
		
		r.r=a.r+this->r;
		r.g=a.g+this->g;
		r.b=a.b+this->b;
		r.a=a.a+this->a;
		
		return r;
	}
};

typedef enum
{
	AWXDirectional,
	AWXOmmniDirectional
} AWXParticleSystemMode;

typedef enum
{
	AWXTimeDie,
	AWXFadeDie
} AWXParticleDie;

class awx_particle
{
	public:
	
	awx_vector3f position;
	awx_vector3f velocity;
	awx_vector3f transPosition;
	
	particleColor color;
	
	awx_vector2f size;
	
	float lifetime,age;
	GLuint myList;
	
	void render(awx_matrix &matrix)
	{
		glColor4f(color.r,color.g,color.b,color.a);

		transPosition = matrix.vectorProduct(position);	
		
		glPushMatrix();
		
		glTranslatef(transPosition.x-size.x*0.5f,transPosition.y-size.y*0.5f,transPosition.z);
		glScalef(size.x,size.y,1.0f);

		glCallList(myList);
		
		
		glPopMatrix();
	}
	
	awx_particle()
	{
		position = awx_vector3f(0.0,0.0,0.0);
		velocity = awx_vector3f(0.0,1.0,0.0);
		color = particleColor(1.0,1.0,1.0,1.0);
		size = awx_vector2f(1.0,1.0);
		
		age=0;
	}
};

typedef enum
{
	AWXSpherical,
	AWXCylindrical,
	AWXPointSprite
} AWXParticleRenderMode;

class awx_particleSystem
{
	public:
	
	int version;
	
	vector<awx_particle> particles;
	awx_vector3f position;
	
	awx_vector3f gravity;
	awx_vector2f minSize, maxSize;
	awx_vector3f minVel, maxVel;
	awx_vector3f plane;
	float		 emitCountMax;
	float		 emitCountMin;
	float	     minLifeTime;
	float		 maxLifeTime;
	bool		 proportionality;
	awx_vector2f sizeDelta;
	particleColor startColor, endColor;
	AWXParticleDie dieModus;
	int			 shadeModus;
	int			 emmitance;
	float		 constAlphaData;
	
	AWXParticleRenderMode renderMode;
	
	int maxParticles;
	
	GLenum blendModeSRC;
	GLenum blendModeDST;
	
	GLuint texture;
	string texturePath;
	//awx_shader particleShader;
	awxRandom randomGenerator;
	
	awx_matrix subMatrixTransformation;
	GLuint myList;
	
	float emitScaleFactor;
	bool isVisible;
	
	// textur
	
	awx_particleSystem();
	
	void render();
	void emit();
	bool update();
	awx_particle createParticle();
	void generateList();
	void init();
	void init(string path);
	void updateColor();
	void loadNewTexture(string newPath);
	string serialize();
	int writeToFile(string filename);
	int loadFromFile(string filename);
	void reloadTexture();
	
	private:
	
	particleColor colorDelta;
};



#endif