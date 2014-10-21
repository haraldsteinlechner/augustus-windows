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


/*class sektor
{
	public:
		float *triangles;
		int polyCount;
	sektor()
	{	
		polyCount=0;
	} 
};

sektor sektoren[SektorenX][SektorenZ];
sektor sektoren_alloc[SektorenX][SektorenZ];



int locatePointX(float x){
	return int((x/LevelAbmessungX)*SektorenX);
}
		
int locatePointZ(float z){
	return int((z/LevelAbmessungZ)*SektorenZ);
}

int locatePointX_extern(float x){
	x-=xmin_global;
	return int((x/LevelAbmessungX)*SektorenX);
}
		
int locatePointZ_extern(float z){
	z-=zmin_global;
	return int((z/LevelAbmessungZ)*SektorenZ);
}
		
void genMinMax(float x1 , float x2 , float x3 , float z1 , float z2 , float z3){
	xMax = locatePointX(x1);
	xMin = locatePointX(x1);
	zMax = locatePointZ(z1);
	zMin = locatePointZ(z1);
			
	if(locatePointX(x2) > xMax) xMax = locatePointX(x2);
	else if(locatePointX(x2) < xMin) xMin = locatePointX(x2);
			
	if(locatePointZ(z2) > zMax) zMax = locatePointZ(z2);
	else if(locatePointZ(z2) < zMin) zMin = locatePointZ(z2);
			
	if(locatePointX(x3) > xMax) xMax = locatePointX(x3);
	else if(locatePointX(x3) < xMin) xMin = locatePointX(x3);
			
	if(locatePointZ(z3) > zMax) zMax = locatePointZ(z3);
	else if(locatePointZ(z3) < zMin) zMin = locatePointZ(z3);
}
		
void locateTriangle(float x1 , float y1 , float z1 , float x2 , float y2 , float z2 , float x3 , float y3 , float z3, float xmin, float zmin){
	
	genMinMax(x1,x2,x3,z1,z2,z3);
	
	
	int polyCount;
	
	for(int loop=xMin;loop<=xMax;loop++)
	{
		for(int loop2=zMin;loop2<=zMax;loop2++)
		{
			polyCount = sektoren[loop][loop2].polyCount;
			
			sektoren[loop][loop2].triangles[polyCount] = x1+xmin;
			sektoren[loop][loop2].triangles[polyCount+1] = y1;
			sektoren[loop][loop2].triangles[polyCount+2] = z1+zmin;
			
			sektoren[loop][loop2].triangles[polyCount+3] = x2+xmin;
			sektoren[loop][loop2].triangles[polyCount+4] = y2;
			sektoren[loop][loop2].triangles[polyCount+5] = z2+zmin;
			
			sektoren[loop][loop2].triangles[polyCount+6] = x3+xmin;
			sektoren[loop][loop2].triangles[polyCount+7] = y3;
			sektoren[loop][loop2].triangles[polyCount+8] = z3+zmin;
			
			sektoren[loop][loop2].polyCount+=9;
		}
	}
					
}

void locateTriangle_alloc(float x1 , float y1 , float z1 , float x2 , float y2 , float z2 , float x3 , float y3 , float z3){
	
	genMinMax(x1,x2,x3,z1,z2,z3);
	
	
	int polyCount;
	
	for(int loop=xMin;loop<=xMax;loop++)
	{
		for(int loop2=zMin;loop2<=zMax;loop2++)
		{
			polyCount = sektoren_alloc[loop][loop2].polyCount;			
			sektoren_alloc[loop][loop2].polyCount+=9;
		}
	}
}

void triangle_alloc()
{
		// Allocate
	for(int loop=0;loop<SektorenX;loop++)
	{
		for(int loop2=0;loop2<SektorenZ;loop2++)
		{
			sektoren[loop][loop2].triangles = new float[sektoren_alloc[loop][loop2].polyCount];
			if(sektoren[loop][loop2].triangles==NULL)
			{
				cout << "Zuwenig RAM." << endl;
				exit(0);
			}
		}
	}
}

void display_sektor(int x, int z)
{
	glPushMatrix();
	//glTranslatef(xmin_global,0,zmin_global);
	for(int loop=0; loop<sektoren[x][z].polyCount; loop+=9)
	{
		glBegin(GL_LINE_STRIP);
		
		glTexCoord2f(0.0,0.0);
		glVertex3f(sektoren[x][z].triangles[loop],sektoren[x][z].triangles[loop+1],sektoren[x][z].triangles[loop+2]);
		glTexCoord2f(1.0,1.0);
		glVertex3f(sektoren[x][z].triangles[loop+3],sektoren[x][z].triangles[loop+4],sektoren[x][z].triangles[loop+5]);
		glTexCoord2f(0.0,1.0);
		glVertex3f(sektoren[x][z].triangles[loop+6],sektoren[x][z].triangles[loop+7],sektoren[x][z].triangles[loop+8]);
	
		glVertex3f(sektoren[x][z].triangles[loop],sektoren[x][z].triangles[loop+1],sektoren[x][z].triangles[loop+2]);	
		
		glEnd();
	}
	glPopMatrix();
}*/

