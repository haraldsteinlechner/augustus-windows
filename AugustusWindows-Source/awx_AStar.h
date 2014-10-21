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


// A*WX 3D V2 - based on http://www.gamedev.net/reference/articles/article2003.asp


#include "awx_vector.h"


#define MAXaStarIterationDepth 800

class aStarNode;

class aStarNode
{
	public:
	
		int x,z;
		int f,g,h;
		int preG;
		
		int parentX,parentZ,parentG;
		float yDistance;
		
		bool error;
		
		// G = Bewegungskosten bis zu diesem Node bisher
		// H = Geschätzte Entfernung zum ziel
		// F = G + H
		
		aStarNode()
		{
			x=0; z=0;
			parentX=-1;parentZ=-1;
			parentG=0;
			f=-1; g=-1; h=-1;
			preG=-1;
			yDistance=0;
			error=false;
		}
		
		void setWeights(int g, int h)
		{
			this->f=f+h;
			this->g=g;
			this->h=h;
		}
		
		void setWeights(int g, int h, int f)
		{
			this->f=f;
			this->g=g;
			this->h=h;
		}
		
		void calculateWeights(aStarNode *parentNode, aStarNode *targetNode, float yDistance)
		{
			if(parentNode==NULL||targetNode==NULL) 
			{
				cout << "DEBUG AI: weight calculation not possible" << endl;
				return;
			} 
			this->g=parentNode->g+preG;
			this->h=(abs(x-targetNode->x))*10+(abs(z-targetNode->z))*10;
			this->f=this->g+this->h+yDistance*0.33;
			this->yDistance=yDistance;
			
			//cout << "CALC WEIGHTS: g=" << this->g << "  h=" << h << "   " << f << "  preG=" << preG << endl;
		}
		
		bool operator==(const aStarNode &v)
		{
			return (v.x==x&&v.z==z);
		}
		
		friend bool operator>(const aStarNode &u, const aStarNode &v);
		
		friend bool operator<(const aStarNode &u, const aStarNode &v);
		
		aStarNode &operator=(const aStarNode &obj)
		{
			f=obj.f;
			g=obj.g;
			h=obj.h;
			x=obj.x;
			z=obj.z;
			parentX = obj.parentX;
			parentZ = obj.parentZ;
			parentG = obj.parentG;
			preG=obj.preG;
			yDistance=obj.yDistance;
			
			return *this;
		}
		
		void plotWithParentInfo()
		{
			cout << x << "/" << z << "  parent:" << parentX << "/" << parentZ << "   f=" << f << " g=" << g << "  h=" << h << "  YDistance=" << yDistance << endl;
		}
};

ostream &operator << (ostream & out, aStarNode a){
    cout << endl << "Node:" << endl;
	cout << a.x << "/" << a.z << /*"  parent:" << a.parentNode->x << "/" << a.parentNode->z << */"   f=" << a.f << " g=" << a.g << "  h=" << a.h << endl;
    return out;
}


bool operator>(const aStarNode& u, const aStarNode &v)
{
		return (u.f>v.f);
}
		
bool operator<(const aStarNode& u, const aStarNode &v)
{
		return (u.f<v.f);
}


unsigned short *aStarGrid = NULL;
float *aStarHeightGrid = NULL;

float aStarGridSize = 2.0;
int maxNodesX=-1,maxNodesZ=-1;

void projectLevelBegehbarkeiten();
void prepareAStarGrid()
{
	if(currentLevel->terrain.xmax<=0||currentLevel->terrain.xmax>10000) 
	{
		cout << "AI DEBUG: Levelmasse out of range" << endl;
		return;
	}
	if(currentLevel->terrain.zmax<=0||currentLevel->terrain.zmax>10000) 
	{
		cout << "AI DEBUG: Levelmasse out of range" << endl;
		return;
	}
	
	maxNodesX=ceil((currentLevel->terrain.xmax*2)/aStarGridSize);
	maxNodesZ=ceil((currentLevel->terrain.zmax*2)/aStarGridSize);
	
	aStarGrid = new unsigned short[maxNodesX*maxNodesZ];
	aStarHeightGrid = new float[maxNodesX*maxNodesZ];
	
	
	for(int i=0;i<maxNodesX*maxNodesZ;i++)
	{
		aStarGrid[i]=0;
		aStarHeightGrid[i]=0;
	}
	
	cout << "AI DEBUG: Gridsize: " << maxNodesX << "/" << maxNodesZ << endl;
	
	projectLevelBegehbarkeiten();
	
	cout << "Levelbegehbarkeiten projected." << endl;
	
}

