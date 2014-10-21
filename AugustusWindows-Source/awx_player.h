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

#include "awx_ki.h"
#include "awx_trueTypeFont.h"

typedef struct
{
	awx_string text;
	int texID;
} rohstoff;

vector<rohstoff> rohstoffe;
int arbeitTexID;

void initRohstoffe()
{
	rohstoff t;
	t.texID = 0;
	t.text = "Holz";
	rohstoffe.push_back(t);
	
	t.text = "Gold";
	rohstoffe.push_back(t);
	
	t.text= "Steine";
	rohstoffe.push_back(t);
	
	t.text = "Nahrung";
	rohstoffe.push_back(t);
}

const int AWXHolz=0;
const int AWXGold=1;
const int AWXSteine=2;
const int AWXNahrung=3;


class awx_player_sektor
{
		public:
		
		bool seen;
		float timeSinceLastVisit;
		
		awx_player_sektor()
		{
			seen=false;
			timeSinceLastVisit=-1;
		}
		
		void setVisited()
		{
			seen=true;
		}
};

typedef struct
{
	int buildingIndex;
	int jobIndex;
	
	float attractivity;
}beruf;

inline int cmp (const void *a, const void *b)
{
  float aa = ((beruf *)a)->attractivity;
  float ba = ((beruf *)b)->attractivity;
  return (aa < ba) ? -1 : (aa > ba) ? 1 : 0;
}

int operator==(const beruf &a , const beruf &b)
{
   if( a.attractivity != b.attractivity) return 0;
   return 1;
}

// This function is required for built-in STL list functions like sort
int operator<(const beruf &a , const beruf &b)
{
   if( a.attractivity < b.attractivity) return 1;
   return 0;
}


struct simplePlayerFunction
{
	int (awx_player::*fp)(void* data);
	awx_player *me;
	
	int exec(void *data)
	{
		return (me->*fp)(data);
	}
};

class buildingGrid
{
	public:
	
	int gridSize;
	
	awx_vector3f **gridData;
	
	void generate(int size, float step, awx_vector3f middle)
	{
		gridData = new awx_vector3f*[size];
	
		for(int i=0;i<size;i++)
		{
			gridData[i] = new awx_vector3f[size];
		}
		
		for(int x=0;x<size;x++)
		{
			for(int z=0;z<size;z++)
			{
				gridData[x][z] = (middle + awx_vector3f(x*size*0.5*step,0,z*0.5*size*step));
				gridData[x][z] = gridData[x][z] - awx_vector3f(size*step*1.5,0,size*step*1.5);
				gridData[x][z].y=0;
			}
		}
		
		gridSize=size;
	}
	
	void calculateNextFreeBlock(awx_vector3f &position)
	{
		int middle_x=gridSize/2;
		int middle_z=gridSize/2;
		
		int minX=0,minZ=0;
		
		float minDist=0xFFFFFFFF;
		
		for(int x=0;x<gridSize;x++)
		{
			for(int z=0;z<gridSize;z++)
			{
				if(gridData[x][z].y==0)
				{
					float dist = pow((float)x-middle_x,2.0f) + pow((float)z-middle_z,2.0f);
					if(dist<minDist)
					{
						minDist=dist;
						minX=x;
						minZ=z;
					}
				} 
			}
		}
		
         gridData[minX][minZ].y=1;

		position = awx_vector3f(gridData[minX][minZ].x,0,gridData[minX][minZ].z);
		
		cout << minX << "  " << minZ << endl;
	}
	
	void display()
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0,1.0,1.0,1.0);
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);
		
		for(int x=0;x<gridSize;x++)
		{
			for(int z=0;z<gridSize;z++)
			{
				if(gridData[x][z].y==0) glColor4f(1.0,1.0,1.0,1.0);
				else if(gridData[x][z].y==1) glColor4f(1.0,0.0,0.0,1.0);
				else glColor4f(0.0,0.0,0.0,1.0);
				
				glPushMatrix();
				glTranslatef(gridData[x][z].x,20,gridData[x][z].z);
				glutSolidCube(1);
				glPopMatrix();
			}
		}
		
		glPopAttrib();
	}
	
	void dealloc()
	{
		for(int i=0;i<gridSize;i++)
		{
			delete[] gridData[i];
		}
		
		delete[] gridData;
	}
};

