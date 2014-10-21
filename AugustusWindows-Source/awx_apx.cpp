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


#include "awx_apx.h"
#include "awx_opengl_utilities.h"


apx::apx()
{
    for(loop=0;loop<16;loop++) modelview[loop] = 0;
    modelview[0] = 1.0;
    modelview[5] = 1.0;
    modelview[10] = 1.0;
    modelview[15] = 1.0;
    
    hasShadowVolume = false;
    
    animationspeed=25;
    u = 1.0;
    v = 1.0;
    current_frame=0;
    rot_x=0; rot_y=0; rot_z=0;
    rotation=0;
    trans_x=0;
    trans_y=0;
    trans_z=0;
    simplerotation=0;
    simple_rot_x=0; simple_rot_y=0; simple_rot_z=0;
    xmin=0;
    zmin=0;
    sektorisiert=false;
    x=0; y=0; z=0;
    
    shadowVolumeID = 0;
    anzahl_frames = -1;
	
	allowMipmapsInAPX=true;
	
	useWaterShading=false;
}

void apx::load_apx(const char *filename)
{
    int frame=0;	
    int temp_i;
    char *temp;
    workingdirectory = filename;
    workingdirectory = workingdirectory.stringByDeletingLastPathComponent();
    input = fopen(filename,"rb");
    if(input==NULL)
    {
        project.error_code=1;
        sprintf(project.error_message,"Datei (APX): %s konnte nicht ge‚Äö√†√∂‚Äö√†√áffnet werden.",filename);
        project.getError();
    }
    
    cout << "Starte Lesevorgang von: " << filename << endl;
    
    awx_fread(&version,sizeof(version),1,input);

    if(version<10) 
    { 
        project.error_code=4;
		project.error_message = new char[strlen(filename)+strlen("Versionskonflik in APX Datei: %s")];
        sprintf(project.error_message,"Versionskonflik in APX Datei: %s",filename);	
        project.getError();
		return;
    }
    
    
    // TextureTable laden
    int temp_ix=0;
    awx_fread(&temp_ix,sizeof(int),1,input);
        
    texture_table = new texture_table_typ[temp_ix];

    for(int l=0;l<temp_ix;l++)
    {
        // Pfad
        awx_fread(&temp_i,sizeof(temp_i),1,input);
        temp = new char[temp_i+1];
        awx_fread(temp,sizeof(char),temp_i,input);
        temp[temp_i]='\0';
        texture_table[l].pfad = new char[temp_i+1];
        strcpy(texture_table[l].pfad,temp);
		
		if(strcmp(texture_table[l].pfad,"greece1_water")==0)
		{
			useWaterShading=true;
		}
        
        // U
        awx_fread(&temp_i,sizeof(temp_i),1,input);
        temp = new char[temp_i+1];
        awx_fread(temp,sizeof(char),temp_i,input);
        temp[temp_i]='\0';
        sscanf(temp,"%i",&texture_table[l].u);
        
        // V
        awx_fread(&temp_i,sizeof(temp_i),1,input);
        temp = new char[temp_i+1];
        awx_fread(temp,sizeof(char),temp_i,input);
        temp[temp_i]='\0';
        sscanf(temp,"%i",&texture_table[l].v);
        
        // ID
        awx_fread(&temp_i,sizeof(temp_i),1,input);
        temp = new char[temp_i+1];
        awx_fread(temp,sizeof(char),temp_i,input);
        temp[temp_i]='\0';
        sscanf(temp,"%i",&texture_table[l].id_tex);
    }
    
    awx_fread(&anzahl_frames,sizeof(anzahl_frames),1,input);
    
    part = new awx_part[anzahl_frames];
    aktionen = new bildaktionen[anzahl_frames];
    
    IDs = new GLuint[anzahl_frames];
    
    xmax = 0;
    xmin = 0;
    zmin = 0;
    zmax = 0;
	ymin = 0;
	ymax = 0;
    
    for(frame=0;frame<anzahl_frames;frame++)
    {
        part[frame].anzahl_frames = anzahl_frames;
    
        // Framebildaktion schreiben
    
        awx_fread(&aktionen[frame].aktionsID,sizeof(aktionen[frame].aktionsID),1,input);
        awx_fread(aktionen[frame].akthelp,sizeof(char),20,input);
        awx_fread(&aktionen[frame].akthelp_int,sizeof(aktionen[frame].akthelp_int),1,input);
        awx_fread(aktionen[frame].bezeichnung,sizeof(char),20,input);
    
        // Objektinfos lesen
        awx_fread(&part[frame].vertexzahl,sizeof(int),1,input);
        awx_fread(&part[frame].normalenzahl,sizeof(int),1,input);
        awx_fread(&part[frame].planezahl,sizeof(int),1,input);
        awx_fread(&part[frame].texzahl,sizeof(int),1,input);
        
    
        // Speicher-Malloc
        part[frame].vertex = new float[part[frame].vertexzahl*3];
        part[frame].tex_koord = new float[part[frame].texzahl*2];
        part[frame].normal = new float[part[frame].normalenzahl*3];
        part[frame].plane = new int[part[frame].planezahl*3];
        part[frame].normalsequenz = new int[part[frame].planezahl*3];
        part[frame].textursequenz = new int[part[frame].planezahl*3];
		part[frame].vertexAttributes = new vec4[part[frame].vertexzahl*3];

        
        // Daten lesen
        for(loop=0;loop<part[frame].vertexzahl*3;loop+=3)
        {
            awx_fread(&part[frame].vertex[loop],sizeof(float),1,input);
            awx_fread(&part[frame].vertex[loop+1],sizeof(float),1,input);
            awx_fread(&part[frame].vertex[loop+2],sizeof(float),1,input);
			
			part[frame].vertexAttributes[loop].data[0] = 1.0;
			part[frame].vertexAttributes[loop].data[1] = 0.0;
			part[frame].vertexAttributes[loop].data[2] = 0.0;
			part[frame].vertexAttributes[loop].data[3] = 0.0;
            
            if(xmin>part[frame].vertex[loop]) xmin = part[frame].vertex[loop];
            if(zmin>part[frame].vertex[loop+2]) zmin = part[frame].vertex[loop+2];
            
            if(xmax<part[frame].vertex[loop]) xmax = part[frame].vertex[loop];
            if(zmax<part[frame].vertex[loop+2]) zmax = part[frame].vertex[loop+2];
			
			if(ymin>part[frame].vertex[loop+1]) ymin = part[frame].vertex[loop+1];
			if(ymax<part[frame].vertex[loop+1]) ymax = part[frame].vertex[loop+1];
        }
        
        bounding_radius = sqrt(pow(xmin,2) + pow(zmin,2)) + 5;
    
        for(loop=0;loop<part[frame].normalenzahl*3;loop+=3)
        {
            awx_fread(&part[frame].normal[loop],sizeof(float),1,input);
            awx_fread(&part[frame].normal[loop+1],sizeof(float),1,input);
            awx_fread(&part[frame].normal[loop+2],sizeof(float),1,input);
        }

        for(loop=0;loop<part[frame].texzahl*2;loop+=2)
        {
            awx_fread(&part[frame].tex_koord[loop],sizeof(float),1,input);
            awx_fread(&part[frame].tex_koord[loop+1],sizeof(float),1,input);
        }

        for(loop=0;loop<part[frame].planezahl*3;loop+=3)
        {	
            awx_fread(&part[frame].plane[loop],sizeof(int),1,input);
            awx_fread(&part[frame].plane[loop+1],sizeof(int),1,input);
            awx_fread(&part[frame].plane[loop+2],sizeof(int),1,input);
    
            awx_fread(&part[frame].normalsequenz[loop],sizeof(int),1,input);
            awx_fread(&part[frame].normalsequenz[loop+1],sizeof(int),1,input);
            awx_fread(&part[frame].normalsequenz[loop+2],sizeof(int),1,input);
    
            awx_fread(&part[frame].textursequenz[loop],sizeof(int),1,input);
            awx_fread(&part[frame].textursequenz[loop+1],sizeof(int),1,input);
            awx_fread(&part[frame].textursequenz[loop+2],sizeof(int),1,input);
        }
    
    }
    int cull,alphabool,lighttwo,mips;
	mips=1;
    for(int i=0;i<=0;i++)
    {
        if(awx_fread(&cull,sizeof(cull),1,input)==0) break;
        if(awx_fread(&alphabool,sizeof(alphabool),1,input)==0) break;
        if(awx_fread(&alpha_func,sizeof(alpha_func),1,input)==0) break;
        if(awx_fread(&alphaParam,sizeof(alphaParam),1,input)==0) break;
        if(awx_fread(&lighttwo,sizeof(lighttwo),1,input)==0) break;
		if(awx_fread(&mips,sizeof(mips),1,input)==0) break;
    }

    fclose(input);

	if(cull==1) cullFace=true;
	else cullFace=false;

	if(alphabool==1) alpha_funcFlag=true;
	else alpha_funcFlag=false;

	if(lighttwo==1)lightModelTwoSided=true;
	else lightModelTwoSided=false;

	if(mips==0) allowMipmapsInAPX=false;
	else allowMipmapsInAPX=true;
    
	setSize(xmax-xmin,zmax-zmin);
	
    cout << "Datei gelesen: " << filename << endl;
}