typedef struct
{
	int x,z;
} int2;

void updateAStarGridAtObject(awx_building &object, awx_vector3f reference)
{
	Uint32 start = SDL_GetTicks();
	awx_vector3f position = awx_vector3f(object.x,object.y,object.z);
	
	object.x=reference.x;
	object.y=reference.y;
	object.z=reference.z;
	
	awx_terrain *terrain = &currentLevel->terrain;

	awx_vector3f ec1 = awx_vector3f(object.data[2].xmin,0,object.data[2].zmin);
	awx_vector3f ec2 = awx_vector3f(object.data[2].xmax,0,object.data[2].zmax);
	
	float diagonal = fabs((ec2-ec1).getLength());
	vector<int2> points;
	
	awx_vector3f currentPoint;
	
	float distance=-1;

	reference.y=0;
	//cout << "DEBUG" << object.xmin << "    " << object.zmin << "   " << object.xmax << "   " << object.zmax << endl;

	for(int i=0;i<maxNodesX;i++)
	{			
		for(int u=0;u<maxNodesZ;u++)
		{
			currentPoint = awx_vector3f(aStarGridSize*i-terrain->xmax,0,aStarGridSize*u-terrain->zmax);
			
			distance = fabs((currentPoint-reference).getLength());
			//cout << distance << endl;
			
			if(distance<=diagonal*4)
			{
				int2 temp;
				temp.x=i;
				temp.z=u;
				points.push_back(temp);
			}
		}
	}
	
	for(int i=0;i<points.size();i++)
	{
		currentPoint = awx_vector3f(aStarGridSize*points[i].x-terrain->xmax,0,aStarGridSize*points[i].z-terrain->zmax);
		if(object.boundingCollision(currentPoint))
		{
			aStarGrid[points[i].x*maxNodesZ+points[i].z]=3;
		}
	}
	
	points.clear();
	
	object.x=position.x;
	object.y=position.y;
	object.z=position.z;
	
	cout << "GridUpdate in " << (SDL_GetTicks()-start)/1000.0 << endl;
}

void updateAStarGridAtLevelPart(levelpart &object)
{
	awx_vector3f reference = awx_vector3f(object.x,object.y,object.z);
	
	awx_terrain *terrain = &currentLevel->terrain;

	awx_vector3f ec1 = awx_vector3f(object.xmin,0,object.zmin);
	awx_vector3f ec2 = awx_vector3f(object.xmax,0,object.zmax);
	
	float diagonal = fabs((ec2-ec1).getLength());
	vector<int2> points;
	
	awx_vector3f currentPoint;
	
	float distance=-1;

	for(int i=0;i<maxNodesX;i++)
	{			
		for(int u=0;u<maxNodesZ;u++)
		{
			currentPoint = awx_vector3f(aStarGridSize*i-terrain->xmax,0,aStarGridSize*u-terrain->zmax);
			
			distance = fabs((currentPoint-reference).getLength());
			
			if(distance<=diagonal*4)
			{
				int2 temp;
				temp.x=i;
				temp.z=u;
				points.push_back(temp);
			}
		}
	}
	
	for(int i=0;i<points.size();i++)
	{
		currentPoint = awx_vector3f(aStarGridSize*points[i].x-terrain->xmax,0,aStarGridSize*points[i].z-terrain->zmax);
		if(object.boundingCollision(currentPoint))
		{
			aStarGrid[points[i].x*maxNodesZ+points[i].z]=3;
		}
	}
	
	points.clear();
}


