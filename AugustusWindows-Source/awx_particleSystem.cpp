#include "awx_particleSystem.h"

#include "awx_tga.h"

class Project;
extern Project project;

extern awx_particleSystemHandler *particleSystemHandler;

awx_particleSystem::awx_particleSystem()
{
	version=1;
	
	//vector<particle> particles;
	position = awx_vector3f(0.0f, -0.60f, 0.0f);
	
	gravity = awx_vector3f(0.0,1.4,0.0);
	minSize = awx_vector2f(0.5f, 0.5f);
	maxSize = awx_vector2f(0.8f, 0.8f);
	sizeDelta = awx_vector2f(-0.5,-0.1);
	proportionality=true;
	minVel = awx_vector3f(-1.0f, -1.0f, -1.0f);
	maxVel = awx_vector3f(1.0f, 1.0f, 1.0f);
	startColor = particleColor(1.0,1.0,0.3,0.8);
	endColor = particleColor(1.0,0.0,0.0,0.0);
	colorDelta = endColor-startColor;

	emitCountMax = 2;
	emitCountMin = 1;
	maxParticles=500;
	minLifeTime=3;
	maxLifeTime=4;
	emitScaleFactor=1.0;
	
	plane = awx_vector3f(0.0,0.0,0.0);
	constAlphaData = -1;
	
	
	dieModus = AWXFadeDie; //AWXTimeDie AWXFadeDie
	
	shadeModus=0;
	emmitance=0;
	
	blendModeSRC = GL_SRC_ALPHA;
	blendModeDST = GL_ONE;
	
	renderMode = AWXSpherical;
	
	texture = 0;
	
	texturePath = "NOPATHSET";
	
	isVisible=true;
	
	randomGenerator.initialize();
	
	if(particleSystemHandler==NULL)
	{
		particleSystemHandler = new awx_particleSystemHandler();
	}
	particleSystemHandler->addParticleSystem(this);
}

void awx_particleSystem::init()
{
	texture = load_tgaAndReturnObject((char*)texturePath.c_str(),true,true);
	
	generateList();
}

void awx_particleSystem::init(string path)
{
	texturePath=path;
	texture = load_tgaAndReturnObject((char*)texturePath.c_str(),true,true);
	
	generateList();
}

void awx_particleSystem::loadNewTexture(string newPath)
{
	glDeleteTextures(1,&texture);
	texture = load_tgaAndReturnObject((char*)newPath.c_str(),true,true);
	texturePath=newPath;
	cout << "texPath=" << texturePath << endl;
}


void awx_particleSystem::updateColor()
{
	colorDelta = endColor-startColor;
}

void awx_particleSystem::generateList()
{
	myList = glGenLists(1);

	glNewList(myList,GL_COMPILE);
	
		glBegin(GL_QUADS);
		
		//glVertexAttrib3f(AWXPositionAttribute,position.x,position.y,position.z);
		
		glTexCoord2f(0.0,0.0);
		glVertex3f(0,0,0);
		
		glTexCoord2f(0.0,1.0);
		glVertex3f(0.0,1.0,0.0);
		
		glTexCoord2f(1.0,1.0);
		glVertex3f(1.0,1.0,0.0);
		
		glTexCoord2f(1.0,0.0);
		glVertex3f(1.0,0.0,0.0);
		
		glEnd();
	
	glEndList();
}


void awx_particleSystem::render()
{
	//glBindTexture(GL_TEXTURE_2D,texture);
	//cout << particles.size() << endl;	

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_CULL_FACE);
	
	
	if(renderMode==AWXSpherical)
	{
	
		float f[16];
		glGetFloatv(GL_MODELVIEW_MATRIX,f);
		glDepthMask(GL_FALSE);
	
		subMatrixTransformation.setMatrixd(project.mvmatrix);

	
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++) 
				if(i==j) f[i*4+j]=1;
				else f[i*4+j]=0;
	
		glLoadMatrixf(f);	

	
		glEnable(GL_BLEND);
		glBlendFunc(blendModeSRC,blendModeDST);
	
	
		if(shadeModus==0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,texture);
		} else glDisable(GL_TEXTURE_2D);
	
		glPushMatrix();

		for(int i=0;i<particles.size();i++)
		{
			particles[i].render(subMatrixTransformation);
		}
	
		glPopMatrix();
	}
	else if(renderMode==AWXCylindrical)
	{
		
		float f[16];
		glGetFloatv(GL_MODELVIEW_MATRIX,f);
		glDepthMask(GL_FALSE);
	
		subMatrixTransformation.setMatrix(f);

		
		for(int i=0;i<3;i+=2)
			for(int j=0;j<3;j++) 
				if(i==j) f[i*4+j]=1;
				else f[i*4+j]=0;
			
	
		glLoadMatrixf(f);	

	
		glEnable(GL_BLEND);
		glBlendFunc(blendModeSRC,blendModeDST);
	
	
		if(shadeModus==0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,texture);
		} else glDisable(GL_TEXTURE_2D);
	
		glPushMatrix();

		for(int i=0;i<particles.size();i++)
		{
			particles[i].render(subMatrixTransformation);
		}
	
		glPopMatrix();
	}
	else
	{
	}
	
	//emit();
	//update();
	
	glDepthMask(GL_TRUE);
}