void apx::display_data(int frame)
{
    awx_utilities.strich();
    
    for(loop=0;loop<part[frame].vertexzahl*3;loop+=3)
    {
        printf("%10.1f %10.1f %10.1f\n", part[frame].vertex[loop], part[frame].vertex[loop+1], part[frame].vertex[loop+2]);
    }
    
    printf("\n");
    
    for(loop=0;loop<part[frame].texzahl*2;loop+=2)
    {
        printf("%i   %f %f\n",loop, part[frame].tex_koord[loop],part[frame].tex_koord[loop+1]);
    }
	
	for(loop=0;loop<part[frame].planezahl*3;loop+=3)
	{
		printf("\nNORM %i: %f  %f  %f\n",loop,part[frame].normal[part[frame].normalsequenz[loop]*3],part[frame].normal[part[frame].normalsequenz[loop]*3+1],part[frame].normal[part[frame].normalsequenz[loop]*3+2]);
        printf("TEX %i: %f  %f\n",loop,part[frame].tex_koord[part[frame].textursequenz[loop]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop]*2+1]*v);
		printf("VERT %i: %f  %f  %f\n",loop,part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
        
        printf("NORM %i: %f  %f  %f\n",loop,part[frame].normal[part[frame].normalsequenz[loop+1]*3],part[frame].normal[part[frame].normalsequenz[loop+1]*3+1],part[frame].normal[part[frame].normalsequenz[loop+1]*3+2]);
		printf("TEX %i: %f  %f\n",loop,part[frame].tex_koord[part[frame].textursequenz[loop+1]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+1]*2+1]*v);
		printf("VERT %i: %f  %f  %f\n",loop,part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
        
		printf("NORM %i: %f  %f  %f\n",loop,part[frame].normal[part[frame].normalsequenz[loop+2]*3],part[frame].normal[part[frame].normalsequenz[loop+2]*3+1],part[frame].normal[part[frame].normalsequenz[loop+2]*3+2]);
		printf("TEX %i: %f  %f \n",loop,part[frame].tex_koord[part[frame].textursequenz[loop+2]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+2]*2+1]*v);
		printf("VERT %i: %f  %f  %f \n",loop,part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
	}
    
    awx_utilities.strich();
}

void apx::render_frame(int frame)
{
    glBegin(GL_TRIANGLES);
        for(loop=0;loop<part[frame].planezahl*3;loop+=3)
        {
            glNormal3f(part[frame].normal[part[frame].normalsequenz[loop]*3],part[frame].normal[part[frame].normalsequenz[loop]*3+1],part[frame].normal[part[frame].normalsequenz[loop]*3+2]);
            glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop]*2+1]*v);
            glVertex3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
        
            glNormal3f(part[frame].normal[part[frame].normalsequenz[loop+1]*3],part[frame].normal[part[frame].normalsequenz[loop+1]*3+1],part[frame].normal[part[frame].normalsequenz[loop+1]*3+2]);
            glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop+1]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+1]*2+1]*v);
            glVertex3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
        
            glNormal3f(part[frame].normal[part[frame].normalsequenz[loop+2]*3],part[frame].normal[part[frame].normalsequenz[loop+2]*3+1],part[frame].normal[part[frame].normalsequenz[loop+2]*3+2]);
            glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop+2]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+2]*2+1]*v);
            glVertex3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
        }
    glEnd();
}

void apx::renderAtOrigin()
{
	glCallList(IDs[(int)floor(current_frame)]);
}

