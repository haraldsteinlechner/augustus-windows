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

#include "awx_level_part.h"


levelpart::levelpart()
{
    apx::apx();
    
    capacity = -1;
    resourceID = -1;
    platzbedarf = 0;
    
    abbaufaktor = 1.0;
	
	myHitTriangle=NULL;
}

void levelpart::init()
{
    apx::apx();
    
    capacity = -1;
    resourceID = -1;
    platzbedarf = 0;
    
    abbaufaktor = 1.0;
	
	myHitTriangle=NULL;
}

void levelpart::reinit(levelpart &part)
{
	capacity = part.capacity;
	resourceID = part.resourceID;
	platzbedarf = part.platzbedarf;
	abbaufaktor = part.abbaufaktor;
	myHitTriangle = part.myHitTriangle;
}

void levelpart::load_apx(const char *filename)
{
    apx::load_apx(filename);
    
    awx_string pathToAttribs = filename;
    pathToAttribs = pathToAttribs.stringByDeletingExtension();
    pathToAttribs = pathToAttribs + ".att";
    if(!resourceAttributes.initWithFile(pathToAttribs.cString()))
    {
        cout << "Rohstoffeinstellungen nicht gefunden." << pathToAttribs << endl;
    }
    else
    {
        resourceID = resourceAttributes.getObjectForKey("RessID").intValue();
        capacity = resourceAttributes.getObjectForKey("Kapazitaet").intValue();
        platzbedarf = resourceAttributes.getObjectForKey("platzbedarf").floatValue();
        abbaufaktor = resourceAttributes.getObjectForKey("abbaufaktor").floatValue();
        
        cout << "loaded attribs: " << resourceID << "  " << capacity  << " platzbedarf:" << platzbedarf << " geschwindigkeitsfaktor:" << abbaufaktor << endl;
        
        if(resourceID!=-1)
        {
            current_frame = 0;
        }
    }
}

void levelpart::checkVisibility()
{
    visibilityCheck::checkVisibility(x,y,z);

}

void levelpart::render()
{
	if(myHitTriangle==NULL)return;
    if(!isVisible()&&isCheckEnabled()&&sichttestflag!=0||!(*myHitTriangle).triangleSeen) return;
    
    rendered_objects++;

	useStates();
    apx::renderFrameFromList();
}

bool levelpart::getVisibility()
{
	if(myHitTriangle==NULL||capacity<=0&&resourceID!=-1)return false;
    if(!isVisible()&&isCheckEnabled()&&sichttestflag!=0||!(*myHitTriangle).triangleSeen) return false;
	
	return true;
}

void levelpart::renderWithoutTest()
{
	useStates();
    apx::renderFrameFromList();
}

bool levelpart::updateResourceStats()
{
    if(capacity<=0)
    {
        current_frame = 0;
        return false;
    }
    else return true;
}

bool levelpart::abbau(float abbauspeed)
{
    if(resourceID!=-1)
    {
        capacity-=abbauspeed;
		if(capacity<=1)capacity=1;
        return updateResourceStats();
    }
    return 0;
}

void levelpart::renderWater(GLuint normalmap, GLuint sky, awx_shader *shader)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	shader->install();
	
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,sky);
	glUniform1i(shader->texture2,2);
	
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,normalmap);
	glUniform1i(shader->texture1,0);
	
	scaleTex-=0.0012;
	
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	glUniform1i(shader->texture0,1);
	
	glUniform2f(shader->uniform0,scaleTex,scaleTex);

	renderFrameFromList();	
	
	shader->deinstall();
	glPopAttrib();
}

bool levelpart::isAbbaubar()
{
    if(resourceID!=-1&&capacity>0) return true;
    else return false;
}

void levelpart::fillVRAM()
{

	awx_string tempstring = workingdirectory + "/" + texture_table[0].pfad + ".tga";
		
		
	if(allowMipmapsInAPX)
		texture = load_tgaAndReturnObject(tempstring.cString(), true, true);
	else
		texture = load_tgaAndReturnObject(tempstring.cString(), false, true);
    

	int frame=0;
	
		IDs[frame] = glGenLists(1);
        
      	#ifdef NVIDIACompileError
			glNewList(IDs[frame],GL_COMPILE_AND_EXECUTE);
		#endif
		#ifndef NVIDIACompileError
			glNewList(IDs[frame],GL_COMPILE);
		#endif
		
		render_frame(frame);

        glEndList();

	stateChangeList = glGenLists(1);
	
	glNewList(stateChangeList,GL_COMPILE);
	        
		if(alpha_funcFlag)
        {
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(alpha_func,alphaParam);
        }
        else glDisable(GL_ALPHA_TEST);

        if(lightModelTwoSided)
        {
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
        } else glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
		

		glBindTexture(GL_TEXTURE_2D,texture);
	
	glEndList();
	
	cout << "List=" << IDs[0] << "   stateList:" << stateChangeList << endl;


	for(int frame=0;frame<anzahl_frames;frame++)
	{
		delete part[frame].vertex;
		delete part[frame].tex_koord;
		delete part[frame].normal;
		delete part[frame].plane;
		delete part[frame].normalsequenz;
		delete part[frame].textursequenz;
		delete part[frame].vertexAttributes;
	}
}

void levelpart::useStates()
{
	glCallList(stateChangeList);
}


bool levelpart::boundingCollision(awx_vector3f position){
    awx_vector3f x1(xmin , 0 , zmin);
    awx_vector3f x2(xmax , 0 , zmin);
    awx_vector3f x3(xmin , 0 , zmax);
    
	
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glTranslatef(x,0,z);
        glRotatef(simple_rot_y,0.0,1.0,0.0);
		float modelview[16];
        for(int i=0;i<16;i++) modelview[i]=0.0;
        glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
        awx_matrix matrix;
        matrix.setMatrix(modelview);
            
        x1 = matrix.vectorProduct4(x1);
        x2 = matrix.vectorProduct4(x2);
        x3 = matrix.vectorProduct4(x3);
    glPopMatrix();
    
    float ux = x3.x-x1.x;
    float uy = x3.z-x1.z;

    float vx = x2.x-x1.x;
    float vy = x2.z-x1.z;

    float lambda=0;
    float my=0;

    float Dlambda;
    float Dmy;
    float D;
    
    Dlambda = (position.x-x1.x)*vy - (position.z-x1.z)*vx;
    Dmy =  (position.z-x1.z)*ux - (position.x-x1.x)*uy;
    D = ux*vy - uy*vx;
    
    lambda = (Dlambda/D);
    my = (Dmy/D);

    if(lambda <= 1  && my <= 1&& lambda >= 0 && my >= 0) return true;
    else return false;
}