void awx_particleSystem::emit()
{
	if(!isVisible) return;
	
	awx_particle pushParticle;

	int numberOfNewParticles = randomGenerator.next(emitCountMin*emitScaleFactor,emitCountMax*emitScaleFactor);
	
	for(int i=0;i<numberOfNewParticles;i++)
	{
		if(particles.size()>maxParticles) 
		{
			particles.erase(particles.begin());
			break;
		}
		particles.push_back(createParticle());
	}
}

awx_particle awx_particleSystem::createParticle()
{
	awx_particle theParticle;
	
	theParticle.velocity = awx_vector3f(randomGenerator.next(minVel.x,maxVel.x),randomGenerator.next(minVel.y,maxVel.y),randomGenerator.next(minVel.z,maxVel.z));
	if(emmitance==1)
	{
		theParticle.position = position + awx_vector3f(randomGenerator.next(-plane.x,plane.x),randomGenerator.next(-plane.y,plane.y),randomGenerator.next(-plane.z,plane.z));
	}
	else theParticle.position = position;
	theParticle.color = startColor;
	if(proportionality) 
	{
		float z;
		theParticle.size = awx_vector2f(z = randomGenerator.next(minSize.x,maxSize.x),z);
	}
	else theParticle.size = awx_vector2f(randomGenerator.next(minSize.x,maxSize.x),randomGenerator.next(minSize.y,maxSize.y));
	
	theParticle.lifetime = randomGenerator.next(minLifeTime,maxLifeTime);
	theParticle.myList=myList;
	
	return theParticle;
} 

bool awx_particleSystem::update()
{
	float t=0.050;
	float f=3;
	float r=0.2;
	float beschleunigung=8;
	awx_vector3f gr;

	if(dieModus==AWXFadeDie)
	{
		for(int i=0;i<particles.size();i++)
		{
			if(particles[i].color.a<=0)
			{
				particles.erase(particles.begin(),particles.begin()+i);
				if(i>=1) i--;
				continue;
			}
			float beschl = -(particles[i].age*particles[i].age)*0.5;
			//gr = awx_vector3f(beschl+gravity.x
			particles[i].position=particles[i].position + (((gravity*beschl) + particles[i].velocity)*t);
			particles[i].age+=t;
			//cout << "sdjffj=" <<particles[i].color.b  << endl;			//
			particles[i].color.r += (colorDelta.r/minLifeTime)*t*f;
			particles[i].color.g += (colorDelta.g/minLifeTime)*t*f;
			particles[i].color.b += (colorDelta.b/(double)minLifeTime)*t*f;
			particles[i].color.a += (colorDelta.a/minLifeTime)*t*f;
			
			particles[i].size.x+=sizeDelta.x*t*r;
			particles[i].size.y+=sizeDelta.y*t*r;
			//cout << particles[i].color.b  << endl;
		}
	} else
	{
		for(int i=0;i<particles.size();i++)
		{
			if(particles[i].age>particles[i].lifetime) 
			{
				particles.erase(particles.begin(),particles.begin()+i);
				if(i>=1) i--;
				continue;
			}
			float beschl = -(particles[i].age*particles[i].age)*0.5;
			//gr = awx_vector3f(beschl+gravity.x
			particles[i].position=particles[i].position + (((gravity*beschl) + particles[i].velocity)*t);
			particles[i].age+=t;
			//cout << "sdjffj=" <<particles[i].color.b  << endl;			//
			particles[i].color.r += (colorDelta.r/minLifeTime)*t*f;
			particles[i].color.g += (colorDelta.g/minLifeTime)*t*f;
			particles[i].color.b += (colorDelta.b/(double)minLifeTime)*t*f;
			particles[i].color.a += constAlphaData*t;
			
			particles[i].size.x+=sizeDelta.x*t*r;
			particles[i].size.y+=sizeDelta.y*t*r;
		}
	}
	
	return true;
}

// UNSAFE
char ret[50];

const char* toString(float a)
{	
	sprintf(ret,"%.2f",a);
	
	return ret;
}

const char* toString(int a)
{	
	sprintf(ret,"%i",a);
	
	return ret;
}