void apx::render_frameWithTangents(int frame)
{
    glPushMatrix();
    
    //glMultMatrixf(modelview);
	awx_vector3f t,b,n;

    glBegin(GL_TRIANGLES);
        for(loop=0;loop<part[frame].planezahl*3;loop+=3)
        {
			float pos1[] = {part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]};
			float pos2[] = {part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]};
			float pos3[] = {part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]};
			
			float tex1[] = {part[frame].tex_koord[part[frame].textursequenz[loop]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop]*2+1]*v};
			float tex2[] = {part[frame].tex_koord[part[frame].textursequenz[loop+1]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+1]*2+1]*v};
			float tex3[] = {part[frame].tex_koord[part[frame].textursequenz[loop+2]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+2]*2+1]*v};

			CalcTangentSpaceVectors(pos1,pos2,pos3,tex1,tex2,tex3,t,b,n);
			
			glMultiTexCoord3f(GL_TEXTURE1,t.x,t.y,t.z);
			
            glNormal3f(part[frame].normal[part[frame].normalsequenz[loop]*3],part[frame].normal[part[frame].normalsequenz[loop]*3+1],part[frame].normal[part[frame].normalsequenz[loop]*3+2]);
            glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop]*2+1]*v);
            glVertex3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
        
            glNormal3f(part[frame].normal[part[frame].normalsequenz[loop+1]*3],part[frame].normal[part[frame].normalsequenz[loop+1]*3+1],part[frame].normal[part[frame].normalsequenz[loop+1]*3+2]);
            glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop+1]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+1]*2+1]*v);
            glVertex3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
        
            glNormal3f(part[frame].normal[part[frame].normalsequenz[loop+2]*3],part[frame].normal[part[frame].normalsequenz[loop+2]*3+1],part[frame].normal[part[frame].normalsequenz[loop+2]*3+2]);
            glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop+2]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+2]*2+1]*v);
            glVertex3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
		 }
    glEnd();
    
    glPopMatrix();
}

void apx::fillVRAM()
{
	fillVRAM(awx_vector3f(0,0,0),false,0,0);
}

void apx::fillVRAM(GLuint loadTexture)
{
	fillVRAM(awx_vector3f(0,0,0),false,0,loadTexture);
}

