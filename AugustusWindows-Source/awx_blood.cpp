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

#ifdef blut
#include "awx_blood.h"
	
awx_particleSystem::awx_particleSystem()
{
    r=1;
    a=1;
}

void awx_particleSystem::render()
{
    if(!running) return;
    
    glPushMatrix();
    glTranslatef(x,y+1,z);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    
    glColor4f(r,g,b,a);
    
    glPointSize(3);
    glBegin(GL_POINTS);
    
    for(int i=0;i<(int)particles.size();i++)
    {
        glVertex3f(particles[i].x,particles[i].y,particles[i].z);
    }
    
    glEnd();
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
    
    update();
}

void awx_particleSystem::update()
{
    float t = (float)0.001;
    speed = 1;
    dir_x = 0;
    dir_y = 12;
    dir_z = 0;

    for(int i=0;i<(int)particles.size();i++)
    {
        particles[i].x += RANDOM_FLOAT * dir_x * speed * t;
        particles[i].y -= RANDOM_FLOAT * dir_y * speed * t;
        //particles[i].z += RANDOM_FLOAT * dir_z * speed * t;

    }
    
    if(RANDOM_FLOAT<0.2)
    {
        awx_particle p;
        p.x=(float)0.3*RANDOM_FLOAT;
        p.y=(float)-0.1*RANDOM_FLOAT;
        particles.push_back(p);
    }
    
    if(RANDOM_FLOAT<0.2&&particles.size()>10||particles.size()>50)
    {
        particles.erase(particles.begin());
    }
    
    if(particles.size()>20&&RANDOM_FLOAT<0.9)
    {
        reset();
        running = false;
    }
}

void awx_particleSystem::reset()
{
    particles.clear();
}

awx_particleSystem::~awx_particleSystem()
{
    particles.clear();
}

#endif