class awxAICommand
{
	public:

	awx_string command,arguments;
};


class awx_player{
	public:
		vector <awx_unit> units;
		vector <awx_building> buildings;
		
		awx_dictionary produkte;
		vector <float> produktPreise;

		bool ki_player;
		
		float force;						// Gewichtungswert
		
		int *rohstoffvalues;
		
		int hauptHausTextID;				// TExturID
		int hauptHausBildID;
		int alteEinwohner;
		
		awx_player_sektor **gamePlaySektoren;
		int sektorenX,sektorenZ;
		bool notabbauing;
		long int startTimeMeldung;			
		
		awx_dictionary variableDictionary;
		awx_network neuronalNetwork;
		
		awx_vector3f startpunkt;
		
		float brunnen;
		float wirtshaus;
		
		int platz;
		int startPlatz;
		
		awx_fuzzy_rule_container buerger;
		
		float steuerSatz;
		
		
		//Wirtschaft:
		int wirtschaftInfoIndex;
		awx_string infoString;
		awx_string infoStringValues;
		int interfaceMode;
		
		/*int produktInfoIndex;
		awx_string produktMengen;
		awx_string produktMengenValues;*/
		
		int myPlayerIndex;
		
		
		// Was ist bebaut in meiner umgebung was nicht GRID (AI)
		buildingGrid myGrid;
		
		
		// ---------------------- Computergegner
		awx_decision_network *AIStrategies;
		awx_dictionary *data;
		bool isAIControlled;
		
		// Internes Function dictionary
		vector<awx_string> dictKeys;
		vector<simplePlayerFunction> dictFunctions;
		vector<awxAICommand> AICommands;
		
		int kannKaserneBauen;
	
		void addFunctionWithKey(int (awx_player::*fp)(void* data), awx_string key);
		int executeFunctionWithKey(awx_string key, void *data);
		int executeFunctionWithIndex(int index, void *data);
		
		int buildBuilding(void *data);
		int idle(void *data);
		int buildUnit(void *data);
		int baueRohstoffeAb(void *data);
	
		
		void runAICommandList();

		awx_player(){
			ki_player = false;
			alteEinwohner = 0;
			
			rohstoffvalues = new int[rohstoffe.size()];
		
			rohstoffvalues[0] = 50000;
			rohstoffvalues[1] = 20000;
			rohstoffvalues[2] = 10000;
			rohstoffvalues[3] = 300000;
			
			steuerSatz=1;
			
			setDictionary();
			
			brunnen = 0;
			wirtshaus = 0;
			startPlatz = 0;
			
			isAIControlled=false;
			myPlayerIndex=-1;
			
			kannKaserneBauen=0;
			
			interfaceMode = 0;
			
			int index = -1;
			awx_trueTypeFont tempFont;
			tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,150,1,true);
			tempFont.init();
			tempFont.calculate("T");
			float firstHeight = tempFont.getLineHeight();
			tempFont.calculate("");
			index = (int)ttfFonts.size();
			ttfFonts.push_back(tempFont);
			ttfFonts[index].setAspectCorrection(true);
			float heighti = (ttfFonts[index].getLineHeight()==0) ? firstHeight : ttfFonts[index].getLineHeight();
					
			float x = 0.81/project.aspect;
			float y = 0.9;
			ttfFonts[index].setRenderPosition(project.width*(x*project.aspect),project.height-project.height*y-heighti);
			wirtschaftInfoIndex = index;
			