const char* toString(awx_vector3f &x)
{
	sprintf(ret,"vec3: %.3f,%.3f,%.3f",x.x,x.y,x.z);
	
	return ret;
}

const char* toString(awx_vector2f &x)
{
	sprintf(ret,"vec3: %.3f,%.3f",x.x,x.y);
	
	return ret;
}

string awx_particleSystem::serialize()
{
	const int anzahl=28;
	string realRet;

	string ret[anzahl];

	ret[0] =  string("AWXParticleSystem V") + (toString(version));
	
	ret[1] = string("\n\nminLifeTime: ") + (toString(minLifeTime));
	ret[2] = string("\nmaxLifeTime: ") + (toString(maxLifeTime));
	ret[3] = string("\nmaxLifeTime: ") + (toString(maxLifeTime));
	ret[4] = string("\nMaxParticles: ") + (toString(maxParticles));
	
	ret[5] = string("\n\nemmitCountMax: ") + (toString(emitCountMax));
	ret[6] = string("\nemmitCountMin: ") + (toString(emitCountMin));

	ret[7] = string("\n\nminLifeTime: ") + (toString(minLifeTime));
	ret[8] = string("\nmaxLifeTime: ") + (toString(maxLifeTime));
	
	ret[9] = string("\n\nProportionality: ") + (toString(proportionality));
	ret[10] = string("\nEmmitance: ") + (toString(emmitance));
	ret[11] = string("\nDieConditional: ") + (toString(dieModus));
	ret[12] = string("\nRender-Mode: ") + (toString(renderMode));
	ret[13] = string("\nDieConditional: ") + (toString(dieModus));
	
	ret[14] = string("\n\nGravity: ") + (toString(gravity));
	ret[15] = string("\nPosition: ") + (toString(position));
	ret[16] = string("\nCubic: ") + (toString(plane));
	ret[17] = string("\nPosition: ") + (toString(position));
	ret[18] = string("\nMinVel: ") + (toString(minVel));
	ret[19] = string("\nMaxVel: ") + (toString(maxVel));
	ret[20] = string("\nMinSize: ") + (toString(minSize));
	ret[21] = string("\nMaxSize: ") + (toString(maxSize));
	
	ret[22] = string("\n\nBlendSRC: ") + (toString((int)blendModeSRC));
	ret[23] = string("\nBlendDST: ") + (toString((int)blendModeDST));
	ret[24] = string("\nShading: ") + (toString(shadeModus));
	ret[25] = string("\nTexturePath: \n\"") + texturePath + string("\"");
	ret[26] = string("\nstartColor: ") + toString(startColor.r) + string("/")+ toString(startColor.g) + string("/")+ toString(startColor.b) + string("/")+ toString(startColor.a);
	ret[27] = string("\nendColor: ") + toString(endColor.r) + string("/")+ toString(endColor.g) + string("/")+ toString(endColor.b) + string("/")+ toString(endColor.a);
	
	
	for(int i=0;i<anzahl;i++)
	{
		realRet+=ret[i];
	}
	
	return realRet;
}

int awx_particleSystem::writeToFile(string filename)
{
	awx_dictionary dict;
	
	dict.setObject(version,"particleSystemVersion");
	dict.setObject(minLifeTime,"minLifeTime");
	dict.setObject(maxLifeTime,"maxLifeTime");
	dict.setObject(maxParticles,"maxParticles");
	dict.setObject(emitCountMax,"emmitCountMax");
	dict.setObject(emitCountMin,"emmitCountMin");
	dict.setObject((int)proportionality,"proportionality");
	dict.setObject(emmitance,"emmitance");
	
	if(dieModus==AWXTimeDie) dict.setObject(awx_string("AWXTimeDie"),"dieMode");
	else dict.setObject(awx_string("AWXFadeDie"),"dieMode");

	if(renderMode==AWXSpherical) cout << "1" << endl;
	else if(renderMode==AWXCylindrical) cout << "2" << endl;
	else if(renderMode==AWXPointSprite) cout << "3" << endl;

	if(renderMode==AWXSpherical) dict.setObject(awx_string("AWXSpherical"),"renderMode");
	else if(renderMode==AWXCylindrical) dict.setObject(awx_string("AWXCylindrical"),"renderMode");
	else if(renderMode==AWXPointSprite) dict.setObject(awx_string("AWXPointSprite"),"renderMode");
	
	
	awx_vector3f minSizeT = awx_vector3f(minSize.x,minSize.y,0);
	awx_vector3f maxSizeT = awx_vector3f(maxSize.x,maxSize.y,0);
	int blendModeSRCT = (int) blendModeSRC;
	int blendModeDSTT = (int) blendModeDST;
	int shadeModusT = (int)shadeModus;
	
	char tP[512];
	sprintf(tP,"%s",texturePath.c_str());

	dict.setObject(gravity,"gravity");
	dict.setObject(position,"position"); 
	dict.setObject(plane,"cubic");		
	dict.setObject(minVel,"minVel");		
	dict.setObject(maxVel,"maxVel");
	dict.setObject(minSizeT,"minSize");
	dict.setObject(maxSizeT,"maxSize");
	dict.setObject(blendModeSRCT,"blendModeSRC");
	dict.setObject(blendModeDSTT,"blendModeDST");
	dict.setObject(shadeModusT,"shadeMode");	
	dict.setObject(tP,"texturePath");
	dict.setObject(endColor.r,"endColor_r");
	dict.setObject(endColor.g,"endColor_g");
	dict.setObject(endColor.b,"endColor_b");
	dict.setObject(endColor.a,"endColor_a");
	dict.setObject(startColor.r,"startColor_r");
	dict.setObject(startColor.g,"startColor_g");
	dict.setObject(startColor.b,"startColor_b");
	dict.setObject(startColor.a,"startColor_a");
	
	dict.writeToFile((char*)filename.c_str());
	
	return 0;
}