void projectLevelBegehbarkeiten()
{
	Uint32 start = SDL_GetTicks();
	
	awx_terrain *terrain = &currentLevel->terrain;
	
	int hitTriangle;
	awx_vector3f position;
	int currentSektorX,currentSektorZ;
	
	
	bool ergebnisflag;
	for(int i=0;i<maxNodesX;i++)
	{
			
		for(int u=0;u<maxNodesZ;u++)
		{
			position = awx_vector3f(aStarGridSize*i-terrain->xmax,0,aStarGridSize*u-terrain->zmax);
			
			hitTriangle = terrain->hit(position , position.y , ergebnisflag, currentSektorX, currentSektorZ);
			
			if(hitTriangle<0)continue;
			
			aStarGrid[i*maxNodesZ+u] = (unsigned short) terrain->begehbarkeit[int(terrain->sektoren[currentSektorX][currentSektorZ].triangles[hitTriangle].data/3.0)];
			aStarHeightGrid[i*maxNodesZ+u] = (float) position.y; 
		}
	}
	
	delete[] terrain->begehbarkeit;
	
	glDeleteBuffersARB(1,&terrain->vboBegehbarkeit);
	
	cout << "Deallocated begehbarkeit." << endl;
	
	
	// Projecting Trees
	
	Uint32 start2 = SDL_GetTicks();
	float size;
	for(int u=0;u<currentLevel->number_of_objects;u++)
	{
		levelpart *part = &currentLevel->data[u];
		size=part->xmax+part->zmax;
		
		if(part->shadowflag==1&&size>3)
		updateAStarGridAtLevelPart(*part);
	}
	
	
	cout << endl << "GridProjection in " << (SDL_GetTicks()-start)/1000.0 << " Projecting levelParts consumed: " << (SDL_GetTicks()-start2)/1000.0 << endl << endl;
}

void displayAStarGrid()
{	
	if(maxNodesX==-1||maxNodesZ==-1) return;
	
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0,1.0,1.0);
	glUseProgramObjectARB(NULL);
	
	glPushMatrix();
	
	glTranslatef(-currentLevel->terrain.xmax,0,-currentLevel->terrain.zmax);
	
	unsigned short *val;
	
	glBegin(GL_POINTS);
	for(int i=0;i<maxNodesX;i++)
	{
			
		for(int u=0;u<maxNodesZ;u++)
		{
			val = &aStarGrid[i*maxNodesZ+u];
			
			if(*val==0)glColor3f(0.0,1.0,0.0);
			else if(*val==1)glColor3f(0.0,0.0,1.0); 
			else if(*val==2)glColor3f(0.0,0.5,0.5);
			else if(*val==3)glColor3f(1.0,0.0,0.0);
			else glColor3f(0.0,0.0,0.0);
			
			glVertex3f(aStarGridSize*i,aStarHeightGrid[i*maxNodesZ+u],aStarGridSize*u);
			
		}
	}
	glEnd();
	
	glPopAttrib();
	
	glPopMatrix();
}

void debugList(string listname, vector<aStarNode> &list)
{	
	cout << endl << listname << ": " << endl;
	cout << "============================" << endl;
	for(int i=0;i<list.size();i++)
	{
		cout << list[i] << endl;
	}
	cout << "============================" << endl;
}