bool awx_triangle_hitTest(awx_triangle trian , awx_vector3f point , float &newY)
{
	newY = AWXFalse;
	
	float ux = trian.p3.x-trian.p1.x;
	float uy = trian.p3.z-trian.p1.z;
	float uz = trian.p3.y-trian.p1.y;
	
	float vx = trian.p2.x-trian.p1.x;
	float vy = trian.p2.z-trian.p1.z;
	float vz = trian.p2.y-trian.p1.y;
	
	float lambda=0;
	float my=0;

	float Dlambda;
	float Dmy;
	float D;
		
		Dlambda = (point.x-trian.p1.x)*vy - (point.z-trian.p1.z)*vx;
		Dmy =  (point.z-trian.p1.z)*ux - (point.x-trian.p1.x)*uy;
		D = ux*vy - uy*vx;
		
		if(D == 0){
			cout << "Kritischer Kollisionsfehler!  D = 0" << endl;
			cout << "Lambda = " << lambda << endl;
			cout << "My = " << my << endl << endl;
			cout << "u = (" << ux << "," << uy << "," << uz << ")" << endl;
			cout << "v = (" << vx << "," << vy << "," << vz << ")" << endl;
		}
		
		lambda = (Dlambda/D);
		my = (Dmy/D);

		float zneu = ( trian.p1.y + lambda*uz + my*vz);
	
		if((lambda+my) <= 1 && lambda >= 0 && my >= 0)
		{
			newY = zneu;
			if(point.y <= zneu+0.5/* && z >= zneu - 0.01*/ )return true;
			else return false;
		}
		else return false;
}

bool awx_triangle_hitTest(float p1x, float p1y, float p1z, 
						  float p2x, float p2y, float p2z, 
						  float p3x, float p3y, float p3z, 
						  float x, float y, float z)
{
	float ux = p3x-p1x;
	float uy = p3y-p1y;
	float uz = p3z-p1z;
	
	float vx = p2x-p1x;
	float vy = p2y-p1y;
	float vz = p2z-p1z;
	
	float lambda=0;
	float my=0;

	float Dlambda;
	float Dmy;
	float D;
	float zneu;
		
		Dlambda = (x-p1x)*vy - (y-p1y)*vx;
		Dmy =  (y-p1y)*ux - (x-p1x)*uy;
		D = ux*vy - uy*vx;
		
		if(D == 0){
			cout << "Kritischer Kollisionsfehler!  D = 0" << endl;
			cout << "Lambda = " << lambda << endl;
			cout << "My = " << my << endl << endl;
			cout << "u = (" << ux << "," << uy << "," << uz << ")" << endl;
			cout << "v = (" << vx << "," << vy << "," << vz << ")" << endl;
		}
		
		lambda = (Dlambda/D);
		my = (Dmy/D);

		zneu = ( p1z + lambda*uz + my*vz);
	
		if( z <= zneu+0.5/* && z >= zneu - 0.01*/ && (lambda+my) <= 1 && lambda >= 0 && my >= 0) return true;
	
		else return false;
}

float awx_triangle_hitTest_2D(float p1x, float p1y, float p1z, 
						  float p2x, float p2y, float p2z, 
						  float p3x, float p3y, float p3z, 
						  float x, float y, float z)
{
	float ux = p3x-p1x;
	float uy = p3y-p1y;
	float uz = p3z-p1z;
	
	float vx = p2x-p1x;
	float vy = p2y-p1y;
	float vz = p2z-p1z;
	
	float lambda=0;
	float my=0;

	float Dlambda;
	float Dmy;
	float D;
	float zneu;
		
		Dlambda = (x-p1x)*vy - (y-p1y)*vx;
		Dmy =  (y-p1y)*ux - (x-p1x)*uy;
		D = ux*vy - uy*vx;
		
		if(D == 0){
			cout << "Kritischer Kollisionsfehler!  D = 0" << endl;
			cout << "Lambda = " << lambda << endl;
			cout << "My = " << my << endl << endl;
			cout << "u = (" << ux << "," << uy << "," << uz << ")" << endl;
			cout << "v = (" << vx << "," << vy << "," << vz << ")" << endl;
		}
		
		lambda = (Dlambda/D);
		my = (Dmy/D);

		zneu = ( p1z + lambda*uz + my*vz);
	
		if((lambda+my) <= 1 && lambda >= 0 && my >= 0) return zneu;
	
		else return AWXFalse;
}


/*#define rasterX 550 
#define rasterZ 550

class rastertyp{
	public:
		char ID;
		
		rastertyp(){
			ID = 0;
		}
};

rastertyp raster[rasterX][rasterZ];

void locate_in_raster(awx_vector3f point , int &x , int &z){
	x = int(((point.x - xmin_global)/LevelAbmessungX)*rasterX);
	z = int(((point.z - zmin_global)/LevelAbmessungZ)*rasterZ);
	
	if(x < 0 || z < 0){
		x = 0;
		z = 0;
	}
}

int rasterID(awx_vector3f point , int &x , int &z){
	locate_in_raster(point,x,z);
	if(x >= 0 && z >= 0){
		return int(raster[x][z].ID);
	}
	else return -1;
}*/