			awx_trueTypeFont tempFont2;
			tempFont2.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,150,1,true);
			tempFont2.init();
			tempFont2.calculate("T");
			firstHeight = tempFont2.getLineHeight();
			tempFont2.calculate("");
			index = (int)ttfFonts.size();
			ttfFonts.push_back(tempFont2);
			ttfFonts[index].setAspectCorrection(true);
			heighti = (ttfFonts[index].getLineHeight()==0) ? firstHeight : ttfFonts[index].getLineHeight();
					
			x = 0.81/project.aspect + 0.05;
			y = 0.907;
			ttfFonts[index].setRenderPosition(project.width*(x*project.aspect),project.height-project.height*y-heighti);
			
		}
		
		~awx_player()
		{
			delete[] rohstoffvalues;
			
			// dealloc sektors
		}
		
		awx_player(const awx_player &p)
		{
			interfaceMode = 0;
			
			rohstoffvalues = new int[rohstoffe.size()];
			
			rohstoffvalues[0] = 50000;
			rohstoffvalues[1] = 20000;
			rohstoffvalues[2] = 10000;
			rohstoffvalues[3] = 300000;
			
			brunnen = 0;
			wirtshaus = 0;
			startPlatz = 0;
			
			buerger.debug = false;
			buerger.readFromFile("Data/ki/buerger.afsx");
			
			setDictionary();


			isAIControlled=p.isAIControlled;

			AIStrategies = p.AIStrategies;
			data = p.data;
			
			myPlayerIndex=p.myPlayerIndex;
			
			for(int i=0;i<p.dictKeys.size();i++)
			{
				dictKeys.push_back(p.dictKeys[i]);
				dictFunctions.push_back(p.dictFunctions[i]);
			}
			
			startpunkt = p.startpunkt;
			myGrid = p.myGrid;
			
			kannKaserneBauen=0;
		}
		
		void flooring(awx_building &building)
		{
			awx_vector3f position = awx_vector3f(building.x,building.y,building.z);
			awx_vector3f minX = position + awx_vector3f(building.xmin,0,0);
			awx_vector3f maxX = position + awx_vector3f(building.xmax,0,0);
			awx_vector3f minZ = position + awx_vector3f(0,0,building.zmin);
			awx_vector3f maxZ = position + awx_vector3f(0,0,building.zmax);

			int currentSektorX=-1,currentSektorZ=-1;
			bool ergebnisflag;
			vector<int> vals;
			int hitTriangle0 = currentLevel->terrain.hit(position , position.y , ergebnisflag, currentSektorX, currentSektorZ);


			float radius = (awx_vector3f(building.data[0].xmax,0,building.data[0].zmax)-awx_vector3f(building.data[0].xmin,0,building.data[0].zmin)).getLength() * 0.65;

			currentLevel->terrain.startUpdateGeometry();
			currentLevel->terrain.average(hitTriangle0,currentSektorX,currentSektorZ, position, 4, radius);
			currentLevel->terrain.stopUpdateGeometry();
		}

		
		void nextMode()
		{
			interfaceMode++;
			if(interfaceMode > 2)interfaceMode = 0;
		}
		
		void allocateSektoren(int x, int z)
		{
			sektorenX = x;
			sektorenZ = z;
			
			gamePlaySektoren = new awx_player_sektor*[sektorenX];
			for(int i=0;i<sektorenX;i++)
			{
				gamePlaySektoren[i] = new awx_player_sektor[sektorenZ];
			}
		}
		
		void plotGamePlaySektorInformation()
		{
			int seenSektors=0;
			for(int i=0;i<sektorenX;i++)
			{
				for(int u=0;u<sektorenZ;u++)
				{
					if(gamePlaySektoren[i][u].seen) seenSektors++;
				}
			}
			cout << "Gameplaysektoren seen: " << seenSektors << "/" << sektorenX*sektorenZ << endl;
		}
		
		bool somethingChanged()
		{
			if(alteEinwohner!=(int)units.size())
			{
				alteEinwohner=(int)units.size();
				return true;
			} 
			return false;
		}
		
		// wildeste funktion dieses projekts
		
		void renderButtons()
		{
			if(interfaceMode != 0)
			{
				
				awx_window win;
				win = infoWindow;
				win.width = 0.24;
				win.height = 0.3;
				win.x = project.aspect-win.width - 0.05;
				win.y = 1-win.height-0.05;
			
				win.render();
				if(!(infoString == ttfFonts[wirtschaftInfoIndex].caption))
				{
					ttfFonts[wirtschaftInfoIndex].setCaption(infoString);
					ttfFonts[wirtschaftInfoIndex].calculate(infoString);
				}
				else if(!(infoStringValues == ttfFonts[wirtschaftInfoIndex+1].caption))
				{
					ttfFonts[wirtschaftInfoIndex+1].setCaption(infoStringValues);
					ttfFonts[wirtschaftInfoIndex+1].calculate(infoStringValues);
				}
				/*else if(!(produktMengen == ttfFonts[produktInfoIndex].caption))
				{
					ttfFonts[produktInfoIndex].setCaption(produktMengen);
					ttfFonts[produktInfoIndex].calculate(produktMengen);
				}
				else if(!(produktMengenValues == ttfFonts[produktInfoIndex+1].caption))
				{
					ttfFonts[produktInfoIndex+1].setCaption(produktMengenValues);
					ttfFonts[produktInfoIndex+1].calculate(produktMengenValues);
				}*/
				
				ttfFonts[wirtschaftInfoIndex].registerRender();
				ttfFonts[wirtschaftInfoIndex+1].registerRender();
			/*	ttfFonts[produktInfoIndex].registerRender();
				ttfFonts[produktInfoIndex+1].registerRender();*/
				
			}
		}
		
		float getPriceFor(awx_string name)
		{
			for(int i = 0; i < produkte.keys.size();i++)
			{
				if(produkte.keys[i] == name)
				{
					return produktPreise[i];
				}
			}
			return 0;
		}
		
		void processKI()
		{
			//cout << "update player AI" << endl;
			/*
			
			float bauzeit;
			float v;
	
			float health_max;
			float health;
	
			float schaden_max;
			float schaden_min;
		
			float reichweite;
			float armor = 100;
			float angriffswert = 500;
			
			20%
			*/
			//float speed = 0.8;
			
			/*vector<int> unusedUnits;
			 
			
			for(int i = 0;i < units.size();i++)
			{
				if(units[i].myRessID != -1 && !units[i].rohstoffabbauIndirect && !units[i].rohstoffabbau)
				{
					unusedUnits.push_back(i);
				}
			}
			
			if(unusedUnits.size() == 0)
			{
				int anzahl = int(speed*5);
				for(int i = 0; i < anzahl;i++)buildings[0].buildUnit("arbeiter");
			}*/
			//Uint32 s = SDL_GetTicks();
			
			//int hauscount = speed * 3;
			//cout << "KI" << endl;
			int wirtshauscount = 0;
			int brunnencount = 0;
			platz = startPlatz;
			
			for(int i = 0; i < buildings.size();i++)
			{
				awx_string temp = buildings[i].workingdirectory.lastPathComponent();
				//cout << temp << endl;
				if(buildings[i].fortschritt >= 1)
				{
					if(temp == "wirtshaus")wirtshauscount++;
					else if(temp == "brunnen")brunnencount++;
					
					platz += buildings[i].wohnplatz;
				}
			}
						
			int freierPlatz = platz;
			for(int i = 0; i < units.size();i++)
			{
				if(units[i].buerger)freierPlatz--;
			}
			
			wirtshaus = wirtshauscount/float(units.size());
			brunnen = brunnencount/float(units.size());			
			
			//if(units.size() > 0)buerger.debug = true;
			
			buerger.setValue("wirtshauscount",wirtshaus);
			buerger.setValue("brunnencount",brunnen);
			
			vector <awx_string> namen;
			vector <float> values;
			buerger.calculate(namen,values);
			
			//if(units.size() > 0)buerger.debug = false;
			//cout << freierPlatz << endl;
			if(namen.size() > 0)
			{
				int andrang = int((values[0]-0.0111622) * (freierPlatz));
				if(andrang > 0)cout << "Andrang: " << andrang << endl;
				for(int i = 0; i < (andrang < 4 ? andrang : 4);i++)
				{
					buildings[0].buildUnit("buerger" , false);
				}
			}
			//else cout << "Fuzzy Problem" << endl;
			
			namen.clear();
			values.clear();
			
			force = 0;
			for(int i = 0; i < (int)units.size();i++){
				force += units[i].calcForce();
			}
			
			
			
			
			
			
			
			
			
			vector <beruf> berufe;
			
			for(int i = 0; i < buildings.size();i++)
			{
				if(buildings[i].fortschritt > 1)
				{
					for(int u = 0; u < buildings[i].berufe.size();u++)
					{
						//cout << buildings[i].berufe[u].name << " " << buildings[i].berufe[u].unitIndex << " " << buildings[i].berufe[u].buildingIndex << endl;
						if(!buildings[i].berufe[u].isVergeben())
						{
							beruf tempb;
						
							tempb.buildingIndex = i;
							tempb.jobIndex = u;
							tempb.attractivity = buildings[i].berufe[u].getAttractivity();
						
							berufe.push_back(tempb);
						}
					}
				}
			}
			
			sort(berufe.begin() , berufe.end());
			for(int i = 0; i < berufe.size();i++)
			{
				cout << i+1 << ": " << buildings[berufe[i].buildingIndex].berufe[berufe[i].jobIndex].name << " " << berufe[i].attractivity << endl;
			}
			int berufindex = berufe.size()-1;
			
			for(int i = 0; i < units.size();i++)
			{
				if(berufindex < 0)break;
				
				if(units[i].beruf == NULL &&
				   units[i].buerger)
				{					
					beruf tempb = berufe[berufindex];

					units[i].beruf = &buildings[tempb.buildingIndex].berufe[tempb.jobIndex];
					
					cout << "Unit["<< i << "] arbeitet jetzt in Haus[" << tempb.buildingIndex << "] als \"" << buildings[tempb.buildingIndex].berufe[tempb.jobIndex].name << "\"" << endl;
					buildings[tempb.buildingIndex].berufe[tempb.jobIndex].unitIndex = i;
					berufindex--;
				}
			}

						
			for(int i = 0; i < units.size();i++)
			{
				if(berufindex < 0)break;
				if(units[i].beruf != NULL && units[i].beruf->getAttractivity()*1.2 < berufe[berufindex].attractivity &&
				   units[i].buerger)
				{
					beruf tempb = berufe[berufindex];

					units[i].beruf->unitIndex = -1;
					units[i].beruf = &buildings[tempb.buildingIndex].berufe[tempb.jobIndex];
					
					units[i].working = false;
					units[i].inFirma = false;
					units[i].invisible = false;
					
					cout << "Unit["<< i << "] arbeitet jetzt in Haus[" << tempb.buildingIndex << "] als \"" << buildings[tempb.buildingIndex].berufe[tempb.jobIndex].name << "\"" << endl;
					buildings[tempb.buildingIndex].berufe[tempb.jobIndex].unitIndex = i;
					berufindex--;
				}
			}
			
			berufe.clear();
			
			produktPreise.clear();
			awx_string infoString2 = "Marktpreise:\n\n";
			awx_string infoStringValues2 = "\n\n";
			for(int i = 0; i < produkte.keys.size();i++)
			{
				vector <float> preise2;
				for(int u = 0; u < buildings.size();u++)
				{
					for(int o = 0; o < buildings[u].berufe.size();o++)
					{
						if(buildings[u].berufe[o].isVergeben())
						{
							for(int a = 0; a < buildings[u].berufe[o].produktIndices.size();a++)
							{
								if(::produkte[buildings[u].berufe[o].produktIndices[a]] == produkte.keys[i])
								{								
									float preis = (buildings[u].berufe[o].einkommen/float(buildings[u].berufe[o].produktIndices.size()))/float( buildings[u].berufe[o].produktMenge[a]);
									preise2.push_back(preis);
								}
							}
						}
					}
				}
				
				float summe = 0;
				for(int u = 0; u < preise2.size();u++)
				{
					summe += preise2[u];
				}
				if(preise2.size() != 0)
				{
					produktPreise.push_back(summe/float(preise2.size()));
				}
				else produktPreise.push_back(0);
				char temp[128];
				sprintf(temp , "$%.2f" , produktPreise[i]);
				infoString2 = infoString2 + produkte.keys[i] + *(new awx_string(":   ")) + *(new awx_string("\n"));
				infoStringValues2 = infoStringValues2 + *(new awx_string(temp)) + *(new awx_string("\n"));
				//cout << produkte.keys[i] << ": " << produktPreise[i] << "$" << endl;
			}
			
			infoStringValues = infoStringValues2;
			infoString = infoString2;
			
			if(interfaceMode == 2)
			{
				awx_string produktInfo2 = "Produktmengen:\n\n";
				awx_string produktInfoValues2 = "\n\n";
				for(int i = 0; i < produkte.keys.size();i++)
				{
					produktInfo2 = produktInfo2 + produkte.keys[i] + ":\n";
					produktInfoValues2 = produktInfoValues2 + *(new awx_string((int)produkte.values[i].floatValue())) + "\n";
				}
				infoString = produktInfo2;
				infoStringValues = produktInfoValues2;
			}
			if(playedTime.getTime() > 2)
			{
				for(int i = 0; i < units.size();i++)
				{
					units[i].fuzzyLogicValues.calcBaugeschwindigkeit();
				}
			}
			
			/*awx_dictionary dict;
			for(int i = 0; i < units.size();i++)
			{
				awx_beruf* tempberuf = units[i].beruf;
				
				int counter = 0;
				if(tempberuf != NULL)
				{
					for(int u = i+1; u < units.size();u++)
					{
						if(tempberuf == units[u].beruf)
						{
							dict.setObject(awx_string(counter) , tempberuf->name.cString());
							counter++;
						}
					}
				}
			}*/

			setDictionary();
			
			//cout << "unitssize=" << units.size() << endl;
			//neuronalNetwork.processNeuron(0,variableDictionary);
			
			//cout << (SDL_GetTicks()-s)/1000.0 << endl;
			
		}	
		
		void kiNachdenk()
		{		
			processAIStrategies();
		}
		
		void processAIStrategies();
		void initAIStrategies();
		void deallocAIStrategies();
		
		void setDictionary()
		{
			variableDictionary.setObject(rohstoffvalues[AWXHolz],"$holz");
			variableDictionary.setObject(rohstoffvalues[AWXGold],"$gold");
			variableDictionary.setObject(rohstoffvalues[AWXSteine],"$steine");
			variableDictionary.setObject(rohstoffvalues[AWXNahrung],"$nahrung");
			variableDictionary.setObject((int)units.size(),"$unitcount");
		}
		
		void prepareGameForPlayer()
		{
			int anzahl_sklaven=5;
			for(int i=0;i<anzahl_sklaven;i++)
			buildings[0].buildUnit("arbeiter",false);
		}
		
		int writeToFile(FILE *file)
		{
			int ret=0;
			cout << "Writing buildings:" << endl;
			fprintf(file,";;size=%i;;",buildings.size());
			for(int i=0;i<buildings.size();i++)
			{
				ret |= buildings[i].writeToFile(file);
			}
			cout << "Writing units:" << endl;
			fprintf(file,";;size=%i;;",units.size());
			for(int i=0;i<units.size();i++)
			{	
				ret |= units[i].writeToFile(file);
			}
			
			return ret;
		}
		
		int loadFromFile(FILE *file, int playerIndex)
		{
			int ret=0;
			int size=0;
			cout << "Reading buildings from player: " << playerIndex<< endl;
			fscanf(file,";;size=%i;;",&size);
			awx_building tempBuilding;
			for(int i=0;i<size;i++)
			{
				tempBuilding.loadFromFile(file);
				loadAndInitBuilding(tempBuilding.myPackageName.cString(),awx_vector3f(tempBuilding.x,tempBuilding.y,tempBuilding.z),playerIndex,tempBuilding);
			}
			cout << "Reading units from player: " << playerIndex<<endl;
			fscanf(file,";;size=%i;;",&size);
			cout << "unitssize=" << size << endl;
			awx_unit tempUnit;
			for(int i=0;i<size;i++)
			{
				tempUnit.loadFromFile(file);
				
				loadAndInitUnit(tempUnit,playerIndex);
			}
			
			return ret;
		}
		
		void loadAndInitBuilding(awx_string name,awx_vector3f position, int playerIndex, awx_building &t)
		{
			bool flag = false;
			int index=0;
			int playerInd=100;
	
			for(int u = 0; u < (int)player.size();u++){
			cout << ::player[u].buildings.size() << endl;
				for(int i = 0; i < (int)::player[u].buildings.size();i++){
					if(::player[u].buildings[i].workingdirectory.lastPathComponent().compare(name)==0)
					{
						index = i;		
						playerInd = u;
						flag = true;
						break;
					}
				}
			}
	
			awx_building tempb;
			
			tempb.health = t.health;
			tempb.fortschritt = t.fortschritt;
			tempb.winkel_y=t.winkel_y;
	
			if(flag){
				tempb.reinit(player[playerInd].buildings[index]);
				tempb.fortschritt = 0;
				tempb.playerIndex = playerIndex;
				//tempb.myIndex = buildings.size();
				buildings.push_back(tempb);
				buildings[buildings.size()-1].myIndex = buildings.size()-1;
			}
			else{
				tempb.openFile(("Data/buildings/" + *(new awx_string(name)) + "/zusammenbau.aax").cString());
				tempb.fortschritt = 0;
				playerInd = playerIndex;
				//tempb.myIndex = buildings.size();
				buildings.push_back(tempb);
				buildings[buildings.size()-1].myIndex = buildings.size()-1;
				cout << "Neues building geladen. PlayerIndex=" << playerIndex << "  index=" << player[playerIndex].buildings.size() << endl;
			}
			
			// PART 2
			
			buildings[buildings.size()-1].x = position.x;
			buildings[buildings.size()-1].y = position.y;
			buildings[buildings.size()-1].z = position.z;
			
			buildings[buildings.size()-1].fortschritt=1.0;
	
			//updateAStarGridAtObject(player[controlIndex].buildings[player[controlIndex].buildings.size() -1],position);
	
			//player[playerIndex].buildings[player[playerIndex].buildings.size()-1].calculateShadow();
	
			if(project.shadowvolumes&&player[playerIndex].buildings.size()>=2&&buildings[buildings.size()-1].winkel_y!=buildings[buildings.size()-2].winkel_y)
			{
				buildings[buildings.size()-1].prepareShadowVolume();
			}
			else if(project.shadowvolumes&&buildings.size()<2) buildings[buildings.size()-1].prepareShadowVolume();

		}
		
		
		void loadAndInitUnit(awx_unit &unit, int playerIndex)
		{
			awx_string name = unit.workingdirectory;
            bool flag = false;
            int index;
            int playerInd;
            
            for(int u = 0; u < (int)player.size();u++){
                for(int i = 0; i < (int)player[u].units.size();i++){
                    if(player[u].units[i].workingdirectory.compare(name)  == 0){
                        flag = true;
                        index = i;
                        playerInd = u; 
                        break;
                    }
                }
            }
            
            awx_unit tempu;
            tempu.playerIndex = playerIndex;
            
            if(!flag)
            {
                tempu.openFile(((*(new awx_string(name))) + "/zusammenbau.aax").cString());
                tempu.loadBlobs("Data/textures/blob.tga",1.2);
				tempu.position.x=unit.modelview[12];
				tempu.position.y=unit.modelview[13];
				tempu.position.z=unit.modelview[14];
			
                units.push_back(tempu);
                
        
                units[(int)units.size()-1].anvisiert = false;
                units[(int)units.size()-1].hasDir = false;
                units[(int)units.size()-1].myIndex = (int)units.size()-1;
				units[(int)units.size()-1].unitValues.leben = unit.unitValues.leben;
				units[(int)units.size()-1].fortschritt = 100;
				units[(int)units.size()-1].data[0].current_frame = unit.loadedCurrentFrame;
                
               /*for(int i = 0; i < (int)namen.size();i++){
                    if(namen[i].compare(name) == 0){
                        units[units.size()-1].myButtonID = buttons[i].texID;
                        break;
                    }
                }*/
            }
            else
            {
				cout << "USING reference " << endl;
			
                tempu.reinit(player[playerInd].units[index]);
				tempu.position.x=unit.modelview[12];
				tempu.position.y=unit.modelview[13];
				tempu.position.z=unit.modelview[14];

                units.push_back(tempu);
                
                units[units.size()-1].blobtexturID = player[playerInd].units[index].blobtexturID;
                units[units.size()-1].blobsizex = player[playerInd].units[index].blobsizex;
                units[units.size()-1].blobsizey=player[playerInd].units[index].blobsizey;
                units[units.size()-1].anvisiert = false;
                units[units.size()-1].hasDir = false;
                
                units[units.size()-1].buttons = player[playerInd].units[index].buttons;
                units[units.size()-1].attributes = player[playerInd].units[index].attributes;
                units[units.size()-1].workingdirectory = player[playerInd].units[index].workingdirectory;
                units[units.size()-1].namen = player[playerInd].units[index].namen;
                units[units.size()-1].myIndex = (int)player[playerInd].units.size()-1;
				units[(int)units.size()-1].unitValues.leben = unit.unitValues.leben;
				units[(int)units.size()-1].fortschritt = 100;
				units[(int)units.size()-1].data[0].current_frame = unit.loadedCurrentFrame;
                
                for(int i = 0; i < (int)player[playerInd].units[index].namen.size();i++){
                    if(player[playerInd].units[index].namen[i].compare(name) == 0){
                        units[units.size()-1].myButtonID = player[playerInd].units[index].buttons[i].texID;
                        break;
                    }
                }
            }
        }
};