void apx::fillVRAM(awx_vector3f lPos, bool t,float winkel, GLuint loadTexture)
{
    hasShadowVolume = t;

    //for(int i=0;i<texturanzahl;i++)
//	{
        //sprintf(resultPath,"%s%s%s",texturePath,texturen[i].textur,textureSuffix);
		
		if(loadTexture==0)
		{
		
			awx_string tempstring = workingdirectory + "/" + texture_table[0].pfad + ".tga";
		
		
			if(allowMipmapsInAPX)
			texture = load_tgaAndReturnObject(tempstring.cString(), true, true);
			else
				texture = load_tgaAndReturnObject(tempstring.cString(), false, true);
		}
		else texture = loadTexture;
//	}
    

    for(int frame=0;frame<anzahl_frames;frame++)
    {
		IDs[frame] = glGenLists(1);
        
      	#ifdef NVIDIACompileError
			glNewList(IDs[frame],GL_COMPILE_AND_EXECUTE);
		#endif
		#ifndef NVIDIACompileError
			glNewList(IDs[frame],GL_COMPILE);
		#endif

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
		
		//glMultMatrixf(modelview);
		render_frame(frame);

        glEndList();
        
    }
	
	cout << "List=" << IDs[0] << endl;

    currentID+=anzahl_frames;
    
    if(project.shadowvolumes&&hasShadowVolume)
    {
        prepareShadowVolume(winkel);
    }
    else
	{ 
		//cout << "delete RAM Data" << endl;
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
}

void apx::fillVRAMWithBump()
{
	int frame=0;
	awx_string tempstring = workingdirectory + "/" + texture_table[0].pfad + ".tga";
		
	if(allowMipmapsInAPX)
		texture = load_tgaAndReturnObject(tempstring.cString(), true, true);
	else
		texture = load_tgaAndReturnObject(tempstring.cString(), false, true);
    
	IDs[frame] = glGenLists(1);

	#ifdef NVIDIACompileError
		glNewList(IDs[frame],GL_COMPILE_AND_EXECUTE);
	#endif
	#ifndef NVIDIACompileError
		glNewList(IDs[frame],GL_COMPILE);
	#endif


	render_frameWithTangents(frame);

	glEndList();
	
	currentID++;

	cout << "delete RAM Data" << endl;
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

void apx::prepareShadowVolume(float winkel)
{	
        if(!project.shadowvolumes) return;
        Uint32 start = SDL_GetTicks();
        createSilhuette(winkel);
		shadowVolumeID = glGenLists(1);
        #ifdef NVIDIACompileError
			glNewList(shadowVolumeID,GL_COMPILE_AND_EXECUTE);
		#endif
		#ifndef NVIDIACompileError
			glNewList(IDs[frame],GL_COMPILE);
		#endif
        if(project.shadowvolumeMode==AWXStaticVolume) renderShadowVolumeCPU(10.0);
        else if(project.shadowvolumeMode==AWXDynamicVolume) renderExtrusionGPU();
        glEndList();
        silhuetteEdges.clear();
        currentID++;
        cout << "Created ShadowVolume on " << shadowVolumeID << " Size= " << silhuetteEdges.size() << " in " << (SDL_GetTicks()-start)/1000.0 << " seconds" << endl;
}

apx::~apx()
{
	/*return;

    cout << "cleanAPX" << endl;
    for(int frame=0;frame<anzahl_frames;frame++)
    {
		delete part[frame].vertex;
        delete  part[frame].tex_koord;
        delete part[frame].normal;
		delete part[frame].plane;
        delete part[frame].normalsequenz;
        delete part[frame].textursequenz;
		delete part[frame].vertexAttributes;
    }*/
}


void apx::renderExtrusion()
{
	glCallList(shadowVolumeID);
}

/*void findNeighbours()
{
    awx_vector3f pT[3];
    awx_vector3f p1,p2,p3;
    
    edges.clear();
    
    int frame=0;
    for(int loop=0;loop<part[frame].planezahl*3;loop+=3)
    {
        pT[0] = awx_vector3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
        pT[1] = awx_vector3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
        pT[2] = awx_vector3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
        
        for(int i=0;i<3;i++)
        {
            p1 = pT[i];
            p2 = pT[(i+1)%3];
            p3 = pT[(i+2)%3];
                
            // Gleiches Edge finden
            bool found = false;
            for(int u=0;u<edges.size();u++)
            {
                if((p1.compareTo(edges[u].p1)&&p2.compareTo(edges[u].p2))||(p1.compareTo(edges[u].p2)&&p2.compareTo(edges[u].p1)))
                {
                    found = true;
                    break;
                }
            }
                
            if(!found)
            {
                edgeIndex temp;
                temp.p1 = loop;
                temp.p2 = loop + (i+1)%3;

                edges.push_back(temp);
            }
        }
    }
}*/

/*void createSilhuetteFromEdges(float winkel)
{
    for(int i=0;i<edges.size();i++)
    {
    
        pT[0] = awx_vector3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
        pT[1] = awx_vector3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
        pT[2] = awx_vector3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
    }
}*/

void apx::createSilhuette(float winkel)
{
    int frame=0;
    
    
    // gedrehte matrix erstellen
    awx_matrix toss;
    toss = toss.rotateY(-winkel);
    awx_vector3f lPos = sunDirection;

    silhuetteEdges.clear();
    
    awx_vector3f p1,p2,n,p3;
    awx_vector3f nCalc, a, b;
    awx_vector3f k;
    awx_vector3f pT[3];
    float NDotL;
	
	awx_matrix mv = modelview;
    
    for(loop=0;loop<part[frame].planezahl*3;loop+=3)
    {
        n = awx_vector3f(part[frame].normal[part[frame].normalsequenz[loop]*3],part[frame].normal[part[frame].normalsequenz[loop]*3+1],part[frame].normal[part[frame].normalsequenz[loop]*3+2]);
        
        pT[0] = awx_vector3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
        pT[1] = awx_vector3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
        pT[2] = awx_vector3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
        
		pT[0] = mv.vectorProduct4(pT[0]);
		pT[1] = mv.vectorProduct4(pT[1]);
		pT[2] = mv.vectorProduct4(pT[2]);
		
        pT[0] = toss.vectorProduct(pT[0]);
        pT[1] = toss * pT[1];
        pT[2] = toss * pT[2];
        
        n = n * toss;
        
        a = pT[2] - pT[1];
        b = pT[0] - pT[2];
            
        nCalc = a.crossProduct(b);
        nCalc = nCalc.normalize();
        
        if(nCalc.scalarProduct(n)<0)
        {
            nCalc = nCalc.product(-1);
        }
        
        NDotL = nCalc.scalarProduct(lPos);
        
        if(NDotL>0.0)
        {
            for(int i=0;i<3;i++)
            {
                p1 = pT[i];
                p2 = pT[(i+1)%3];
                p3 = pT[(i+2)%3];
                
                // Gleiches Edge finden
                bool killed = false;
                for(int i=0;i<(int)silhuetteEdges.size();i++)
                {
                    if((p1.compareTo(silhuetteEdges[i].p1)&&p2.compareTo(silhuetteEdges[i].p2))||(p1.compareTo(silhuetteEdges[i].p2)&&p2.compareTo(silhuetteEdges[i].p1)))
                    {
                        silhuetteEdges.erase(silhuetteEdges.begin()+i);
                        killed = true;
                        break;
                    }
                }
                
                if(!killed)
                {
                    edge temp;
                    temp.p1 = p1;
                    temp.p2 = p2;

                    silhuetteEdges.push_back(temp);
                }
            }
        }
    }
}

void apx::renderShadowVolumeCPU(float infinity)
{	
    cout << "Rendering ShadowVolume CPU" << endl;

    glBegin(GL_QUADS);
    for(int i=0;i<(int)silhuetteEdges.size();i++)
    {
        glVertex3f(silhuetteEdges[i].p1.x,silhuetteEdges[i].p1.y,silhuetteEdges[i].p1.z);
        glVertex3f(silhuetteEdges[i].p2.x,silhuetteEdges[i].p2.y,silhuetteEdges[i].p2.z);
        
        glVertex3f(silhuetteEdges[i].p2.x - sunDirection.x*infinity,silhuetteEdges[i].p2.y - sunDirection.y*infinity,silhuetteEdges[i].p2.z - sunDirection.z*infinity);
        glVertex3f(silhuetteEdges[i].p1.x - sunDirection.x*infinity,silhuetteEdges[i].p1.y - sunDirection.y*infinity,silhuetteEdges[i].p1.z - sunDirection.z*infinity);

    }
    glEnd(); 
}

void apx::renderFrameFromList()
{
    glPushMatrix();
    glTranslatef(x,y,z);
    
    
    if(rotation!=0)
    {
        glTranslatef(trans_x,trans_y,trans_z);
        glRotatef(rotation,rot_x,rot_y,rot_z);
        glTranslatef(-trans_x,-trans_y,-trans_z);
    }
    
    if(simplerotation!=0) glRotatef(simplerotation,simple_rot_x,simple_rot_y,simple_rot_z);
    
    glMultMatrixf(modelview);
    
    glCallList(IDs[(int)floor(current_frame)]);
	glPopMatrix();
}

void apx::play()
{
	play(current_frame);
}
    
void apx::play(float &current_frame)
{	
    glPushMatrix();
    glTranslatef(x,y,z);
    rendered_objects++;

    if(rotation!=0)
    {
        glTranslatef(trans_x,trans_y,trans_z);
        glRotatef(rotation,rot_x,rot_y,rot_z);
        glTranslatef(-trans_x,-trans_y,-trans_z);
    }
    if(simplerotation!=0) glRotatef(simplerotation,simple_rot_x,simple_rot_y,simple_rot_z);
    glMultMatrixf(modelview);
    //glScalef(scale_x,scale_y,scale_z);

    glCallList(IDs[int(floor(current_frame))]);
    glPopMatrix();
    
    //&aktionen[frame].aktionsID,sizeof(aktionen[frame].aktionsID),1,input);
        /*awx_fread(aktionen[frame].akthelp,sizeof(char),20,input);
        awx_fread(&aktionen[frame].akthelp_int,sizeof(aktionen[frame].akthelp_int),1,input);
        awx_fread(aktionen[frame].bezeichnung*/
    
    animationspeed=30;
    
    // Aktionen verarbeiten
    switch(aktionen[int(floor(current_frame))].aktionsID)
    {
        case 0: 
            break;
        
        case 1: 
            current_frame = aktionen[int(current_frame)].akthelp_int;
            break;
        
        case -1: 
            float ux = 1.0/(float(runtime.fps)/float(animationspeed));
            if(ux>1.0) ux = 1.0;
            current_frame+= ux;
            if(current_frame>=anzahl_frames) current_frame=0;
            break;
    }
}

void apx::renderBlob()
{
    glPushMatrix();
    glTranslatef(x,y,z);
    
    
    if(rotation!=0)
    {
        glTranslatef(trans_x,trans_y,trans_z);
        glRotatef(rotation,rot_x,rot_y,rot_z);
        glTranslatef(-trans_x,-trans_y,-trans_z);
    }
    
    if(simplerotation!=0) glRotatef(simplerotation,simple_rot_x,simple_rot_y,simple_rot_z);
    
    glMultMatrixf(modelview);
    glutSolidCube(10);
    glBindTexture(GL_TEXTURE_2D,textur_liste[3]);
        
        glBegin(GL_POLYGON);
            glTexCoord2f(0.0,0.0);
            glVertex3f(0.0,0.0,0.0);
            glTexCoord2f(1.0,0.0);
            glVertex3f(1.0,0.0,0.0);
            glTexCoord2f(1.0,1.0);
        glVertex3f(1.0,0.0,1.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0,0.0,1.0);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glEnd();
    glPopMatrix();
}

void apx::gotoBezeichnung(char* bezeichnung)
{
    int i=0;
    bool found=false;
    
    for(i=0;i<anzahl_frames;i++)
    {
        if(strcmp(bezeichnung,aktionen[i].bezeichnung)==0)
        {
            found = true;
            break;
        }
    }
    
    if(found)
    current_frame=i;		
}

void apx::gotoBezeichnung(char* bezeichnung , int random)
{
    int i=0;
    bool found=false;
    
    for(i=0;i<anzahl_frames;i++)
    {
        if(strcmp(bezeichnung,aktionen[i].bezeichnung)==0)
        {
            found = true;
            break;
        }
    }
    
    if(found)
    current_frame=i + RANDOM_FLOAT*random;
}

void apx::setSpeed(int speed)
{
    animationspeed=speed;
}

void apx::setPositionf(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void apx::movef(float x, float y, float z)
{
    this->x+=x;
    this->y+=y;
    this->z+=z;
}

void apx::scalef(float x, float y, float z)
{
    this->scale_x=x;
    this->scale_y=y;
    this->scale_z=z;
}


/* !!!!!! Nicht voll Funktionst‚àö¬∫chtig !!!!!! */
void apx::rotate_achsef(float winkel, float trans_x, float trans_y, float trans_z, float rot_x, float rot_y, float rot_z)
{
    rotation=winkel;
    this->trans_x=trans_x;
    this->trans_y=trans_y;
    this->trans_z=trans_z;
    this->rot_x=rot_x;
    this->rot_y=rot_y;
    this->rot_z=rot_z;
}

void apx::rotate_um_mittelpunktf(float winkel, float x, float y, float z)
{
    simplerotation=winkel;
    simple_rot_x = x;
    simple_rot_y = y;
    simple_rot_z = z;
}

void apx::render()
{	
    glPushMatrix();
    /*glTranslatef(x,y,z);
    
    
    if(rotation!=0)
    {
        glTranslatef(trans_x,trans_y,trans_z);
        glRotatef(rotation,rot_x,rot_y,rot_z);
        glTranslatef(-trans_x,-trans_y,-trans_z);
    }
    
    if(simplerotation!=0) glRotatef(simplerotation,simple_rot_x,simple_rot_y,simple_rot_z);
    
    glMultMatrixf(modelview);*/
    
    //glScalef(scale_x,scale_y,scale_z);
    glCallList(IDs[0]);
    glPopMatrix();
}

awx_vector3f apx::getPositionOfVector(awx_vector3f vec)
{
    awx_matrix matrix;
    matrix.setMatrix(modelview);
    return matrix.vectorProduct4(vec);
}
/*
const apx & apx::operator= (const apx &x)
{
	xmin = x.xmin;
	xmax = x.xmax;
	ymin = x.ymin;
	ymax = x.ymax;
	zmin = x.zmin;
	zmax = x.zmax;
	
	blobsizex = xmax-xmin;
	blobsizey = zmax-zmin;
	
	return 
}*/


void apx::renderBlobTexture(awx_vector3f position,awx_vector3f pos)
{
	#ifndef _WIN32
	#pragma mark BLOB
	#endif 
	
	
    int size=project.planarShadowSize;
	
	float xmin=this->xmin;
	float xmax=this->xmax;
	float zmin=this->zmin;
	float zmax=this->zmax;

    glViewport( 0, 0, size, size );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	if((xmax-xmin)>(zmax-zmin))
	glOrtho(xmin,xmax,xmin,xmax,0.1,100);
	else 
	glOrtho(zmin,zmax,zmin,zmax,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	

    glPushMatrix();
    glLoadIdentity();
	
	gluLookAt(xmin+(xmax-xmin)/2.0,100,zmin+(zmax-zmin)/2.0,
			  xmin+(xmax-xmin)/2.0,0,zmin+(zmax-zmin)/2.0,
			  0,0,1);

	glDisable(GL_LIGHTING);
    glCallList(IDs[0]);		
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();


    unsigned char *pixels = new unsigned char[size*size*4];
    GLfloat *depths = new GLfloat[size*size];
    
    glReadPixels(0,0,size,size,GL_DEPTH_COMPONENT,GL_FLOAT,depths);
    glReadPixels(0,0,size,size,GL_RGBA,GL_UNSIGNED_BYTE,pixels);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    int d=0;
    for(int i=0;i<size*size*4;i+=4)
    {
        if(depths[d]>=1.0)
        {
            pixels[i]=0;
            pixels[i+1]=0;
            pixels[i+2]=0;
            pixels[i+3]=0;
        }
        else
        {
            pixels[i]=00;
            pixels[i+1]=0;
            pixels[i+2]=0 ;
            pixels[i+3]=int(0.3*255);
        }
        d++;
    }	
    
	glGenTextures(1,&blobTexture);
	cout << "BlobTextureObject: " << blobTexture << endl;
    glBindTexture(GL_TEXTURE_2D,blobTexture);
    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,size,size,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    
    if(!project.softblobs) return;
    
    glDisable(GL_ALPHA_TEST);

    glDisable(GL_LIGHTING);
    
    glViewport( 0, 0, size, size);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	
        
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,1,0,1);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D,blobTexture);
    typedef struct 
    {
        GLfloat x, y;
    } jitter_point;
    
    jitter_point j8[] =
    {
        {-0.334818,  0.435331},
        { 0.286438, -0.393495},
        { 0.459462,  0.141540},
        {-0.414498, -0.192829},
        {-0.183790,  0.082102},
        {-0.079263, -0.317383},
        { 0.102254,  0.299133},
        { 0.164216, -0.054399}
    };
    
    glClear(GL_ACCUM_BUFFER_BIT);
    
    for(int i=0;i<8;i++)
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        glTranslatef(j8[i].x/3,j8[i].y/3,0);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(1.0,0.0,0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(1.0,1.0,0.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0,1.0,0.0);
        glEnd();
        
        glAccum(GL_ACCUM,1.0/8.0);
    }
    
    glAccum(GL_RETURN,1.0);

    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glReadPixels(0,0,size,size,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
    
    for(int i=0;i<size*size*4-4;i+=4)
    {
        if(pixels[i+3]==0)
        {
            pixels[i]=0;
            pixels[i+1]=0;
            pixels[i+2]=0;
            pixels[i+3]=0;
        }

    }
    
    glBindTexture(GL_TEXTURE_2D,blobTexture);
    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,size,size,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glEnable(GL_ALPHA_TEST);
    
    delete pixels;
    delete depths;
}

    void apx::renderExtrusionGPU()
{
    int frame=0;
    awx_vector3f pT[3],n,nCalc,a,b;

    glBegin(GL_QUADS);
        for(loop=0;loop<part[frame].planezahl*3;loop+=3)
        {
            n = awx_vector3f(part[frame].normal[part[frame].normalsequenz[loop]*3],part[frame].normal[part[frame].normalsequenz[loop]*3+1],part[frame].normal[part[frame].normalsequenz[loop]*3+2]);
        
            pT[0] = awx_vector3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
            pT[1] = awx_vector3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
            pT[2] = awx_vector3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
        
            a = pT[2] - pT[1];
            b = pT[0] - pT[2];
            
            nCalc = a.crossProduct(b);
            nCalc = nCalc.normalize();
        
            if(nCalc.scalarProduct(n)<0)
            {
                nCalc = nCalc.product(-1);
            }
            
            glNormal3f(n.x,n.y,n.z);
        
        
                // 1---2
                glVertexAttrib1fARB(AWXExtrudorLocation,0.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
                
                glVertexAttrib1fARB(AWXExtrudorLocation,0.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
            
                glVertexAttrib1fARB(AWXExtrudorLocation,1.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
            
                glVertexAttrib1fARB(AWXExtrudorLocation,1.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);

                // 2----3
                glVertexAttrib1fARB(AWXExtrudorLocation,0.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
        
                glVertexAttrib1fARB(AWXExtrudorLocation,0.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
            
                glVertexAttrib1fARB(AWXExtrudorLocation,1.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
            
                glVertexAttrib1fARB(AWXExtrudorLocation,1.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);


                // 3---1
                glVertexAttrib1fARB(AWXExtrudorLocation,0.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
            
                glVertexAttrib1fARB(AWXExtrudorLocation,0.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
            
                glVertexAttrib1fARB(AWXExtrudorLocation,1.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
            
                glVertexAttrib1fARB(AWXExtrudorLocation,1.0);
                glVertex3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);

        }
        
    glEnd();
    
    cout << "ERR=" << glGetError()<< "  " << currentID << endl;
}

void apx::createVBO()
{
	cout << "WARNING: generating apx vbo" << endl;
	awx_string tempstring = workingdirectory + "/" + texture_table[0].pfad + ".tga";
	load_tga(tempstring.cString(),currentID+0, true, true);
	IDs = new GLuint[1];
	IDs[0] = currentID;
	currentID++;
	
	int frame=0;
		
	GLfloat *vertices=new GLfloat[part[frame].planezahl*9];
	GLfloat *normals=new GLfloat[part[frame].planezahl*9];
	GLfloat *texkoords=new GLfloat[part[frame].planezahl*6];
	GLfloat *texturGew=new GLfloat[part[frame].planezahl*12];
	GLfloat *begehbarkeit=new GLfloat[part[frame].planezahl*3];

	int normal_loop=0;
	int vertex_loop=0; 
	int tex_loop=0;
	int texturGew_loop=0;
	int r=0;
	
	v = texture_table[0].v;		// Bug
	u = texture_table[0].u;
		
	for(loop=0;loop<part[frame].planezahl*3;loop+=3)
	{
		int loopPerVertex = (int) loop;
		
		begehbarkeit[r] = 0.0;
		begehbarkeit[r+1] = 0.0;
		begehbarkeit[r+2] = 0.0;
		
		r+=3;
		
		normals[normal_loop] = part[frame].normal[part[frame].normalsequenz[loop]*3];
		normals[normal_loop+1] = part[frame].normal[part[frame].normalsequenz[loop]*3+1];
		normals[normal_loop+2] = part[frame].normal[part[frame].normalsequenz[loop]*3+2];
				
		normals[normal_loop+3] = part[frame].normal[part[frame].normalsequenz[loop+1]*3];
		normals[normal_loop+4] = part[frame].normal[part[frame].normalsequenz[loop+1]*3+1];
		normals[normal_loop+5] = part[frame].normal[part[frame].normalsequenz[loop+1]*3+2];
				
		normals[normal_loop+6] = part[frame].normal[part[frame].normalsequenz[loop+2]*3];
		normals[normal_loop+7] = part[frame].normal[part[frame].normalsequenz[loop+2]*3+1];
		normals[normal_loop+8] = part[frame].normal[part[frame].normalsequenz[loop+2]*3+2];
				
		texturGew[texturGew_loop+0] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex]*3].data[0];
		texturGew[texturGew_loop+1] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex]*3].data[1];
		texturGew[texturGew_loop+2] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex]*3].data[2];
		texturGew[texturGew_loop+3] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex]*3].data[3];
				
		texturGew[texturGew_loop+4] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex+1]*3].data[0];
		texturGew[texturGew_loop+5] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex+1]*3].data[1];
		texturGew[texturGew_loop+6] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex+1]*3].data[2];
		texturGew[texturGew_loop+7] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex+1]*3].data[3];
				
		texturGew[texturGew_loop+8] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex+2]*3].data[0];
		texturGew[texturGew_loop+9] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex+2]*3].data[1];
		texturGew[texturGew_loop+10] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex+2]*3].data[2];
		texturGew[texturGew_loop+11] = part[frame].vertexAttributes[part[frame].plane[loopPerVertex+2]*3].data[3];
				
		vertices[vertex_loop] = part[frame].vertex[part[frame].plane[loop]*3];
		vertices[vertex_loop+1] = part[frame].vertex[part[frame].plane[loop]*3+1];
		vertices[vertex_loop+2] = part[frame].vertex[part[frame].plane[loop]*3+2];
				
		vertices[vertex_loop+3] = part[frame].vertex[part[frame].plane[loop+1]*3];
		vertices[vertex_loop+4] = part[frame].vertex[part[frame].plane[loop+1]*3+1];
		vertices[vertex_loop+5] = part[frame].vertex[part[frame].plane[loop+1]*3+2];
				
		vertices[vertex_loop+6] = part[frame].vertex[part[frame].plane[loop+2]*3];
		vertices[vertex_loop+7] = part[frame].vertex[part[frame].plane[loop+2]*3+1];
		vertices[vertex_loop+8] = part[frame].vertex[part[frame].plane[loop+2]*3+2];
				
		texkoords[tex_loop] = part[frame].tex_koord[part[frame].textursequenz[loop]*2]*u;
		texkoords[tex_loop+1] = part[frame].tex_koord[part[frame].textursequenz[loop]*2+1]*v;
				
		texkoords[tex_loop+2] = part[frame].tex_koord[part[frame].textursequenz[loop+1]*2]*u;
		texkoords[tex_loop+3] = part[frame].tex_koord[part[frame].textursequenz[loop+1]*2+1]*v;
				
		texkoords[tex_loop+4] = part[frame].tex_koord[part[frame].textursequenz[loop+2]*2]*u;
		texkoords[tex_loop+5] = part[frame].tex_koord[part[frame].textursequenz[loop+2]*2+1]*v;
			
		vertex_loop+=9;
		normal_loop+=9;
		tex_loop+=6;
		texturGew_loop+=12;
	}
			
	// VBO Part
	glGenBuffersARB(1,&vboVertex);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertex);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*9*sizeof(float),vertices,GL_STATIC_DRAW_ARB);
		
	glGenBuffersARB(1,&vboTexCoord);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexCoord);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*6*sizeof(float),texkoords,GL_STATIC_DRAW_ARB);
	
	glGenBuffersARB(1,&vboBegehbarkeit);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboBegehbarkeit);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*sizeof(float)*3,begehbarkeit,GL_DYNAMIC_DRAW_ARB);
		
		
	glGenBuffersARB(1,&vboNormal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormal);
		
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*9*sizeof(float),normals,GL_STATIC_DRAW_ARB);
		
	glGenBuffersARB(1,&vboTextureWeights);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTextureWeights);
	
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*12*sizeof(float),texturGew,GL_DYNAMIC_DRAW_ARB);
		
		
	delete vertices;
	delete texkoords;
	delete normals;
	delete texturGew;
}

