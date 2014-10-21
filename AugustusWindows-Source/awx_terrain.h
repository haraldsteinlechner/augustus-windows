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

#ifndef AWX_TERRAIN_H
#define AWX_TERRAIN_H


#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_vector.h"

#include "awx_project.h"
#include "globals_extern.h"

#include "awx_blob.h"

#include "awx_matrix.h"
#include "awx_cpp_utilities.h"

#include "awx_apx.h"
#include "awx_aax.h"
#include "awx_level_part.h"

#include <algorithm>

class textureTableEntry
{
	public:
	
	string name;
	int ID;
	
	textureTableEntry()
	{
		ID=-1;
		name="";
	}
	
	textureTableEntry(const textureTableEntry &t)
	{
		ID=t.ID;
		name=t.name;
	}
	
	const textureTableEntry& operator=(const textureTableEntry &x)
	{ 
		return *this;
	}
};

class awx_sektor
{
		public:
		
		vector<awx_triangle> triangles;
		
};

class awx_indexSektor
{
	public:
	
	vector<GLuint> indices;
	float height;
};

class awx_terrain: public apx
{
	public:
	
	float *begehbarkeit;
	float *texturGew;
	
	textureTableEntry textureTable[4]; 
	GLuint vboFogOfWar;
	
	awx_shader terrainshader;
	awx_shader debugshader;
	awx_sektor **sektoren;
	
	GLuint AWXTangentAttrib;
	
	float triangleSize;
	
	float sektorSize;		
	int sektorenX;
	int sektorenZ;
	
	bool loaded;
	
	float *fogOfWarPtr;
	float *vertexDataBuffer;
	GLuint *indexBuffers;
	awxPlaneEquation *planes;
	unsigned short *indexBuffersInternalCount;
	int sektorenXIndex;
	int sektorenZIndex;
	float sektorSizeIndex;
	vector<int>terrainLODVector;
	awx_indexSektor *indices;
	
    awx_terrain()
    {
        apx::apx();
		
		sektorSize = 10.0;
		loaded=false;
		triangleSize=-1;
		indexBuffers=NULL;
		indexBuffersInternalCount=NULL;
    }
	
	~awx_terrain()
	{
		delete[] sektoren;
	}

	
	int loadTerrainNeu(const char* filename)
	{
		loaded=false;
		// Datei šffnen
        FILE* input = fopen(filename,"rb");
        if(input==NULL)
        {
            return -1;
        }
        
        workingdirectory = filename;
        workingdirectory = workingdirectory.stringByDeletingLastPathComponent();
        
		
		char *header = new char[24];
		char *testheader = new char[20];
		fread(header,1,24,input);
		for(int i=0;i<20;i++)
		{
			testheader[i] = header[i];
		}
		testheader[20]='\0';
		if(strcmp(testheader,"AWXLEditorTerrain-V1")!=0)
		{	
			cout << "Falsches Terrainformat" << endl;
			return -1;
		} 
		
		cout << "Neues Terrainformat" << endl;
		delete header;
		
		awx_fread(&u,sizeof(u),1,input);
		awx_fread(&v,sizeof(v),1,input);
		
		int frame=0;
		
		part = new awx_part[1];
		
		awx_fread(&part[frame].vertexzahl,sizeof(int),1,input);
		awx_fread(&part[frame].normalenzahl,sizeof(int),1,input);
		awx_fread(&part[frame].planezahl,sizeof(int),1,input);
		awx_fread(&part[frame].texzahl,sizeof(int),1,input);
		
		part[frame].vertex = new float[part[frame].vertexzahl*3];
		
		begehbarkeit = new float[part[frame].planezahl];
		texturGew = new float[part[frame].planezahl*12];
		
		part[frame].tex_koord = new float[part[frame].texzahl*2];
		part[frame].normal = new float[part[frame].normalenzahl*3];
		part[frame].plane = new int[part[frame].planezahl*3];
		part[frame].normalsequenz = new int[part[frame].planezahl*3];
		part[frame].textursequenz = new int[part[frame].planezahl*3];
		
		xmax = 0;
        xmin = 0;
        zmin = 0;
        zmax = 0;
		ymin = 0;
		ymax = 0;
    
    
        // Daten schreiben
        for(loop=0;loop<part[frame].vertexzahl*3;loop+=3)
        {
            awx_fread(&part[frame].vertex[loop],sizeof(float),1,input);
            awx_fread(&part[frame].vertex[loop+1],sizeof(float),1,input);
            awx_fread(&part[frame].vertex[loop+2],sizeof(float),1,input);
    
            if(xmin>part[frame].vertex[loop]) xmin = part[frame].vertex[loop];
            if(zmin>part[frame].vertex[loop+2]) zmin = part[frame].vertex[loop+2];
                
            if(xmax<part[frame].vertex[loop]) xmax = part[frame].vertex[loop];
            if(zmax<part[frame].vertex[loop+2]) zmax = part[frame].vertex[loop+2];
			
			if(ymin>part[frame].vertex[loop+1]) ymin = part[frame].vertex[loop+1];
			if(ymax<part[frame].vertex[loop+1]) ymax = part[frame].vertex[loop+1];
        }
    
        
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
    
		int textureGewLoop=0;
		
		
        for(loop=0;loop<part[frame].planezahl*3;loop+=3)
        {	
            awx_fread(&part[frame].plane[loop],sizeof(int),1,input);
            awx_fread(&part[frame].plane[loop+1],sizeof(int),1,input);
            awx_fread(&part[frame].plane[loop+2],sizeof(int),1,input);
			
			/*if(loop==0||1==1)
			{
				awx_vector3f p1 = awx_vector3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
				awx_vector3f p2 = awx_vector3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
				awx_vector3f p3 = awx_vector3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
				
				float p[3];
				p[0] = (p2-p1).getHorizontalLength(); //p1p2
				p[1] = (p3-p1).getHorizontalLength(); //p1p3
				p[2] = (p3-p2).getHorizontalLength(); //p2p3
				
				float max=p[0];
				int index=0;
				if(p[1]>max)
				{
					max=p[1];
					index=1;
				}
				if(p[2]>max)
				{
					max=p[2];
					index=2;
				}
				if(index==0)
				{
					cout << "p0 winds" << p[0] << "  " << p[1] << "  " << p[2] << endl;
					triangleSize=p[1];
				}else if(index==1)
				{
					cout << "p1 winds" << p[0] << "  " << p[1] << "  " << p[2] << endl;
					triangleSize = p[2];
				}else if(index==2)
				{
					cout << "p2 winds" << p[0] << "  " << p[1] << "  " << p[2] << endl;
					triangleSize = p[0];
				}
				
				cout << "triangleSize=" << triangleSize << endl;
				
			}*/
		
            awx_fread(&part[frame].normalsequenz[loop],sizeof(int),1,input);
            awx_fread(&part[frame].normalsequenz[loop+1],sizeof(int),1,input);
            awx_fread(&part[frame].normalsequenz[loop+2],sizeof(int),1,input);
        
            awx_fread(&part[frame].textursequenz[loop],sizeof(int),1,input);
            awx_fread(&part[frame].textursequenz[loop+1],sizeof(int),1,input);
            awx_fread(&part[frame].textursequenz[loop+2],sizeof(int),1,input);
			
			for(int u=0;u<3;u++)
			{
				for(int i=0;i<4;i++)
				{
					awx_fread(&texturGew[textureGewLoop+u*4+i],sizeof(float),1,input);
				}
			}
			awx_fread(&begehbarkeit[int(loop/3)],sizeof(float),1,input);
			
			textureGewLoop+=12;
        }
	
		
		int ID=0;
		for(int i=0;i<4;i++)
		{
			int l;
			awx_fread(&l,sizeof(int),1,input);
			char *name = new char[l+1];
			fread(name,sizeof(char),l,input);
			name[l]='\0';
			sscanf(name,"%i: %s",&ID,name);
			textureTable[i].name=name;
			textureTable[i].ID=ID;
			//cout << "wild="<< i << "   " << name << "   " << ID << endl;
			delete name;
		}
		
		
		loaded=true;
		return 0;

	}
	
