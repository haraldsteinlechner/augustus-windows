#ifdef terraincpp

#include "awx_terrain.h"


awx_terrain::awx_terrain()
{
    apx::apx();
}

int awx_terrain::loadTerrain(const char* filename)
{
    // Datei öffnen
    FILE* input = fopen(filename,"rb");
    if(input==NULL)
    {
        return -1;
    }
    
    workingdirectory = filename;
    workingdirectory = workingdirectory.stringByDeletingLastPathComponent();
    
    awx_fread(&version,sizeof(version),1,input);

    int anzahl_ids;
    awx_fread(&anzahl_ids,sizeof(anzahl_ids),1,input);
    anzahl_usedIDs = anzahl_ids;
    usedIDs = new int[anzahl_ids];
    for(int i=0;i<anzahl_ids;i++)
    { 
        awx_fread(&usedIDs[i],sizeof(usedIDs[i]),1,input);
    }
    
    anzahl_frames=1;
    part = new awx_part[anzahl_frames];
    IDs = new int[anzahl_frames];
    
    int frame=0;
    part[frame].anzahl_frames = anzahl_frames;

    awx_fread(&u,sizeof(u),1,input);
    awx_fread(&v,sizeof(v),1,input);
    
    
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
    part[frame].texturIDs = new int[part[frame].planezahl*3];
    part[frame].texturWinkel = new int[part[frame].planezahl*3];
    
    xmax = 0;
    xmin = 0;
    zmin = 0;
    zmax = 0;


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

        awx_fread(&part[frame].texturIDs[loop],sizeof(part[frame].texturIDs[loop]),1,input);
        awx_fread(&part[frame].texturWinkel[loop],sizeof(part[frame].texturWinkel[loop]),1,input);
    }
    
    fclose(input);
    
    // StandardTextur setzen
    standard_tex_ID=24+9;
    
    xmax = this->xmax;
    zmax = this->zmax;
    return 0;
}

void awx_terrain::render()
{
    glPushMatrix();
        glCallList(IDs[0]);
    glPopMatrix();

}

void awx_terrain::render_special()
{			
    int frame=0;
    
    for(loop=0;loop<part[frame].planezahl*3;loop+=3)
    {
        if(part[frame].texturWinkel[int(loop)]!=0)
        {
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glRotatef(part[frame].texturWinkel[int(loop)],0,0,1);
            glMatrixMode(GL_MODELVIEW);
        }

        if(loop<3||part[frame].texturIDs[int(loop)]!=part[frame].texturIDs[int(loop-3)])
        {	
            glBindTexture(GL_TEXTURE_2D,textur_liste[part[frame].texturIDs[int(loop)]+startTileID]);
        }
        
        glBegin(GL_TRIANGLES);
            glNormal3f(part[frame].normal[part[frame].normalsequenz[loop]*3],part[frame].normal[part[frame].normalsequenz[loop]*3+1],part[frame].normal[part[frame].normalsequenz[loop]*3+2]);
            glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop]*2+1]*v);
            glVertex3f(part[frame].vertex[part[frame].plane[loop]*3],part[frame].vertex[part[frame].plane[loop]*3+1],part[frame].vertex[part[frame].plane[loop]*3+2]);
        
            glNormal3f(part[frame].normal[part[frame].normalsequenz[loop+1]*3],part[frame].normal[part[frame].normalsequenz[loop+1]*3+1],part[frame].normal[part[frame].normalsequenz[loop+1]*3+2]);
            glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop+1]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+1]*2+1]*v);
            glVertex3f(part[frame].vertex[part[frame].plane[loop+1]*3],part[frame].vertex[part[frame].plane[loop+1]*3+1],part[frame].vertex[part[frame].plane[loop+1]*3+2]);
        
            glNormal3f(part[frame].normal[part[frame].normalsequenz[loop+2]*3],part[frame].normal[part[frame].normalsequenz[loop+2]*3+1],part[frame].normal[part[frame].normalsequenz[loop+2]*3+2]);
            glTexCoord2f(part[frame].tex_koord[part[frame].textursequenz[loop+2]*2]*u,part[frame].tex_koord[part[frame].textursequenz[loop+2]*2+1]*v);
            glVertex3f(part[frame].vertex[part[frame].plane[loop+2]*3],part[frame].vertex[part[frame].plane[loop+2]*3+1],part[frame].vertex[part[frame].plane[loop+2]*3+2]);
        glEnd();
        
        if(part[frame].texturWinkel[loop]!=0)
        {
            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
        }
    
    }	
}