void apx::renderGamePlayMode()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArrayARB(AWXBegehbarkeitsAttrib);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertex);
	glVertexPointer(3,GL_FLOAT,0,NULL);
				
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboBegehbarkeit);
	glVertexAttribPointerARB(AWXBegehbarkeitsAttrib,1,GL_FLOAT,0,0,NULL);
		
	glDrawArrays(GL_TRIANGLES,0,part[0].planezahl*3);
		
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}	
	
void apx::renderVBO()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableVertexAttribArrayARB(AWXAlphaAttrib);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertex);
	glVertexPointer(3,GL_FLOAT,0,NULL);
		
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormal);
	glNormalPointer(GL_FLOAT,0,NULL);
		
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexCoord);
	glTexCoordPointer(2,GL_FLOAT,0,NULL);
		
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTextureWeights);
	glVertexAttribPointerARB(AWXAlphaAttrib,4,GL_FLOAT,0,0,NULL);
		
	glDrawArrays(GL_TRIANGLES,0,part[0].planezahl*3);
		
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableVertexAttribArrayARB(AWXAlphaAttrib);
}

void apx::startUpdate()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTextureWeights);
	updatePtr = (float*) glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_READ_WRITE_ARB);
}
	
void apx::stopUpdate()
{
	glBindBuffer(GL_ARRAY_BUFFER_ARB,vboTextureWeights);
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	glBindBuffer(GL_ARRAY_BUFFER_ARB,0);
}