	void reloadLowDetailShaders()
	{
		useBumpMapping=false;
		usePerPixelLighting=false;
		if(useBumpMapping)
		{
			terrainshader.set("Data/shaders/terrain4Bump.vert","Data/shaders/terrain4Bump.frag");
			AWXTangentAttrib = glGetAttribLocationARB(terrainshader.p,"tangent");
		}
		else if(usePerPixelLighting) terrainshader.set("Data/shaders/terrain4PerPixel.vert","Data/shaders/terrain4PerPixel.frag");
		else terrainshader.set("Data/shaders/terrain4.vert","Data/shaders/terrain4.frag");		
		debugshader.set("Data/shaders/debug.vert","Data/shaders/debug.frag");
		AWXTexture0 = glGetUniformLocationARB(terrainshader.p,"texture0");
		AWXTexture1 = glGetUniformLocationARB(terrainshader.p,"texture1");
		AWXTexture2 = glGetUniformLocationARB(terrainshader.p,"texture2");
		AWXTexture3 = glGetUniformLocationARB(terrainshader.p,"texture3");
		AWXAlphaAttrib = glGetAttribLocationARB(terrainshader.p,"alphaData");
		AWXFogOfWarAttrib = glGetAttribLocationARB(terrainshader.p,"fogOfWar");
		AWXBegehbarkeitsAttrib = glGetAttribLocationARB(debugshader.p,"indexData");
	}
	
	int createVBO()
	{
		useBumpMapping=false;
		usePerPixelLighting=true;
		if(useBumpMapping)
		{
			terrainshader.set("Data/shaders/terrain4Bump.vert","Data/shaders/terrain4Bump.frag");
			AWXTangentAttrib = glGetAttribLocationARB(terrainshader.p,"tangent");
		}
		else if(usePerPixelLighting) terrainshader.set("Data/shaders/terrain4PerPixel.vert","Data/shaders/terrain4PerPixel.frag");
		else terrainshader.set("Data/shaders/terrain4.vert","Data/shaders/terrain4.frag");		
		debugshader.set("Data/shaders/debug.vert","Data/shaders/debug.frag");
		AWXTexture0 = glGetUniformLocationARB(terrainshader.p,"texture0");
		AWXTexture1 = glGetUniformLocationARB(terrainshader.p,"texture1");
		AWXTexture2 = glGetUniformLocationARB(terrainshader.p,"texture2");
		AWXTexture3 = glGetUniformLocationARB(terrainshader.p,"texture3");
		AWXAlphaAttrib = glGetAttribLocationARB(terrainshader.p,"alphaData");
		AWXFogOfWarAttrib = glGetAttribLocationARB(terrainshader.p,"fogOfWar");
		AWXBegehbarkeitsAttrib = glGetAttribLocationARB(debugshader.p,"indexData");
	
		IDs = new GLuint[4];
		
		for(int i=0;i<4;i++)
		{
			awx_string tempstring = workingdirectory.stringByAppendingString("/") + ((char*)textureTable[i].name.c_str());
			load_tga(tempstring.cString(),currentID, true, true);
			IDs[i] = currentID;
			currentID++;
		}
        
        int frame=0;
            
        GLfloat *vertices=new GLfloat[part[frame].planezahl*9];
        GLfloat *normals=new GLfloat[part[frame].planezahl*9];
        GLfloat *texkoords=new GLfloat[part[frame].planezahl*6];
        GLfloat *fogOfWar=new GLfloat[part[frame].planezahl*3];
		GLfloat *begehbarkeitPerVertex=new GLfloat[part[frame].planezahl*3];
		planes=new awxPlaneEquation[part[frame].planezahl];
		
		GLfloat *tangents;
		if(useBumpMapping) tangents = new GLfloat[part[frame].planezahl*3];
    
        int normal_loop=0;
        int vertex_loop=0; 
        int tex_loop=0; 
        int r=0;
		
		awx_vector3f t,b,n;

            
        for(loop=0;loop<part[frame].planezahl*3;loop+=3)
		{
		
            fogOfWar[r] = 0.0;
            fogOfWar[r+1] = 0.0;
            fogOfWar[r+2] = 0.0;
			
			begehbarkeitPerVertex[r]=begehbarkeit[int(loop/3)];
			begehbarkeitPerVertex[r+1]=begehbarkeit[int(loop/3)];
			begehbarkeitPerVertex[r+2]=begehbarkeit[int(loop/3)];
            
            
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
			
			if(useBumpMapping)
			{
				float pos1[] = {part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]};
				float pos2[] = {part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]};
				float pos3[] = {part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]};
			
