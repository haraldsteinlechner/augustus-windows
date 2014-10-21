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

#include "awx_specialEffects.h"
#include "awx_particleSystem.h"

awx_particleSystemHandler *particleSystemHandler;

typedef struct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
	GLfloat emission[4];
} materialProperties;


typedef struct
{
	GLfloat position[4];
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat angle;
} sunProperties;


typedef enum
{
	morningE,
	middayE,
	eveningE,
	nightE
} dayTimeEnum;


class awx_weather
{
	public:
	
	materialProperties morning;
	materialProperties midday;
	materialProperties evening;
	materialProperties night;
	
	sunProperties sunMorning;	
	sunProperties sunMidday;	
	sunProperties sunEvening;	
	sunProperties sunNight;	
	
	materialProperties *currentMaterialProperties;
	sunProperties *currentSunProperties;
	
	GLfloat sunAngle;
	
	GLenum materialTarget;
	GLuint lightTarget;
	
	deepFog fog;

	awx_particleSystem *rainSystem;
	
	
	void setupStandardStates()
	{/*
		morning.ambient		=		{0.1,0.1,0.1,1.0};
		morning.diffuse		=		{0.1,0.1,0.1,1.0};
		morning.specular	=		{0.1,0.1,0.1,1.0};
		morning.shininess	=		10.0;
		morning.emission	=		{0.1,0.1,0.1,1.0};
		
		midday.ambient		=		{0.1,0.1,0.1,1.0};
		midday.diffuse		=		{0.1,0.1,0.1,1.0};
		midday.specular		=		{0.1,0.1,0.1,1.0};
		midday.shininess	=		10.0;
		midday.emission		=		{0.1,0.1,0.1,1.0};
		
		evening.ambient		=		{0.1,0.1,0.1,1.0};
		evening.diffuse		=		{0.1,0.1,0.1,1.0};
		evening.specular	=		{0.1,0.1,0.1,1.0};
		evening.shininess	=		10.0;
		evening.emission	=		{0.1,0.1,0.1,1.0};
		
		night.ambient		=		{0.1,0.1,0.1,1.0};
		night.diffuse		=		{0.1,0.1,0.1,1.0};
		night.specular		=		{0.1,0.1,0.1,1.0};
		night.shininess		=		10.0;
		night.emission		=		{0.1,0.1,0.1,1.0};
		
		
		
		sunMorning.position		=	{0.0,1000.0,0.0,1.0};
		sunMorning.angle		=	-45;
		sunMorning.ambient		=	{1.0,1.0,1.0,1.0};
		sunMorning.diffuse		=	{1.0,1.0,1.0,1.0};
		sunMorning.specular		=	{1.0,1.0,1.0,1.0};
		
		sunMidday.position		=	{0.0,1000.0,0.0,1.0};
		sunMidday.angle			=	-45;
		sunMidday.ambient		=	{1.0,1.0,1.0,1.0};
		sunMidday.diffuse		=	{1.0,1.0,1.0,1.0};
		sunMidday.specular		=	{1.0,1.0,1.0,1.0};
		
		sunEvening.position		=	{0.0,1000.0,0.0,1.0};
		sunEvening.angle		=	-45;
		sunEvening.ambient		=	{1.0,1.0,1.0,1.0};
		sunEvening.diffuse		=	{1.0,1.0,1.0,1.0};
		sunEvening.specular		=	{1.0,1.0,1.0,1.0};
		
		sunNight.position		=	{0.0,1000.0,0.0,1.0};
		sunNight.angle			=	-45;
		sunNight.ambient		=	{1.0,1.0,1.0,1.0};
		sunNight.diffuse		=	{1.0,1.0,1.0,1.0};
		sunNight.specular		=	{1.0,1.0,1.0,1.0};*/
	}
	
	awx_weather()
	{
		sunAngle = 0;
		
		materialTarget = GL_FRONT;
		lightTarget = GL_LIGHT0;
		
		setupStandardStates();
		
		currentMaterialProperties = &midday;
		currentSunProperties = &sunMidday;
		
		rainSystem = new awx_particleSystem();
	}
	
	void dealloc()
	{
		delete rainSystem;
	}
	
	void setDayTimeStatic(dayTimeEnum time)
	{
		switch(time)
		{
			case morningE: 
			
				currentMaterialProperties = &morning; 
				currentSunProperties = &sunMorning;
				break;
			
			case middayE: 
			
				currentMaterialProperties = &midday; 
				currentSunProperties = &sunMidday;
				break;
			
			case eveningE: 
			
				currentMaterialProperties = &evening; 
				currentSunProperties = &sunEvening;
				break;
			
			case nightE: 
			
				currentMaterialProperties = &night; 
				currentSunProperties = &sunNight;
				break;
			
			default: 
				
				currentMaterialProperties = &midday;
				currentSunProperties = &sunMidday;
		}
	}
	
	void updateWeatherDynamic(awx_time &currentTime)
	{
	}
	
	
	void setMaterialStatesStatic()
	{
		if(currentMaterialProperties==NULL) return;
		
		glMaterialfv(materialTarget, GL_AMBIENT, currentMaterialProperties->ambient);
		glMaterialfv(materialTarget, GL_DIFFUSE, currentMaterialProperties->diffuse);
		glMaterialfv(materialTarget, GL_SPECULAR, currentMaterialProperties->specular);
		glMaterialfv(materialTarget, GL_SHININESS, &currentMaterialProperties->shininess);
		glMaterialfv(materialTarget, GL_EMISSION, currentMaterialProperties->emission);
	}

	void setLightStatesStatic()
	{
		if(currentSunProperties==NULL) return;
		
		glPushMatrix();
			glRotatef(sunAngle,0.0,0.0,1.0);
			glLightfv(lightTarget, GL_POSITION, currentSunProperties->position);
		glPopMatrix();
		glLightfv(lightTarget, GL_AMBIENT, currentSunProperties->ambient);
		glLightfv(lightTarget, GL_DIFFUSE, currentSunProperties->diffuse);
		glLightfv(lightTarget, GL_SPECULAR, currentSunProperties->specular);
	}
	
	void setStatesStatic()
	{
		setMaterialStatesStatic();
		
		setLightStatesStatic();
	}
	
	void render()
	{	
		fog.render();

		if(fog.alpha>0)
		{
			rainSystem->emitScaleFactor=fog.alpha*8.0f;
			rainSystem->isVisible=true;
			renderRain();
		} else rainSystem->isVisible=false;
	}
	
	
	void setup()
	{
		fog.setupFog();
		rainSystem->loadFromFile("Data/ParticleSystems/rain.apsys");
		cout << rainSystem->serialize() << endl;
	}
	
	void renderRain()
	{	
		glPushMatrix();
				  
		glLoadMatrixf(camera.rotationMatrixForSpecialEffects);
		
		glTranslatef(0,0,-1);
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(45,project.aspect,0.01,10.0);
		
		glMatrixMode(GL_MODELVIEW);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_DEPTH_TEST);
		
		rainSystem->render();
		
		glPopAttrib();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
	
} currentWeather;