void apx::changeTextureWeight(int loop,int l,int tex,float change)
{
	int ui = loop*4 + l*4;
		
	for(int i=0;i<4;i++)
	{
		if(tex==i)
		{
			updatePtr[ui+i] += change;
		}
		else updatePtr[ui+i] -= change;
			 
		if(updatePtr[ui+i]>1.0)updatePtr[ui+i]=1.0;
		else if(updatePtr[ui+i]<0.0)updatePtr[ui+i]=0.0;
	}
}

void apx::setAllTextureWeights(int index)
{
	for(int i=0;i<part[0].vertexzahl*6;i++)
	{
		for(int u=0;u<4;u++)
		{
			if(u==index)
			{
				updatePtr[i*4+u]=1.0;
			}
			else updatePtr[i*4+u]=0.0;
		}
	}
}

void apx::startUpdateBegehbarkeit()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboBegehbarkeit);
	updatePtr2 = (float*) glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_READ_WRITE_ARB);
}

void apx::stopUpdateBegehbarkeit()
{
	glBindBuffer(GL_ARRAY_BUFFER_ARB,vboBegehbarkeit);
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	glBindBuffer(GL_ARRAY_BUFFER_ARB,0);
}

void apx::changeTextureWeightBegehbarkeit(int loop,int ID)
{
	updatePtr2[loop] = (float) ID;
	updatePtr2[loop+1] = (float) ID;
	updatePtr2[loop+2] = (float) ID;
}