void awx_particleSystem::reloadTexture()
{
	glDeleteTextures(1,&texture);
	texture = load_tgaAndReturnObject((char*)texturePath.c_str(),true,true);
}

int awx_particleSystem::loadFromFile(string filename)
{
	awx_dictionary dict;
	dict.initWithFile((char*)filename.c_str());
	
	int temp = dict.getIntForKey("particleSystemVersion");
	if(temp!=1)
	{ 
		cout << "falsche partikelSystemversion." << endl; 
		return -1;
	}
	
	minLifeTime = dict.getFloatForKey("minLifeTime");
	maxLifeTime = dict.getFloatForKey("maxLifeTime");
	maxParticles = dict.getIntForKey("maxParticles");
	emitCountMax = dict.getFloatForKey("emmitCountMax");
	emitCountMin = dict.getFloatForKey("emmitCountMin");
	proportionality = dict.getIntForKey("proportionality");
	emmitance = dict.getIntForKey("emmitance");
	
	awx_string die = dict.getObjectForKey("dieMode");
	awx_string render = dict.getObjectForKey("renderMode");
	
	if(die.compare("AWXTimeDie")==0) dieModus=AWXTimeDie;
	else dieModus=AWXFadeDie;
	
	if(render.compare("AWXCylindrical")==0) renderMode=AWXCylindrical;
	else if(render.compare("AWXSpherical")==0) renderMode=AWXSpherical;
	else if(render.compare("AWXPointSprite")==0) renderMode=AWXPointSprite;
	
	gravity = dict.getVectorForKey("gravity");
	position = dict.getVectorForKey("position");
	plane = dict.getVectorForKey("cubic");
	minVel = dict.getVectorForKey("minVel");
	maxVel = dict.getVectorForKey("maxVel");
	minSize = awx_vector2f( (dict.getVectorForKey("minSize")).x, (dict.getVectorForKey("minSize")).y);
	maxSize = awx_vector2f( (dict.getVectorForKey("maxSize")).x, (dict.getVectorForKey("maxSize")).y);
	shadeModus = dict.getIntForKey("shadeMode");
	texturePath = string(dict.getObjectForKey("texturePath").cString());
	blendModeSRC = (GLenum)dict.getIntForKey("blendModeSRC");
	blendModeDST = (GLenum)dict.getIntForKey("blendModeDST");
	float r = dict.getFloatForKey("startColor_r");
	float g = dict.getFloatForKey("startColor_g");
	float b = dict.getFloatForKey("startColor_b");
	float a = dict.getFloatForKey("startColor_a");
	startColor = particleColor(r,g,b,a);
	r = dict.getFloatForKey("endColor_r");
	g = dict.getFloatForKey("endColor_g");
	b = dict.getFloatForKey("endColor_b");
	a = dict.getFloatForKey("endColor_a");
	endColor = particleColor(r,g,b,a);
	sizeDelta = awx_vector2f( (dict.getVectorForKey("sizeDelta")).x, (dict.getVectorForKey("sizeDelta")).y);
	
	colorDelta = endColor-startColor;
	

	awx_string temps = texturePath.c_str();
	awx_string temps2 = filename.c_str();
	temps = (temps2.stringByDeletingLastPathComponent().stringByAppendingString("/")) + temps.lastPathComponent();
	texturePath = temps.cString();
	cout << "ParticleSystemTexturePath:" << texturePath << endl;

		init();
	
	return 0;
}


void awx_particleSystemHandler::update()
{
		for(int i=0;i<systems.size();i++)
		{
			systems[i]->emit();
		}


		for(int i=0;i<systems.size();i++)
		{
			systems[i]->update();
		}
}