				float tex1[] = {part[frame].tex_koord[part[frame].textursequenz[loop]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop]*2+1]*v};
				float tex2[] = {part[frame].tex_koord[part[frame].textursequenz[loop+1]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+1]*2+1]*v};
				float tex3[] = {part[frame].tex_koord[part[frame].textursequenz[loop+2]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+2]*2+1]*v};

				CalcTangentSpaceVectors(pos1,pos2,pos3,tex1,tex2,tex3,t,b,n);
				
				vertices[vertex_loop+0] = t.x;
				vertices[vertex_loop+1] = t.y;
				vertices[vertex_loop+2] = t.z;
                    
				vertices[vertex_loop+3] = t.x;
				vertices[vertex_loop+4] = t.y;
				vertices[vertex_loop+5] = t.z;
                    
				vertices[vertex_loop+6] = t.x;
				vertices[vertex_loop+7] = t.y;
				vertices[vertex_loop+8] = t.z;
			}
                
            vertex_loop+=9;
            normal_loop+=9;
            tex_loop+=6;
			
			r+=3;
			
			
			// Calculate Plane
			awx_vector3f p1(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
			awx_vector3f p2(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
			awx_vector3f p3(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
		
			
			awxCalcPlane2(p1,p2,p3,&planes[int(loop/3)]);
        }
                
        // VBO Part
        glGenBuffersARB(1,&vboVertex);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertex);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*9*sizeof(float),vertices,GL_STATIC_DRAW_ARB);
            
        glGenBuffersARB(1,&vboTexCoord);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexCoord);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*6*sizeof(float),texkoords,GL_STATIC_DRAW_ARB);
        
        glGenBuffersARB(1,&vboFogOfWar);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboFogOfWar);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*sizeof(float)*3,fogOfWar,GL_DYNAMIC_DRAW_ARB);
            
        glGenBuffersARB(1,&vboNormal);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormal);
            
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*9*sizeof(float),normals,GL_STATIC_DRAW_ARB);
		
        glGenBuffersARB(1,&vboTextureWeights);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTextureWeights);
	
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*12*sizeof(float),texturGew,GL_STATIC_DRAW_ARB);
		
		
		if(useBumpMapping)
		{
			glGenBuffersARB(1,&vboTangents);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTangents);
		
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*9*sizeof(float),tangents,GL_STATIC_DRAW_ARB);
		}
		
		glGenBuffersARB(1,&vboBegehbarkeit);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboBegehbarkeit);
		
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,part[frame].planezahl*sizeof(float)*3,begehbarkeitPerVertex,GL_STATIC_DRAW_ARB);
		
        delete[] vertices;
        delete[] texkoords;
        delete[] normals;
		
		generateIndexBuffers();
		sektorize();
		
		delete[] part[frame].tex_koord; 
		//delete[] part[frame].normal;
		//delete[] part[frame].plane;
		//delete[] part[frame].normalsequenz;
		delete[] part[frame].textursequenz;
		
		//#warning dealloc
		
		if(useBumpMapping) delete[] tangents;
		
		//delete[] begehbarkeit;
		delete[] texturGew;
		delete[] begehbarkeitPerVertex;
		delete[] fogOfWar;
		
		return 0;
    }    
	
		
	awx_vector3f intersect(awxRay &viewRay)
	{
		awx_vector3f intersectionPoint;
		float Vd,V0,t;
		awxPlaneEquation *plane;
		
		/*float hitF;
		bool test;
		int x,z;
		awx_vector3f lowestHit;
		
		float minT=0;*/

		int frame=0;
		
		if(!loaded)return intersectionPoint;
		
		 for(int loop=0;loop<part[0].planezahl*3;loop+=3)
		 {
				plane = &planes[int(loop/3)];
		
				Vd = plane->n.scalarProduct(viewRay.Rd);
			
				V0 = -(plane->n.scalarProduct(viewRay.R0) + plane->d);
		
				t = V0/Vd;
		
				if(t<0) // hinter ray
				{
					continue;
				}
		

				intersectionPoint.set(viewRay.R0.x + viewRay.Rd.x*t, viewRay.R0.y + viewRay.Rd.y*t, viewRay.R0.z + viewRay.Rd.z*t);
			
				/*getSektor(intersectionPoint,x,z);

				if(x<0||x>=sektorenX||z<0||z>=sektorenZ) continue;
				
				
				hit(intersectionPoint , hitF , test, x, z);
				if(test&&fabs(hitF-intersectionPoint.y)<0.01)
				{
					//intersectionPoint.y=hitF;
					intersectionPoint.y=hitF;
							cout << SDL_GetTicks() - start << endl;
					return intersectionPoint;
				} */
				
				
				awx_vector3f p0(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
				awx_vector3f p1(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
				awx_vector3f p2(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
		
				awx_vector3f n = (p1-p0).crossProduct(p2-p0);
				
				float v1 = ((p1-p0).crossProduct(intersectionPoint-p0)).scalarProduct(n);
				if(v1<0) continue;
				
				v1 = ((p2-p1).crossProduct(intersectionPoint-p1)).scalarProduct(n);
				if(v1<0) continue;
				
				v1 = ((p0-p2).crossProduct(intersectionPoint-p2)).scalarProduct(n);
				if(v1<0) continue;
				
				return intersectionPoint;
		}
		
		
		return intersectionPoint;
	}
        
    void render(bool useDefaultShader=true)
    {	
		if(useDefaultShader) terrainshader.install();
		
	
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[0].ID]);
		glUniform1iARB(AWXTexture0,textureTable[0].ID);
		
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[1].ID]);
		glUniform1iARB(AWXTexture1,textureTable[1].ID);
		
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[2].ID]);
		glUniform1iARB(AWXTexture2,textureTable[2].ID);
	
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[3].ID]);
		glUniform1iARB(AWXTexture3,textureTable[3].ID);
	
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableVertexAttribArrayARB(AWXAlphaAttrib);
		glEnableVertexAttribArrayARB(AWXFogOfWarAttrib);
        
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertex);
        glVertexPointer(3,GL_FLOAT,0,NULL);
		
            
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormal);
        glNormalPointer(GL_FLOAT,0,NULL);
            
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexCoord);
        glTexCoordPointer(2,GL_FLOAT,0,NULL);
		
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTextureWeights);
        glVertexAttribPointerARB(AWXAlphaAttrib,4,GL_FLOAT,0,0,NULL);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboFogOfWar);
		glVertexAttribPointerARB(AWXFogOfWarAttrib,1,GL_FLOAT,0,0,NULL);
		
        glDrawArrays(GL_TRIANGLES,0,part[0].planezahl*3);

            
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableVertexAttribArrayARB(AWXAlphaAttrib);
		glDisableVertexAttribArrayARB(AWXFogOfWarAttrib);
		
		for(int i=4;i>=1;i--)
		{
			glActiveTexture(GL_TEXTURE0+i);
			glDisable(GL_TEXTURE_2D);
		}
		
		glActiveTexture(GL_TEXTURE0);
		
		terrainshader.deinstall();
	}
	
	void generateIndexBuffers()
	{
		cout << endl << "Generate Indexbuffers..." << endl;
		Uint32 start = SDL_GetTicks();
		sektorSizeIndex=18;
		int frame=0;
		
		sektorenXIndex = int((xmax-xmin)/sektorSizeIndex+1)+1;
		sektorenZIndex = int((zmax-zmin)/sektorSizeIndex+1)+1;
		
		cout << "sektoren: " << sektorenXIndex << "/" << sektorenZIndex << endl;
		
		indexBuffers = new GLuint[sektorenXIndex*sektorenZIndex];
		indexBuffersInternalCount = new unsigned short[sektorenXIndex*sektorenZIndex];
		indices = new awx_indexSektor[sektorenXIndex*sektorenZIndex];
		
		
		awx_vector3f vertex;
		int xI,zI;
		awx_vector3f min = awx_vector3f(xmin , 0 , zmin);
        for(unsigned int loop=0;loop<part[frame].planezahl*3;loop+=3)
        {			
			//cout << "zuzling" << endl;
            vertex = awx_vector3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]); 
			//cout << "calcing" << endl;
			xI = ceil((vertex.x-xmin)/(float)sektorSizeIndex);
			zI = ceil((vertex.z-zmin)/(float)sektorSizeIndex);
			
			//cout << xI << "/" << zI << "  " << vertex << endl;
			
			if(xI<0||xI>sektorenXIndex||zI<0||zI>sektorenZIndex) 
			{
				cout << "Problem in DReieck:" << loop  << "  xi=" << xI << "  " << zI << "   "<< vertex << endl;
				continue;
			}
			
			//cout << "pushing:" << zI*sektorenXIndex+xI << endl;
			int vertexNumber = int(loop);
			indices[int(zI*sektorenXIndex+xI)].indices.push_back(vertexNumber);
			indices[int(zI*sektorenXIndex+xI)].indices.push_back(vertexNumber+1);
			indices[int(zI*sektorenXIndex+xI)].indices.push_back(vertexNumber+2);
			indices[int(zI*sektorenXIndex+xI)].height=vertex.y;
			
			//cout << "pushed" << endl;
		}

		cout << "Filling Buffers.." << endl;
		GLushort *data = NULL;
		for(int x=0;x<sektorenXIndex;x++)
		{
			for(int z=0;z<sektorenZIndex;z++)
			{
				if(data!=NULL)
				{
					delete[] data;
				}
				
				data = new GLushort[indices[z*sektorenXIndex+x].indices.size()];
				for(int i=0;i<indices[z*sektorenXIndex+x].indices.size();i++)
				{
					data[i]=(GLushort)indices[z*sektorenXIndex+x].indices[i];
				}
			

				glGenBuffersARB(1,&indexBuffers[z*sektorenXIndex+x]);
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,indexBuffers[z*sektorenXIndex+x]);
				glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,indices[z*sektorenXIndex+x].indices.size()*sizeof(GLushort),data,GL_STATIC_DRAW);
				
				indexBuffersInternalCount[z*sektorenXIndex+x]=indices[z*sektorenXIndex+x].indices.size();
				indices[z*sektorenXIndex+x].indices.clear();
				
			}
		}
		
		cout << "CPU Done." << endl;
		
		delete[] data;
		//delete[] indices;
		
		cout << "Cleanup Done. Complete: " << (SDL_GetTicks()-start)/1000.0 << endl;
	
	}
	
	void cull()
	{
		//cout << "culll.." << wx << "  " << wy << "   " << wz << endl;
		//Uint32 start = SDL_GetTicks();
		ExtractFrustum();
		
		float half = sektorSizeIndex/2.0;
		int val=0;
		
		terrainLODVector.clear();
		int xi,zi;
		
		float factor=1.3;
		
		for(int x=0;x<sektorenXIndex;x++)
		{
			for(int z=0;z<sektorenZIndex;z++)
			{
				xi=x-0;
				zi=z-0;
				if(SphereInFrustum((xmin+(xi*sektorSizeIndex-half*factor)),indices[z*sektorenXIndex+x].height,zmin+(zi*sektorSizeIndex-half*factor),half*factor))
				{
					val = getLinearSektorFromXZ(x,z);
					if(find(terrainLODVector.begin(),terrainLODVector.end(),val)==terrainLODVector.end())
					{
						terrainLODVector.push_back(val);
					}
				}
			}
		}
		
		//cout << "Culling in " << (SDL_GetTicks()-start)/1000.0 << endl;
	}
	
	void cull2()
	{
		terrainLODVector.clear();
		int val=0;
		GLdouble winx,winy,winz;
		int xi,zi;
		float tol=100;
		float tol2=0;
		float tol3=80;
		
		for(int x=0;x<sektorenXIndex;x++)
		{
			for(int z=0;z<sektorenZIndex;z++)
			{
				xi=x-1;
				zi=z-1;
				gluProject((xmin+(xi*sektorSizeIndex)),indices[z*sektorenXIndex+x].height,zmin+(zi*sektorSizeIndex),project.mvmatrix,project.projmatrix,project.viewport,&winx,&winy,&winz);
				
				if(winx>-tol&&winx<project.viewport[2]*+tol&&winy>-tol3&&winy<project.viewport[3]+tol2&&winz>0&&winz<0.975)
				{
					val = getLinearSektorFromXZ(x,z);
					if(find(terrainLODVector.begin(),terrainLODVector.end(),val)==terrainLODVector.end())
					{
						terrainLODVector.push_back(val);
					}
				} 
			}
		}
	}
	
	int getIndexSectorFromWorldCoordinates(awx_vector3f w)
	{
		int xI = ceil((w.x-xmin)/(float)sektorSizeIndex);
		int zI = ceil((w.z-zmin)/(float)sektorSizeIndex);
		
		//cout << zI*sektorenXIndex+xI << "  " << xI << "  " << zI << "      " << w <<endl;
		int ret = zI*sektorenXIndex+xI;
		
		if(ret<0||ret>=sektorenZIndex*sektorenXIndex) return -1;
		
		return ret;
	}
	
	int getXIFromWorldCoordinates(awx_vector3f w)
	{
		int xI = ceil((w.x-xmin)/(float)sektorSizeIndex);
		
		if(xI<0||xI>=sektorenXIndex) return -1;
		
		return xI;
	}
	
	int getZIFromWorldCoordinates(awx_vector3f w)
	{
		int zI = ceil((w.z-zmin)/(float)sektorSizeIndex);

		if(zI<0||zI>=sektorenZIndex) return -1;
		
		return zI;
	}
	
	int getLinearSektorFromXZ(int x,int z)
	{
		return (sektorenXIndex*z+x);
	}
	
	void renderSektorWithIndex(int index)
	{
		terrainshader.install();
		
	
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[0].ID]);
		glUniform1iARB(AWXTexture0,textureTable[0].ID);
		
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[1].ID]);
		glUniform1iARB(AWXTexture1,textureTable[1].ID);
		
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[2].ID]);
		glUniform1iARB(AWXTexture2,textureTable[2].ID);
	
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[3].ID]);
		glUniform1iARB(AWXTexture3,textureTable[3].ID);
	
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableVertexAttribArrayARB(AWXAlphaAttrib);
		glEnableVertexAttribArrayARB(AWXFogOfWarAttrib);
        
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertex);
        glVertexPointer(3,GL_FLOAT,0,NULL);
            
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormal);
        glNormalPointer(GL_FLOAT,0,NULL);
            
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexCoord);
        glTexCoordPointer(2,GL_FLOAT,0,NULL);
		
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTextureWeights);
        glVertexAttribPointerARB(AWXAlphaAttrib,4,GL_FLOAT,0,0,NULL);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboFogOfWar);
		glVertexAttribPointerARB(AWXFogOfWarAttrib,1,GL_FLOAT,0,0,NULL);
		
		
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,indexBuffers[index]);

		glDrawRangeElements(GL_TRIANGLES,0,indexBuffersInternalCount[index],indexBuffersInternalCount[index],GL_UNSIGNED_SHORT,NULL);
		//glDrawRangeElementArrayAPPLE(GL_TRIANGLES,0,indexBuffersInternalCount[index],0,indexBuffersInternalCount[index]);
		//glDrawElements(GL_TRIANGLES,indexBuffersInternalCount[index] ,GL_UNSIGNED_INT,NULL);

            
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableVertexAttribArrayARB(AWXAlphaAttrib);
		glDisableVertexAttribArrayARB(AWXFogOfWarAttrib);
		
		for(int i=4;i>=1;i--)
		{
			glActiveTexture(GL_TEXTURE0+i);
			glDisable(GL_TEXTURE_2D);
		}
		
		glActiveTexture(GL_TEXTURE0);
		
		terrainshader.deinstall();
	}
	
	void renderSektorWithIndexVector(vector<int> &indices)
	{
		terrainshader.install();
		
	
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[0].ID]);
		glUniform1iARB(AWXTexture0,textureTable[0].ID);
		
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[1].ID]);
		glUniform1iARB(AWXTexture1,textureTable[1].ID);
		
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[2].ID]);
		glUniform1iARB(AWXTexture2,textureTable[2].ID);
	
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[3].ID]);
		glUniform1iARB(AWXTexture3,textureTable[3].ID);
	
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableVertexAttribArrayARB(AWXAlphaAttrib);
		glEnableVertexAttribArrayARB(AWXFogOfWarAttrib);
        
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertex);
        glVertexPointer(3,GL_FLOAT,0,NULL);
            
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormal);
        glNormalPointer(GL_FLOAT,0,NULL);
            
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexCoord);
        glTexCoordPointer(2,GL_FLOAT,0,NULL);
		
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTextureWeights);
        glVertexAttribPointerARB(AWXAlphaAttrib,4,GL_FLOAT,0,0,NULL);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboFogOfWar);
		glVertexAttribPointerARB(AWXFogOfWarAttrib,1,GL_FLOAT,0,0,NULL);
		
		if(useBumpMapping)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTangents);
			glVertexAttribPointerARB(AWXTangentAttrib,3,GL_FLOAT,0,0,NULL);
		}
		
		int index=0;
		for(int i=0;i<indices.size();i++)
		{
			index=indices[i];
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,indexBuffers[index]);

			glDrawRangeElements(GL_TRIANGLES,0,indexBuffersInternalCount[index],indexBuffersInternalCount[index],GL_UNSIGNED_SHORT,NULL);
			//glDrawRangeElementArrayAPPLE(GL_TRIANGLES,0,indexBuffersInternalCount[index],0,indexBuffersInternalCount[index]);
			//glDrawElements(GL_TRIANGLES,indexBuffersInternalCount[index] ,GL_UNSIGNED_INT,NULL);
		}

            
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableVertexAttribArrayARB(AWXAlphaAttrib);
		glDisableVertexAttribArrayARB(AWXFogOfWarAttrib);
		
		for(int i=4;i>=1;i--)
		{
			glActiveTexture(GL_TEXTURE0+i);
			glDisable(GL_TEXTURE_2D);
		}
		
		glActiveTexture(GL_TEXTURE0);
		
		terrainshader.deinstall();
	}

	
	void renderRect()
    {
		terrainshader.install();
		
	
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[0].ID]);
		glUniform1iARB(AWXTexture0,textureTable[0].ID);
		
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[1].ID]);
		glUniform1iARB(AWXTexture1,textureTable[1].ID);
		
		glActiveTexture(GL_TEXTURE2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[2].ID]);
		glUniform1iARB(AWXTexture2,textureTable[2].ID);
	
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textur_liste[IDs[0]+textureTable[3].ID]);
		glUniform1iARB(AWXTexture3,textureTable[3].ID);
	
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableVertexAttribArrayARB(AWXAlphaAttrib);
		glEnableVertexAttribArrayARB(AWXFogOfWarAttrib);
        
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertex);
        glVertexPointer(3,GL_FLOAT,0,NULL);
            
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormal);
        glNormalPointer(GL_FLOAT,0,NULL);
            
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTexCoord);
        glTexCoordPointer(2,GL_FLOAT,0,NULL);
		
        glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboTextureWeights);
        glVertexAttribPointerARB(AWXAlphaAttrib,4,GL_FLOAT,0,0,NULL);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboFogOfWar);
		glVertexAttribPointerARB(AWXFogOfWarAttrib,1,GL_FLOAT,0,0,NULL);
		
        glDrawArrays(GL_TRIANGLES,500*3,500*3+12*3);

            
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableVertexAttribArrayARB(AWXAlphaAttrib);
		glDisableVertexAttribArrayARB(AWXFogOfWarAttrib);
		
		for(int i=4;i>=1;i--)
		{
			glActiveTexture(GL_TEXTURE0+i);
			glDisable(GL_TEXTURE_2D);
		}
		
		glActiveTexture(GL_TEXTURE0);
		
		terrainshader.deinstall();
	}
	
	void renderDebug()
	{
		debugshader.install();
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableVertexAttribArrayARB(AWXBegehbarkeitsAttrib);
	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertex);
		glVertexPointer(3,GL_FLOAT,0,NULL);
				
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboBegehbarkeit);
		glVertexAttribPointerARB(AWXBegehbarkeitsAttrib,1,GL_FLOAT,0,0,NULL);
		
		glDrawArrays(GL_TRIANGLES,0,part[0].planezahl*3);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableVertexAttribArrayARB(AWXBegehbarkeitsAttrib);
		
		debugshader.deinstall();
	}
	
    void sektorize()
    {
		Uint32 start = SDL_GetTicks();

		cout << "Sektorisieren..." << endl;
		sektorSize = 10.0;
		
		sektorenX = int((xmax-xmin)/sektorSize+1);
		sektorenZ = int((zmax-zmin)/sektorSize+1);
		
		sektoren = new awx_sektor*[sektorenX];
		for(int i=0;i<sektorenX;i++)
		{
			sektoren[i] = new awx_sektor[sektorenZ];
		}
		
	
        int frame=0;
        awx_triangle theTriangle;
    
        for(loop=0;loop<part[frame].planezahl*3;loop+=3)
        {
			theTriangle.setf(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2],
							 part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2],
							 part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);

            awx_vector3f min = awx_vector3f(xmin , 0 , zmin);
			
			awx_vector3f p1 = (theTriangle.p1 - min)/sektorSize;
			awx_vector3f p2 = (theTriangle.p2 - min)/sektorSize;
			awx_vector3f p3 = (theTriangle.p3 - min)/sektorSize;
			
			int x1 = (int)p1.getX();
			int z1 = (int)p1.getZ();
			
			int x2 = (int)p2.getX();
			int z2 = (int)p2.getZ();
			
			int x3 = (int)p3.getX();
			int z3 = (int)p3.getZ();
			
			theTriangle.data=loop;
			
			sektoren[x1][z1].triangles.push_back(theTriangle);
			if(x2 != x1 || z2 != z1)sektoren[x2][z2].triangles.push_back(theTriangle);
			if((x3 != x1 || z3 != z1) && (x3 != x2 || z3 != z2))
			{
				if((x2 + (x3 - x1))>=0 && (z2 + (z3 - z1))>=0 && (z2 + (z3 - z1)) < sektorenZ && (x2 + (x3 - x1)) < sektorenX)
				{
					sektoren[x2 + (x3 - x1)][z2 + (z3 - z1)].triangles.push_back(theTriangle);
				}
				sektoren[x2][z2].triangles.push_back(theTriangle);
			}
		
		}
		
		cout << "Sektorisiert mit " << sektorenX << "/" << sektorenZ << "  in " << (SDL_GetTicks()-start)/1000.0 << " Sekunden." << endl;
    }
	
	void getSektor(awx_vector3f &point,int &x, int &z)
	{
		z=(point.getZ()-zmin)/sektorSize;
		x=(point.getX()-xmin)/sektorSize;
	}
	
	void display_sektor(awx_vector3f point)
	{
		if(!loaded) return;

		int x,z;
		getSektor(point,x,z);
	
		awx_sektor* temp = &sektoren[x][z];

		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glBegin(GL_TRIANGLES);
		for(int i=0;i<(int)(*temp).triangles.size();i++)
		{
			glVertex3f((*temp).triangles[i].p1.x,(*temp).triangles[i].p1.y,(*temp).triangles[i].p1.z);
			glVertex3f((*temp).triangles[i].p2.x,(*temp).triangles[i].p2.y,(*temp).triangles[i].p2.z);
			glVertex3f((*temp).triangles[i].p3.x,(*temp).triangles[i].p3.y,(*temp).triangles[i].p3.z);
		}
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
	
	void display_sektor(int x,int z)
	{
		awx_sektor* temp = &sektoren[x][z];

		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glBegin(GL_TRIANGLES);
		for(int i=0;i<(int)(*temp).triangles.size();i++)
		{
			glVertex3f((*temp).triangles[i].p1.x,(*temp).triangles[i].p1.y,(*temp).triangles[i].p1.z);
			glVertex3f((*temp).triangles[i].p2.x,(*temp).triangles[i].p2.y,(*temp).triangles[i].p2.z);
			glVertex3f((*temp).triangles[i].p3.x,(*temp).triangles[i].p3.y,(*temp).triangles[i].p3.z);
		}
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
	
	void renderSektor(awx_vector3f point)
	{
		int x,z;
		getSektor(point,x,z);
	
		
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glBegin(GL_QUADS);
		glVertex3f(xmin + x*sektorSize,point.y+1,zmin + z*sektorSize);
		glVertex3f(xmin + (x+1)*sektorSize,point.y+1,zmin + z*sektorSize);
		glVertex3f(xmin + (x+1)*sektorSize,point.y+1,zmin + (z+1)*sektorSize);
		glVertex3f(xmin + x*sektorSize,point.y+1,zmin + (z+1)*sektorSize);

		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
	
	
	int hit(awx_vector3f point , float &hit , bool &test, int &xP, int &zP)
	{
	
		if(!loaded) return 0;
		
		int x,z;
		getSektor(point,x,z);
		
		xP=x;
		zP=z;

		int loop;
	
		awx_sektor* temp = &sektoren[x][z];
		
		for(loop = 0; loop < (int)(*temp).triangles.size();loop++)
		{
			test = awx_triangle_hitTest(temp->triangles[loop] , point , hit);
			if(hit != AWXFalse)return loop;
			else hit=point.y;
		}

		// Keine hit gefunden
		return -1;
		
	}

	void average(int hitTriangle,int xp,int zp, awx_vector3f &pos, int kernelSize, float radius)
	{		
		//Uint32 start=SDL_GetTicks();
		
		awx_vector3f *punkt[3];
		float distance=0;
		
		vector<int>usedVertices;
		int vertexNumber=0;
		
		bool result=true;
		vector<int>::iterator resIt;
		float tempFloat=0;

		for(int i=0;i<sektorenX;i++)
		{
			if(i<0||i>=sektorenX) continue;
			for(int u=0;u<sektorenZ;u++)
			{
				if(u<0||u>=sektorenZ) continue;
				
				for(int r=0;r<sektoren[i][u].triangles.size();r++)
				{
					punkt[0] = &sektoren[i][u].triangles[r].p1;
					punkt[1] = &sektoren[i][u].triangles[r].p2;
					punkt[2] = &sektoren[i][u].triangles[r].p3;
					
					result = true;
					/*for(int g=0;g<usedVertices.size();g++)
					{
						if(usedVertices[g]==sektoren[i][u].triangles[r].data)
						{
							result = false;
							break;
						}
					}*/
					/*
					if(find(usedVertices.begin(),usedVertices.end(),sektoren[i][u].triangles[r].data)==usedVertices.end())
					{
						result = true;
					} else result=false;*/

					if(result)
					{
						for(int z=0;z<3;z++)
						{
							vertexNumber = sektoren[i][u].triangles[r].data+z;

							distance = fabs((*punkt[z]-pos).getHorizontalLengthXZ());
							
							if(distance>radius) continue;

							tempFloat=2.0*(1.0-(distance/radius));
							setHeightForTriangle(vertexNumber,pos,tempFloat);
						}
					}
				}
			}
		}
		
		//cout << "CPU and VRAM fow update in " << (SDL_GetTicks()-start)/1000.0 << "  involved=" << usedVertices.size() << endl;
		usedVertices.clear();
	}	

	void setHeightForTriangle(int vertexNumber,awx_vector3f &pos, float fac)
	{
		vertexDataBuffer[vertexNumber*3+1]=pos.y;//(pos.y-vertexDataBuffer[vertexNumber*3+1]) * fac;
	}
	
	void startUpdateFogOfWar()
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboFogOfWar);
		fogOfWarPtr = (float*) glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_READ_WRITE_ARB);
	}
	
	void stopUpdateFogOfWar()
	{
		glBindBuffer(GL_ARRAY_BUFFER_ARB,vboFogOfWar);
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		glBindBuffer(GL_ARRAY_BUFFER_ARB,0);
	}
	
	void startUpdateGeometry()
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboVertex);
		vertexDataBuffer = (float*) glMapBufferARB(GL_ARRAY_BUFFER_ARB,GL_READ_WRITE_ARB);
	}
	
	void stopUpdateGeometry()
	{
		glBindBuffer(GL_ARRAY_BUFFER_ARB,vboVertex);
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		glBindBuffer(GL_ARRAY_BUFFER_ARB,0);
	}
	
	void smoothGeometry()
	{
		startUpdateGeometry();
		
			/*awx_vector3f ps = awx_vector3f(x,y,z);
	int currentSektorX,currentSektorZ;
	bool ergebnisflag;
	hitTriangle=currentLevel->terrain.hit(ps , ps.y , ergebnisflag, currentSektorX, currentSektorZ);
	if(hitTriangle>=0) currentLevel->terrain.doFogOfWarForTriangleInSektor(hitTriangle,currentSektorX,currentSektorZ,ps,3,20,1.0,true);*/
		
		stopUpdateGeometry();
	}
	
	void fillFogOfWar(float value)
	{
		startUpdateFogOfWar();
		
		for(int i=0;i<part[0].planezahl*3;i++)
		{
			fogOfWarPtr[i]=value;
		}
		
		stopUpdateFogOfWar();
	}
	
	
	void darkenEdge()
	{
		awx_vector3f *punkt[3];
		float epsilon=0.045;
	
		startUpdateFogOfWar();
		for(int i=0;i<sektorenX;i++)
		{
			for(int u=0;u<sektorenZ;u++)
			{
				if(1||i>=sektorenX||u>=sektorenZ||i==0||u==0)
				{
					for(int r=0;r<sektoren[i][u].triangles.size();r++)
					{
						punkt[0] = &sektoren[i][u].triangles[r].p1;
						punkt[1] = &sektoren[i][u].triangles[r].p2;
						punkt[2] = &sektoren[i][u].triangles[r].p3;
						
						for(int o=0;o<=2;o++)
						{
							if(closeEnough(punkt[o]->x,xmin,epsilon)||closeEnough(punkt[o]->z,zmin,epsilon)||
							closeEnough(punkt[o]->x,xmax,epsilon)||closeEnough(punkt[o]->z,zmax,epsilon))
						   
							fogOfWarPtr[sektoren[i][u].triangles[r].data+o] = 0.0;
							
							if(closeEnough(punkt[o]->x,xmin,epsilon)||closeEnough(punkt[o]->z,zmin,epsilon)||
							closeEnough(punkt[o]->x,xmax,epsilon)||closeEnough(punkt[o]->z,zmax,epsilon))
							
							sektoren[i][u].triangles[r].triangleSeen=false;
						}
						   
						
					}
				}
			}
		}
		stopUpdateFogOfWar();
	}
	
	void setFogOfWarForTriangleInc(int vertex,float value)
	{
		if(value<0) return;
		fogOfWarPtr[vertex]+=value;
		if(fogOfWarPtr[vertex]>1.0) fogOfWarPtr[vertex]=1.0;
	}
	
	void setFogOfWarForTriangle(int vertex,float value)
	{
		if(fogOfWarPtr[vertex]<value)
		fogOfWarPtr[vertex]=value;
	}
	
	void setFogOfWarForTriangleABS(int vertex,float value)
	{
		if(fogOfWarPtr[vertex]<value)
		fogOfWarPtr[vertex]=value;
	}
	
	void setFogOfWarForTriangleHALV(int vertex,float value)
	{
		if(fogOfWarPtr[vertex]>0.5)
		fogOfWarPtr[vertex]-=value;
	}
	
	int setFogOfWarForPoint(awx_vector3f &point)
	{
	
		if(!loaded) return 0;
		
		int x,z;
		getSektor(point,x,z);

		int loop;
		bool test;
	
		awx_sektor* temp = &sektoren[x][z];
		float hitF;
		
		for(loop = 0; loop < (int)(*temp).triangles.size();loop++)
		{
			test = awx_triangle_hitTest(temp->triangles[loop] , point , hitF);
			if(hitF != AWXFalse)return loop;
		}

		return loop;
		
	}
	
	void doFogOfWarForTriangleInSektor(int hitTriangle,int xp,int zp, awx_vector3f &pos, int kernelSize, float radius, float speed, bool allwaysV)
	{		
		//Uint32 start=SDL_GetTicks();
		
		awx_vector3f *punkt[3];
		float distance=0;
		
		vector<int>usedVertices;
		int vertexNumber=0;
		
		bool result=true;
		vector<int>::iterator resIt;
		float tempFloat=0;

		for(int i=xp-kernelSize;i<=xp+kernelSize;i++)
		{
			if(i<0||i>=sektorenX) continue;
			for(int u=zp-kernelSize;u<=zp+kernelSize;u++)
			{
				if(u<0||u>=sektorenZ) continue;
				
				for(int r=0;r<sektoren[i][u].triangles.size();r++)
				{
					punkt[0] = &sektoren[i][u].triangles[r].p1;
					punkt[1] = &sektoren[i][u].triangles[r].p2;
					punkt[2] = &sektoren[i][u].triangles[r].p3;
					
					result = true;
					/*for(int g=0;g<usedVertices.size();g++)
					{
						if(usedVertices[g]==sektoren[i][u].triangles[r].data)
						{
							result = false;
							break;
						}
					}*/
					if(find(usedVertices.begin(),usedVertices.end(),sektoren[i][u].triangles[r].data)==usedVertices.end())
					{
						result = true;
					} else result=false;

					if(result)
					{
						int zz=0;
						for(int z=0;z<3;z++)
						{
							vertexNumber = sektoren[i][u].triangles[r].data+z;

							distance = fabs((*punkt[z]-pos).getHorizontalLengthXZ());
							
							if(distance>radius) continue;
							zz++;
							
							if(z==2) // i check eher nid warum
							usedVertices.push_back(sektoren[i][u].triangles[r].data);
							
							if(speed<1.0)
							{
								setFogOfWarForTriangleInc(vertexNumber,fabs(speed*(1.0-(distance/radius))));
							}
							else
							{
								tempFloat=2.0*(1.0-(distance/radius));
								if(tempFloat>1.0) tempFloat=1.0;
								if(tempFloat<fowDarkValue) tempFloat=fowDarkValue;
								setFogOfWarForTriangle(vertexNumber,tempFloat);
							}
						}
						
						if(zz>0)
						{
							sektoren[i][u].triangles[r].triangleSeen=true;
							sektoren[i][u].triangles[r].timeSinceLastView=0;
							if(!sektoren[i][u].triangles[r].allwaysVisible)
							sektoren[i][u].triangles[r].allwaysVisible=allwaysV;
						}
					}
				}
			}
		}
		
		//cout << "CPU and VRAM fow update in " << (SDL_GetTicks()-start)/1000.0 << "  involved=" << usedVertices.size() << endl;
		usedVertices.clear();
	}
	
	void doFogOfWarForTriangleInSektor(int hitTriangle,int xp,int zp, awx_vector3f &pos, int kernelSize, float radius, bool allwaysV)
	{		
		//Uint32 start=SDL_GetTicks();
		
		awx_vector3f *punkt[3];
		float distance=0;
		
		int vertexNumber=0;
		float tempFloat=0;

		for(int i=xp-kernelSize;i<=xp+kernelSize;i++)
		{
			if(i<0||i>=sektorenX) continue;
			for(int u=zp-kernelSize;u<=zp+kernelSize;u++)
			{
				if(u<0||u>=sektorenZ) continue;
				
				for(int r=0;r<sektoren[i][u].triangles.size();r++)
				{
					punkt[0] = &sektoren[i][u].triangles[r].p1;
					punkt[1] = &sektoren[i][u].triangles[r].p2;
					punkt[2] = &sektoren[i][u].triangles[r].p3;
					
					int zz=0;
					for(int z=0;z<3;z++)
					{
						vertexNumber = sektoren[i][u].triangles[r].data+z;

						distance = fabs((*punkt[z]-pos).getHorizontalLengthXZ());
					
						if(distance>radius) continue;
						
						zz++;

						tempFloat=2.0*(1.0-(distance/radius));
						if(tempFloat>1.0) tempFloat=1.0;
						setFogOfWarForTriangleABS(vertexNumber,tempFloat);
						
					}
					
					if(zz>0)
					{
						if(!sektoren[i][u].triangles[r].allwaysVisible)
						sektoren[i][u].triangles[r].allwaysVisible=allwaysV;
						
						
						sektoren[i][u].triangles[r].triangleSeen=true;
						sektoren[i][u].triangles[r].timeSinceLastView=0;
					}
				}
			}
		}
		
		//cout << "CPU and VRAM fow update in " << (SDL_GetTicks()-start)/1000.0 << "  involved=" << usedVertices.size() << endl;
	}
	
	void doUnvisibleTerrain()
	{
		Uint32 start = SDL_GetTicks();
	
		startUpdateFogOfWar();
	
		for(int i=0;i<sektorenX;i++)
		{
			for(int u=0;u<sektorenZ;u++)
			{
				for(int r=0;r<sektoren[i][u].triangles.size();r++)
				{
					if(!sektoren[i][u].triangles[r].triangleSeen||sektoren[i][u].triangles[r].allwaysVisible) continue;
					sektoren[i][u].triangles[r].timeSinceLastView+=3;
					//cout << sektoren[i][u].triangles[r].timeSinceLastView << endl;
					bool rand=false;
					if(!rand)
					{
						if(sektoren[i][u].triangles[r].timeSinceLastView>3)
						{
							setFogOfWarForTriangleHALV(sektoren[i][u].triangles[r].data+0,0.01);
							setFogOfWarForTriangleHALV(sektoren[i][u].triangles[r].data+1,0.01);
							setFogOfWarForTriangleHALV(sektoren[i][u].triangles[r].data+2,0.01);
						}
					}
				}
			}
		}
		
		stopUpdateFogOfWar();
		
		cout << "FOW update in" << (SDL_GetTicks()-start)/1000.0 << endl;
	}
	
	void fillFogOfWarForAllSektors(float value)
	{
		if(value!=0.0&&value!=1.0) return;
		
		bool flag=false;
		
		/*if(value==0.0) flag=false;
		else if(value==1.0) flag=true;*/
		// FOW initial state ----- seen as init
		value=0;
		
		for(int i=0;i<sektorenX;i++)
		{
			for(int u=0;u<sektorenZ;u++)
			{
				for(int r=0;r<sektoren[i][u].triangles.size();r++)
				{
					sektoren[i][u].triangles[r].timeSinceLastView=0;
					sektoren[i][u].triangles[r].triangleSeen=flag;
				}
			}
		}
	}

	
	void writeFogOfWarToFile(FILE *file)
	{
		startUpdateFogOfWar();
		int frame=0;
		fprintf(file,"%i",part[frame].planezahl);
		for(int loop=0;loop<part[frame].planezahl*3;loop+=3)
		{
			awx_fwrite(&fogOfWarPtr[loop],sizeof(float),1,file);
			awx_fwrite(&fogOfWarPtr[loop+1],sizeof(float),1,file);
			awx_fwrite(&fogOfWarPtr[loop+2],sizeof(float),1,file);
		}
		stopUpdateFogOfWar();
	}
	
	void readFogOfWarFromFile(FILE *file)
	{
		startUpdateFogOfWar();
		int z=0;
		fscanf(file,"%i",&z);
		for(int loop=0;loop<z*3;loop+=3)
		{
			awx_fread(&fogOfWarPtr[loop],sizeof(float),1,file);
			awx_fread(&fogOfWarPtr[loop+1],sizeof(float),1,file);
			awx_fread(&fogOfWarPtr[loop+2],sizeof(float),1,file);
		}
		stopUpdateFogOfWar();
	}
	
};

#endif