void apx::generateBegehbarkeit(float steigung)
{
	int frame=0;
	for(loop=0;loop<part[frame].planezahl*3;loop+=3)
	{
		awx_vector3f p[3];
		p[0] = awx_vector3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
		p[1] = awx_vector3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
		p[2] = awx_vector3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
	
		float max=p[0].y;
		int maxI=0;
		if(p[1].y>max)
		{
			max=p[1].y;
			maxI=1;
		}
		if(p[2].y>max)
		{
			max=p[2].y;
			maxI=2;
		}
		
		float min=p[0].y;
		int minI=0;
		if(p[1].y<min)
		{
			min=p[1].y;
			minI=1;
		}
		if(p[2].y<min)
		{
			min=p[2].y;
			minI=2;
		}
		
		float dist = max-min;
		awx_vector3f oben = p[maxI];
		awx_vector3f unten = p[minI];
		
		oben.y=0;
		unten.y=0;
		
		float distHor = (oben-unten).getLength();

		if((dist/distHor)>steigung)
		{
			changeTextureWeightBegehbarkeit(loop,3);
		}
	}

}

void apx::resetBegehbarkeit(int ID)
{
	int frame=0;
	for(loop=0;loop<part[frame].planezahl*3;loop+=3)
	{
		changeTextureWeightBegehbarkeit(loop,ID);
	}
}
	
	
/*
void apx::fillVRAM(awx_vector3f lPos, bool t,float winkel)
{
    hasShadowVolume = t;

    //for(int i=0;i<texturanzahl;i++)
//	{
        //sprintf(resultPath,"%s%s%s",texturePath,texturen[i].textur,textureSuffix);
        awx_string tempstring = workingdirectory + "/" + texture_table[0].pfad + ".tga";
        load_tga(tempstring.cString(),currentID+0, true, true);
//	}
    
    IDzaehler=0;

    for(int frame=0;frame<anzahl_frames;frame++)
    {
        IDs[IDzaehler] = frame + currentID;

        IDzaehler++;

        GLfloat *vertices=new GLfloat[part[frame].planezahl*9];
        GLfloat *normals=new GLfloat[part[frame].planezahl*9];
        GLfloat *texkoords=new GLfloat[part[frame].planezahl*6];

        int normal_loop=0;
        int vertex_loop=0; 
        int tex_loop=0;
        
        v = 1;//texture_table[0].v;		// Bug
        u = 1;//texture_table[0].u;
        
        for(loop=0;loop<part[frame].planezahl*3;loop+=3)
        {
            normals[normal_loop] = part[frame].normal[part[frame].normalsequenz[loop]*3];
            normals[normal_loop+1] = part[frame].normal[part[frame].normalsequenz[loop]*3+1];
            normals[normal_loop+2] = part[frame].normal[part[frame].normalsequenz[loop]*3+2];
            
            normals[normal_loop+3] = part[frame].normal[part[frame].normalsequenz[loop+1]*3];
            normals[normal_loop+4] = part[frame].normal[part[frame].normalsequenz[loop+1]*3+1];
            normals[normal_loop+5] = part[frame].normal[part[frame].normalsequenz[loop+1]*3+2];
            
            normals[normal_loop+6] = part[frame].normal[part[frame].normalsequenz[loop+2]*3];
            normals[normal_loop+7] = part[frame].normal[part[frame].normalsequenz[loop+2]*3+1];
            normals[normal_loop+8] = part[frame].normal[part[frame].normalsequenz[loop+2]*3+2];
            
            vertices[vertex_loop] = part[frame].vertex[part[frame].plane[loop]*3];
            vertices[vertex_loop+1] = part[frame].vertex[part[frame].plane[loop]*3+1];
            vertices[vertex_loop+2] = part[frame].vertex[part[frame].plane[loop]*3+2];
            
            vertices[vertex_loop+3] = part[frame].vertex[part[frame].plane[loop+1]*3];
            vertices[vertex_loop+4] = part[frame].vertex[part[frame].plane[loop+1]*3+1];
            vertices[vertex_loop+5] = part[frame].vertex[part[frame].plane[loop+1]*3+2];
            
            vertices[vertex_loop+6] = part[frame].vertex[part[frame].plane[loop+2]*3];
            vertices[vertex_loop+7] = part[frame].vertex[part[frame].plane[loop+2]*3+1];
            vertices[vertex_loop+8] = part[frame].vertex[part[frame].plane[loop+2]*3+2];
            
            texkoords[tex_loop] = part[frame].tex_koord[part[frame].textursequenz[loop]*2]*u;
            texkoords[tex_loop+1] = part[frame].tex_koord[part[frame].textursequenz[loop]*2+1]*v;
            
            texkoords[tex_loop+2] = part[frame].tex_koord[part[frame].textursequenz[loop+1]*2]*u;
            texkoords[tex_loop+3] = part[frame].tex_koord[part[frame].textursequenz[loop+1]*2+1]*v;
            
            texkoords[tex_loop+4] = part[frame].tex_koord[part[frame].textursequenz[loop+2]*2]*u;
            texkoords[tex_loop+5] = part[frame].tex_koord[part[frame].textursequenz[loop+2]*2+1]*v;
        
            vertex_loop+=9;
            normal_loop+=9;
            tex_loop+=6;
        }
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            
        glVertexPointer(3,GL_FLOAT,0,vertices);
        glNormalPointer(GL_FLOAT,0,normals);
        glTexCoordPointer(2,GL_FLOAT,0,texkoords);
        
        glNewList(frame+currentID,GL_COMPILE);
        

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
        
            glBindTexture(GL_TEXTURE_2D,textur_liste[currentID]);
            glDrawArrays(GL_TRIANGLES,0,part[frame].planezahl*3);
        
        glEndList();
        

        delete vertices;
        delete normals;
        delete texkoords;
        
        
    }
    if(sektorflag==1) currentID++;
    else currentID+=anzahl_frames;
    
    if(project.shadowvolumes&&hasShadowVolume)
    {
        prepareShadowVolume(winkel);
    }
    
    for(int frame=0;frame<anzahl_frames;frame++)
    {
        delete part[frame].tex_koord;
        //delete part[frame].vertex;
        //delete part[frame].normal;
    }
}
*/