void rollOffPath(vector<aStarNode> &closedList, awx_vector3f &startPosition, awx_vector3f &endPosition, vector<awx_vector3f> &resultVectorReference)
{
	#ifdef aStarDebug
	cout << "RollOffPath" << endl << endl;
	#endif
	
	vector<aStarNode>::iterator findIterator;
	vector<awx_vector3f> resultVector;
	aStarNode lastNode = closedList[closedList.size()-1];
	
	#ifdef aStarDebug
	cout << "lastNode:" << lastNode << endl;
	#endif
	
	closedList.erase(closedList.end()-1);
	
	aStarNode *currentNode = &lastNode;
	
	aStarNode temp;
	
	int u=0;
	
	resultVector.push_back(awx_vector3f(currentLevel->terrain.xmin,0,currentLevel->terrain.zmin) + endPosition);
	
	while((temp.x=currentNode->parentX)!=-1&&(temp.z=currentNode->parentZ)!=-1)
	{
		findIterator = find(closedList.begin(),closedList.end(),temp);
		u = distance(closedList.begin(),findIterator);
		currentNode = &closedList[u];
		resultVector.push_back(awx_vector3f(currentLevel->terrain.xmin+currentNode->x*aStarGridSize,0,currentLevel->terrain.zmin+currentNode->z*aStarGridSize));
		//closedList.erase(findIterator);
	}
	
	resultVector.erase(resultVector.end()-1);
	resultVector.push_back(awx_vector3f(currentLevel->terrain.xmin,0,currentLevel->terrain.zmin) + startPosition);
	
	#ifdef aStarDebug
	cout << "THEResult(beforeMirror)=" << endl;
	#endif
	
	#ifdef aStarPathDebug
	#ifdef GCCDebugCompile
	#warning loeschen
	#endif
	drawPath.clear();
	#endif
	
	for(int i=resultVector.size()-1;i>=0;i--)
	{
		#ifdef aStarDebug
		cout << resultVector[i] << endl;
		#endif
		resultVectorReference.push_back(resultVector[i]);
		#ifdef aStarPathDebug
		drawPath.push_back(resultVector[i]);
		#endif
	}

	#ifdef aStarDebug
	cout << "begin=" << startPosition << "   end=" << endPosition << endl;
	#endif
}

#ifdef _WIN32
float round(float number)
{
	float numberInt = (float) int(number); 
	if(number-numberInt<0.5) return (int)number;
	return (int)number+1;
}
#endif

struct sorterForNodes
{
	bool operator()(const aStarNode &sepp,const aStarNode &hugo)
	{
		return sepp.f<hugo.f;
	}
};