void awx_terrain::generateLists()
{
    int frame=0;
    for(int i=0;i<anzahl_usedIDs;i++)
    {
        glNewList(currentID,GL_COMPILE);
        int loop=0;
        glBindTexture(GL_TEXTURE_2D,textur_liste[part[frame].texturIDs[int(loop)]+startTileID]);
        
        glBegin(GL_TRIANGLES);
        for(loop=0;loop<part[frame].planezahl*3;loop+=3)
        {
                if(part[frame].texturIDs[int(loop-3)]==i)
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
        }
        glEnd();
        
        glEndList();
        
        currentID++;
    }
    
}

void awx_terrain::sektorize()
{
    int frame=0;
    float punkt1X,punkt1Y,punkt1Z,punkt2X,punkt2Y,punkt2Z,punkt3X,punkt3Y,punkt3Z;

    for(loop=0;loop<part[frame].planezahl*3;loop+=3)
    {
        punkt1X = part[frame].vertex[part[frame].plane[loop]*3];
        punkt1Y = part[frame].vertex[part[frame].plane[loop]*3+1];
        punkt1Z = part[frame].vertex[part[frame].plane[loop]*3+2];
        
        punkt2X = part[frame].vertex[part[frame].plane[loop+1]*3];
        punkt2Y = part[frame].vertex[part[frame].plane[loop+1]*3+1];
        punkt2Z = part[frame].vertex[part[frame].plane[loop+1]*3+2];
        
        punkt3X = part[frame].vertex[part[frame].plane[loop+2]*3];
        punkt3Y = part[frame].vertex[part[frame].plane[loop+2]*3+1];
        punkt3Z = part[frame].vertex[part[frame].plane[loop+2]*3+2];
        
        
        locateTriangle_alloc(punkt1X-xmin, punkt1Y, punkt1Z-zmin, punkt2X-xmin, punkt2Y, punkt2Z-zmin, punkt3X-xmin, punkt3Y, punkt3Z-zmin);
    }
        
    triangle_alloc();
        
    for(loop=0;loop<part[frame].planezahl*3;loop+=3)
    {
        punkt1X = part[frame].vertex[part[frame].plane[loop]*3];
        punkt1Y = part[frame].vertex[part[frame].plane[loop]*3+1];
        punkt1Z = part[frame].vertex[part[frame].plane[loop]*3+2];
        
        punkt2X = part[frame].vertex[part[frame].plane[loop+1]*3];
        punkt2Y = part[frame].vertex[part[frame].plane[loop+1]*3+1];
        punkt2Z = part[frame].vertex[part[frame].plane[loop+1]*3+2];
        
        punkt3X = part[frame].vertex[part[frame].plane[loop+2]*3];
        punkt3Y = part[frame].vertex[part[frame].plane[loop+2]*3+1];
        punkt3Z = part[frame].vertex[part[frame].plane[loop+2]*3+2];
        
        //cout << "Original " << punkt1X << "/" << punkt1Y << "/" << punkt1Z << endl;
        
        locateTriangle(punkt1X-xmin, punkt1Y, punkt1Z-zmin, punkt2X-xmin, punkt2Y, punkt2Z-zmin, punkt3X-xmin, punkt3Y, punkt3Z-zmin, xmin, zmin);
    }
    
    xmin_global = xmin;
    zmin_global = zmin;
}

void awx_terrain::fillVRAM()
{
    int startID = currentID;
    startTileID = startID;

    // Tiles laden
    cout << "Tiles laden.." << endl;
    for(int i=0;i<anzahl_usedIDs;i++)
    {
        char temp[5];
        sprintf(temp,"%i",usedIDs[i]);
        awx_string path = workingdirectory + "/tile" + temp;
        load_tga(path.cString(),i+currentID, true, true);
    }
    cout << "Tiles geladen." << endl;
    
    int frame=0;
    
    //rotateTexture(1,90,512,true,true);

    for(int loop=0;loop<part[frame].planezahl*3;loop+=3)
    {
        int i;
        for(i=0;i<anzahl_usedIDs;i++)
        {
            if(usedIDs[i] == part[frame].texturIDs[int(loop)]) break;
        }
        part[frame].texturIDs[int(loop)] = i;
    }	
    
    currentID+=anzahl_usedIDs;
    
    IDs[0] = currentID;
    
    glNewList(IDs[0],GL_COMPILE);
    render_special();
//	render_frame(0);
    glEndList();
    
    currentID++;
    
    
    //generateLists();
    
    sektorize();
}

void awx_terrain::regenerateList()
{
    generateLists();
}

#endif