/*
void apx::fillVRAM(awx_vector3f lPos, bool t,float winkel)
{
    hasShadowVolume = t;

    //for(int i=0;i<texturanzahl;i++)
//	{
        //sprintf(resultPath,"%s%s%s",texturePath,texturen[i].textur,textureSuffix);
        awx_string tempstring = workingdirectory + "/" + texture_table[0].pfad + ".tga";
        load_tga(tempstring.cString(),currentID+0, true, true);
//	}
    
    IDzaehler=0;

    for(int frame=0;frame<anzahl_frames;frame++)
    {
        IDs[IDzaehler] = frame + currentID;
		
		glNewList(GL_COMPILE,frame+currentID);
		cout << "ID=" << frame+currentID << endl;

        IDzaehler++;
		
		glBegin(GL_TRIANGLES);
			for(loop=0;loop<part[frame].planezahl*3;loop+=3)
			{
				glNormal3f(part[frame].normal[part[frame].normalsequenz[loop]*3],part[frame].normal[part[frame].normalsequenz[loop]*3+1],part[frame].normal[part[frame].normalsequenz[loop]*3+2]);
				glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop]*2+1]*v);
				glVertex3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
        
				glNormal3f(part[frame].normal[part[frame].normalsequenz[loop+1]*3],part[frame].normal[part[frame].normalsequenz[loop+1]*3+1],part[frame].normal[part[frame].normalsequenz[loop+1]*3+2]);
				glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop+1]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+1]*2+1]*v);
				glVertex3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
        
				glNormal3f(part[frame].normal[part[frame].normalsequenz[loop+2]*3],part[frame].normal[part[frame].normalsequenz[loop+2]*3+1],part[frame].normal[part[frame].normalsequenz[loop+2]*3+2]);
				glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop+2]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+2]*2+1]*v);
				glVertex3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
			}
		glEnd();
		
		glEndList();

    }
    if(sektorflag==1) currentID++;
    else currentID+=anzahl_frames;
    
    for(int frame=0;frame<anzahl_frames;frame++)
    {
        delete part[frame].tex_koord;
        delete part[frame].vertex;
        delete part[frame].normal;
    }
}
*/