bool generatePathAStar(awx_vector3f start, awx_vector3f target, vector<awx_vector3f> &resultVectorReference)
{
	//Uint32 startTime = SDL_GetTicks();
	
	vector<awx_vector3f> resultVector;
	
	
	vector<aStarNode> openList;
	vector<aStarNode> closedList;
	vector<aStarNode>::iterator it;
	
	openList.clear();
	closedList.clear();
	
	
	// Punkte in GridKoordinatensystem bringen
	start.z = start.z - currentLevel->terrain.zmin;
	start.x = start.x - currentLevel->terrain.xmin;
	target.z = target.z - currentLevel->terrain.zmin;
	target.x = target.x - currentLevel->terrain.xmin;

	
	// Start und Zielnode bestimmen
	aStarNode startNode;
	startNode.x = int(round((float)start.x / (float)aStarGridSize));
	startNode.z = int(round((float)start.z / (float)aStarGridSize));
	aStarNode endNode;
	endNode.x = int(round((float)target.x / (float)aStarGridSize));
	endNode.z = int(round((float)target.z / (float)aStarGridSize));
	
	
	// Startnode zur OpenList hinzufügen
	startNode.setWeights(0,0,0);
	openList.push_back(startNode);
	
	// OpenList is dem FWert absteigend geordnet.
	
	aStarNode currentNode;
	aStarNode tempNode;
	
	aStarNode adjacencyNodes[8];
	int adjacencyCounter=0;
	int iterationDepth=0;
	unsigned short *data;
	
	#ifdef aStarDebug 
	cout << endl << endl << "A* working..." << endl << endl;
	#endif
	
	
	float heightOfCurrentNode=0; 
	float adjHeight=0;
	float heightDistance=0;
	
	while(1)
	{
		iterationDepth++;
		
		// Niedrigstes F node fetch
		
		//debugList("unsorted",openList);
		
		
		// Keinen passenden Knoten gefunden
		if(openList.size()<=0&&iterationDepth>1)
		{
			resultVectorReference.clear();
			resultVectorReference.push_back(target);
			cout << "Empty openList" << endl;
			goto aStarERROR;
		}
		
		sort(openList.begin(),openList.end());
		currentNode = openList[0];
		
		
		// Range Check
		if(currentNode.error) goto aStarERROR;
		
		#ifdef aStarDebug 
		cout << "Derzeitiges" << currentNode << endl;
		aStarDebug debugList("closedList",closedList);
		aStarDebug debugList("openList",openList);
		#endif
		
		// In Closed List verschieben
		closedList.push_back(currentNode);
		openList.erase(openList.begin());
		
		
		// Höhe auslesen
		int acess = currentNode.x*maxNodesZ+currentNode.z;
		if(acess<0||acess>=maxNodesX*maxNodesZ) goto aStarERROR;
		heightOfCurrentNode = aStarHeightGrid[acess];
		
		// Umliegende 8 nodes betrachten
		adjacencyCounter=0;
		
		tempNode.error = true;
		
		if(currentNode.x>=1&&currentNode.z>=1)
		{
			tempNode.x = currentNode.x-1;
			tempNode.z = currentNode.z-1;
			tempNode.error = false;
			
			tempNode.preG = 14;
			adjacencyNodes[adjacencyCounter] = tempNode;
			adjacencyCounter++;
		}
		if(currentNode.z>=1)
		{
			tempNode.x = currentNode.x;
			tempNode.z = currentNode.z-1;
			tempNode.error = false;
			
			tempNode.preG = 10;
			adjacencyNodes[adjacencyCounter] = tempNode;
			adjacencyCounter++;	
		}
		if(currentNode.x>=1)
		{
			tempNode.x = currentNode.x-1;
			tempNode.z = currentNode.z;
			tempNode.error = false;
			
			tempNode.preG = 10;
			adjacencyNodes[adjacencyCounter] = tempNode;
			adjacencyCounter++;	
		}
		if(currentNode.x>=1&&currentNode.z<maxNodesZ)
		{
			tempNode.x = currentNode.x-1;
			tempNode.z = currentNode.z+1;
			tempNode.error = false;
			
			tempNode.preG = 14;
			adjacencyNodes[adjacencyCounter] = tempNode;
			adjacencyCounter++;	
		}
		if(currentNode.z<maxNodesZ)
		{
			tempNode.x = currentNode.x;
			tempNode.z = currentNode.z+1;
			tempNode.error = false;
			
			tempNode.preG = 10;
			adjacencyNodes[adjacencyCounter] = tempNode;
			adjacencyCounter++;	
		}
		if(currentNode.x<maxNodesX&&currentNode.z<maxNodesZ)
		{
			tempNode.x = currentNode.x+1;
			tempNode.z = currentNode.z+1;
			tempNode.error = false;
			
			tempNode.preG = 14;
			adjacencyNodes[adjacencyCounter] = tempNode;
			adjacencyCounter++;	
		}
		if(currentNode.x<maxNodesX)
		{
			tempNode.x = currentNode.x+1;
			tempNode.z = currentNode.z;
			tempNode.error = false;
			
			tempNode.preG = 10;
			adjacencyNodes[adjacencyCounter] = tempNode;
			adjacencyCounter++;	
		}
		if(currentNode.x<maxNodesX&&currentNode.z>=1)
		{
			tempNode.x = currentNode.x+1;
			tempNode.z = currentNode.z-1;
			tempNode.error = false;
			
			tempNode.preG = 14;
			adjacencyNodes[adjacencyCounter] = tempNode;
			adjacencyCounter++;	
		}
		
		// Alle validen ADJ nodes durchwandern
		for(int i=0;i<adjacencyCounter;i++)
		{
			#ifdef aStarDebug 
			cout << "ADJ wird bearbeitet(" << i << ")=" << adjacencyNodes[i] << endl;	
			#endif	
			// Wenn illegales node oder schon in closed list -> ignorieren
			if(find(closedList.begin(),closedList.end(),adjacencyNodes[i])!=closedList.end())
			{
				#ifdef aStarDebug 
				cout << "Schon on closed list" << adjacencyNodes[i];
				debugList("closedList",closedList);
				#endif
				continue;
			}
			
			// Aus Griddata lesen und schauen ob begehbar ist
			data = &aStarGrid[adjacencyNodes[i].x*maxNodesZ+adjacencyNodes[i].z];
			if(*data!=0)
			{
				#ifdef aStarDebug 
				cout << "Schon on closed list" << endl;
				debugList("closedList",closedList);
				#endif
				continue;
			}
			
			// NOCH nicht in closed list oder unbegehbar:
			
			
			// Höhe dises nodes auslesen
			adjHeight = aStarHeightGrid[adjacencyNodes[i].x*maxNodesZ+adjacencyNodes[i].z];
			heightDistance = heightOfCurrentNode - adjHeight;
			if(heightDistance<0) heightDistance = 0;
			
			// Schauen ob es in der openList ist
			if((it=find(openList.begin(),openList.end(),adjacencyNodes[i]))==openList.end())
			{
				// OpenList hinzufügen
				adjacencyNodes[i].parentX = currentNode.x;
				adjacencyNodes[i].parentZ = currentNode.z;
				adjacencyNodes[i].parentG = currentNode.g;
				adjacencyNodes[i].calculateWeights(&currentNode,&endNode,heightDistance);
				openList.push_back(adjacencyNodes[i]);
				if(adjacencyNodes[i].x==endNode.x&&adjacencyNodes[i].z==endNode.z)
				{
					closedList.push_back(adjacencyNodes[i]);
					goto ENDFound;
				}
			}
			else
			{
				int u = distance(openList.begin(),it);
				
				adjacencyNodes[i].calculateWeights(&currentNode,&endNode,heightDistance);
			
				// Wenn derzeitiger Node zu 
				if(openList[u].g>adjacencyNodes[i].g)
				{
					openList[u].parentX = currentNode.x;
					openList[u].parentZ = currentNode.z;
					openList[u].parentG = currentNode.g;
					openList[u].calculateWeights(&adjacencyNodes[i],&endNode,heightDistance);
				}
			}
		}
				
		if(iterationDepth>MAXaStarIterationDepth) 
		{
			goto ENDNotFound;
		}
		
	}
	
	aStarERROR:
	cout << "Fehlerhaftes node in liste." << endl;
	return false;
	
	ENDNotFound:
	cout << "Resultat: Nicht gefunden - iterationdepth überschritten: " << iterationDepth << "   start=" << startNode << " end:" << endNode << endl;
	return false;
	
	ENDFound:
	#ifdef aStarDebug 
	cout << "Resultat: Gefunden timeToFind=" << (SDL_GetTicks()-startTime)/1000.0 << "  (" << iterationDepth << " iterations)" << endl;
	#endif
	
	
	
	#ifdef aStarDebug
	cout << "Final WAY=" << endl;
	#endif
	
	resultVectorReference.clear();
	rollOffPath(closedList,start,target,resultVectorReference);
	
	
	closedList.clear();
	openList.clear();
	
	
	return true;
}

void getGridPosition(awx_vector3f pos)
{
	pos = pos - awx_vector3f(currentLevel->terrain.xmin,0,currentLevel->terrain.zmin);
	
	//cout << "x=" << pos.x/aStarGridSize << "  y=" << pos.z/aStarGridSize << endl;
}

void cleanupAStarGrid()
{
	if(aStarGrid!=NULL)
		delete[] aStarGrid;
		
	if(aStarHeightGrid!=NULL)
		delete[] aStarHeightGrid;
}

void renderCurrentPath()
{
	#ifdef aStarPathDebug
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glColor3f(1.0,1.0,1.0);
			glPushMatrix();
				//glTranslatef(currentLevel->terrain.xmin,0,currentLevel->terrain.zmin); 
				glBegin(GL_LINE_STRIP);
					for(int i=0;i<drawPath.size();i++)
					{
						glVertex3f(drawPath[i].x,-5,drawPath[i].z);
					}
				glEnd();
			glPopMatrix();
	glPopAttrib();
	#endif
}