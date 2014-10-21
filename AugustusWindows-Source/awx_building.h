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

#include "awx_building_utilities.h"

class buttoncontainer
{
	public:
	
	int unitID;
	float fortschritt;
};

class awx_building : public aax, public visibilityCheck, public awx_blob
{
	public:
		int playerIndex;
		
		vector <awx_button> buttons;
		vector <awx_inputTextField> textFields;
		vector <int> berufTextIndices;
		
		vector <awx_string> namen;
		vector <awx_string> einheitennamen;
		vector <awx_string> descriptions;
		vector <awx_string> levelPartnamen;
		
		vector <awx_beruf> berufe;
		int myIndex;
		int freierPlatz;
		
		awx_vector2f myKopfPositionInWindowCoords;
		
		vector <int> unitQue;
		
		float bauGeschwindigkeit;
		float bauzeit;
		float fortschritt;
		int art;
		
		float health;
		float armor;
		float health_max;
		int wohnplatz;
		bool burning;
		
		vector <opponentUnit> attacked;
		
		int descriptionIndex;
		float winkel_y;
		
		vector<int> ressIDs;
		
		int myFirstTextFieldID;
		
		awx_vector3f fahne;		// Fahnenposition in worldcoords
		
		GLdouble progressX,progressY,progressZ;
		
		awx_particleSystem *myParticleSystem;
		awx_particleSystem *houseFire;
		awx_particleSystem *houseFireSmoke;
		
		int hitTriangle;
		
		// Baueigenschaften
		int holzkosten;
		int steinkosten;
		int goldkosten;
		
		float specialAttribute;		// MŸhlenradwinkel
		
		vector<awx_string> updateIdentifiers;
		
		awx_string myFileName;
		awx_string myPackageName;
		
		awx_string unitPrice;
		int unitPriceIndex;
		
		awx_string meinName;
	
		awx_building()
		{
			aax::aax();
			
			descriptionIndex = -1;
			bauGeschwindigkeit=1;
			bauzeit = 10;
			
			fahne = awx_vector3f(AWXNull,AWXNull,AWXNull);
			
			winkel_y=0;
			art = 0;
			
			playerIndex = 0;
			
			myParticleSystem=NULL;
			
			wohnplatz = 0;
			
			specialAttribute=0;			// MŸhlenradwinkel
			
			houseFire=NULL;
			
			myIndex = -1;
			freierPlatz = 0;
			
			unitPriceIndex = -1;
			
			int index = -1;
			float firstHeight;
			float heighti;
			
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
					
			x = 0.84/project.aspect;
			y = 0.17;
			ttfFonts[index].setRenderPosition(project.width*(x*project.aspect),project.height-project.height*y-heighti);
			
			unitPriceIndex = index;
			
			/*awx_beruf beruf;
			
			char* temp = new char[256];
			sprintf(temp , "%s;Der Baecker produziert Brot;einkommen:5.000000;anstrengung:%f;startArbeit:%f;arbeitsdauer:%f;Bier:10#Brot:5#;" ,workingdirectory.cString() , RANDOM_FLOAT , 5+RANDOM_FLOAT*7 , RANDOM_FLOAT*12);
			beruf.interpretString(*(new awx_string(temp)));
			
			berufe.push_back(beruf);*/
		}
		
		void buildUnit(char* name, bool userControlled);
		void buildLevelPart(string name);
		void calculateQue();
		void eraseQue();
		void lichtRadius();
		
		bool boundingCollision(awx_vector3f position){
            if(number_of_objects < 3)return false;
            awx_vector3f x1(data[2].xmin , 0 , data[2].zmin);
            awx_vector3f x2(data[2].xmax , 0 , data[2].zmin);
            awx_vector3f x3(data[2].xmin , 0 , data[2].zmax);
			
			glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
                glLoadIdentity();
                glTranslatef(x,0,z);
                glRotatef(winkel_y,0.0,1.0,0.0);
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
		
		void reinit(awx_building &wild){
			number_of_objects = wild.number_of_objects;
			data = new apx[wild.number_of_objects];
			for(int i=0;i<wild.number_of_objects;i++)
			{
				data[i] = wild.data[i];
				data[i].xmax = wild.data[i].xmax;
				data[i].zmax = wild.data[i].zmax;
				data[i].xmin = wild.data[i].xmin;
				data[i].zmin = wild.data[i].zmin;
				data[i].shadowVolumeID = wild.data[i].shadowVolumeID;
				
				data[i].current_frame=0;
			}
			for(int i = 0; i < (int)wild.namen.size();i++){
				namen.push_back(wild.namen[i]);
				einheitennamen.push_back(wild.einheitennamen[i]);
				descriptions.push_back(wild.descriptions[i]);
			}
			myFirstTextFieldID = wild.myFirstTextFieldID;
			bauzeit = wild.bauzeit;
			health_max = wild.health_max;
			armor = wild.armor;
			health = health_max;
			ressIDs = wild.ressIDs;
			updateIdentifiers = wild.updateIdentifiers;
			art = wild.art;
			holzkosten = wild.holzkosten;
			steinkosten = wild.steinkosten;
			goldkosten = wild.goldkosten;
			wohnplatz = wild.wohnplatz;
			freierPlatz = wohnplatz;
			
			float y = 0.12;
			berufe.clear();
			for(int i = 0; i < wild.berufe.size();i++)
			{
				berufe.push_back(wild.berufe[i]);
				
				awx_inputTextField textf;
				textf.setTexture(pressedTextFieldID);
				textf.width = 0.1;
				textf.height = 0.03;
				textf.setColors(1,0.8,0,1.0  ,0.8,0.5,0,1.0);
				textf.x = 0.5*project.aspect + 0.16;
				textf.y = y;
				textf.initCaption();
				textf.setBehavior(true,true,0);	
				char temp[256];
				sprintf(temp , "%.2f" , wild.berufe[i].einkommen);
				textf.setCaption(temp);
				textf.calculatePositions(8,1);
				ttfFonts[textf.ttfTextID].calculate(textf.caption);	
				textFields.push_back(textf);
					
				
				y -= 0.035;
				
				berufe[i].unitIndex = -1;
				berufe[i].buildingIndex = myIndex;
			}
			
			berufTextIndices.clear();
			for(int i = 0; i < wild.berufTextIndices.size();i++)
			{
				berufTextIndices.push_back(wild.berufTextIndices[i]);
			}
			
			buttons = wild.buttons;
			attributes = wild.attributes;
			workingdirectory = wild.workingdirectory;
			namen = wild.namen;
			myFileName = wild.myFileName;
			myPackageName = wild.myPackageName;
			
			meinName = wild.meinName;
			x=wild.x;
			y=wild.y;
			z=wild.z;
			fortschritt=wild.fortschritt;
		}
		
		int openFile(const char* filename){
		
			cout << "FILE=" << filename << endl;
			
			if(strcmp(filename,"Data/buildings/schmiede/zusammenbau.aax")==0)
			{
				myParticleSystem = new awx_particleSystem();
				
				cout << "using particleSystem" << endl;
				myParticleSystem->loadFromFile("Data/ParticleSystems/smoke3.apsys");
				cout << myParticleSystem->serialize() << endl;
			} 
			else if(strcmp(filename,"Data/buildings/wirtshaus/zusammenbau.aax")==0)
			{
				myParticleSystem = new awx_particleSystem();
				
				cout << "using lorti test-particleSystem" << endl;
				myParticleSystem->loadFromFile("Data/ParticleSystems/beautiful_smoke.apsys");
				cout << myParticleSystem->serialize() << endl;
			}
			else if(strcmp(filename,"Data/buildings/brunnen/zusammenbau.aax")==0)
			{
				myParticleSystem = new awx_particleSystem();
				
				cout << "using lorti test-particleSystem" << endl;
				myParticleSystem->loadFromFile("Data/ParticleSystems/brunnen.apsys");
				cout << myParticleSystem->serialize() << endl;
			}

			
			//Berufe einlesen:
			awx_string tempstr = filename;
			
			tempstr = tempstr.stringByDeletingLastPathComponent();
			tempstr = tempstr.stringByAppendingPathComponent("berufe.txt");
			
			awx_string content;
			content.readFromFile(tempstr);
			if(content.length() > 0)
			{
				vector <awx_string> jobs;
				content.split('\n' , jobs);
			
				float y = 0.12;
				berufe.clear();
				for(int i = 0; i < jobs.size();i++)
				{
					awx_beruf tempb;
					tempb.interpretString(jobs[i]);
				
					awx_inputTextField textf;
					textf.setTexture(pressedTextFieldID);
					textf.width = 0.1;
					textf.height = 0.03;
					textf.setColors(1,0.8,0,1.0  ,0.8,0.5,0,1.0);
					textf.x = 0.5*project.aspect + 0.16;
					textf.y = y;
					textf.initCaption();
					textf.setBehavior(true,true,0);	
					char temp[256];
					sprintf(temp , "%.2f" , tempb.einkommen);
					textf.setCaption(temp);
					textf.calculatePositions(8,1);	
					ttfFonts[textf.ttfTextID].calculate(textf.caption);
					textFields.push_back(textf);
					
					
					
					int index = -1;
					awx_trueTypeFont tempFont;
					tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,150,1,true);
					tempFont.init();
					tempFont.calculate("T");
					float firstHeight = tempFont.getLineHeight();
					tempFont.calculate(tempb.name);
					index = (int)ttfFonts.size();
					ttfFonts.push_back(tempFont);
					ttfFonts[index].setAspectCorrection(true);
					float heighti = (ttfFonts[index].getLineHeight()==0) ? firstHeight : ttfFonts[index].getLineHeight();
					
					float x = 0.5/1.33;
					ttfFonts[index].setRenderPosition(project.width*(x*project.aspect),project.height-project.height*y-heighti);
					
					berufTextIndices.push_back(index);
					
					berufe.push_back(tempb);
					berufe[i].unitIndex = -1;
					berufe[i].buildingIndex = myIndex;
					
					
					y -= 0.035;
				}
			}
			
			//Attribute einlesen:
		
			awx_string pfad = filename;
			myFileName = pfad;
			pfad = pfad.stringByDeletingLastPathComponent();
			myFileName = pfad.lastPathComponent();
			meinName=pfad.lastPathComponent();
			workingdirectory = pfad;
			myPackageName = pfad.lastPathComponent();
			cout << workingdirectory << endl; 
			pfad = pfad.stringByAppendingPathComponent(*(new awx_string("config.att")));
			
			if(!attributes.initWithFile(pfad.cString())){
				cout << ".att Datei Konnte nicht gefunden werden!" << endl;
			}
			
			//Attribute setzen:
				wohnplatz = attributes.getObjectForKey("Wohnplatz").intValue();
				freierPlatz = wohnplatz;
				bauzeit = attributes.getObjectForKey("Bauzeit").floatValue();
				health = attributes.getObjectForKey("Lebenspunkte").floatValue();
				health_max = health;
				armor = attributes.getObjectForKey("Ruestung").floatValue();
				awx_string ressIDsString = attributes.getObjectForKey("ressIDs");
				vector<awx_string> vec;
				ressIDsString.split(',',vec);
				for(int i=0;i<vec.size();i++)
				{
					ressIDs.push_back(vec[i].intValue());
				}
				
				art = attributes.getObjectForKey("art").intValue();
				
				holzkosten = attributes.getObjectForKey("holzkosten").intValue();
				steinkosten = attributes.getObjectForKey("steinkosten").intValue();
				goldkosten = attributes.getObjectForKey("goldkosten").intValue();
				awx_string updateString = attributes.getObjectForKey("updates");
				updateString.split(',',updateIdentifiers);

				
			awx_string temp = attributes.getObjectForKey("units");
			
			awx_button tempButton;
			
			tempButton.hasPushedState = true;
			tempButton.hasRollOverState = true;
        
			tempButton.width = 0.0457;
			tempButton.height = 0.0457;
                    
			tempButton.x = 0.0175 * 1.33;
			tempButton.y = 0.05;
			
			if(temp.length() > 0)
			{
			    temp.split(',' , namen);
                
                myFirstTextFieldID = (int)ttfFonts.size();
                
                for(int i = 0; i < (int)namen.size();i++){
                    awx_dictionary temp;
                    temp.initWithFile(("Data/Units/" + namen[i] + "/config.att").cString());
                    descriptions.push_back(temp.getObjectForKey("Beschreibung"));
                    
                    char tempString[255];
                    sprintf(tempString,"#b %s\n\n%s",temp.getObjectForKey("Name").cString(),(temp.getObjectForKey("Beschreibung")).cString());
                    
                    // Setup Text
                    awx_trueTypeFont tempFont;
                    tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,1,300,1,true);
                    tempFont.init();
                    tempFont.caption = tempString;
                    tempFont.calculate();
                    ttfFonts.push_back(tempFont);
                    
                    
                    
                    einheitennamen.push_back(temp.getObjectForKey("Name"));
                    load_tga(("Data/Units/" + namen[i] + "/button.tga").cString(),currentID,false,true);
                    tempButton.setTexture(currentID);
                    currentID++;
                    
                    buttons.push_back(tempButton);
                    
                    string s = namen[i].cString();
                        
                    tempButton.y+=tempButton.height + 0.0053333333333;
                }
			}
			hasShadowVolume = true;
			
			// levelpartbuttons laden. (feld)
			awx_string temp2 = attributes.getObjectForKey("isAbleToBuildLevelParts");
			
		
			if(temp2.length() > 0)
			{
				temp2.split(',' , levelPartnamen);
				for(int i = 0; i < (int)levelPartnamen.size();i++){
					if(levelPartnamen[i].compare("")==0) 
					{	
						cout << "keine felder. continue" << endl;
						continue;
					}
					awx_dictionary temp;
					temp.initWithFile(("Data/buildings/" + levelPartnamen[i] + "/part0.att").cString());
					descriptions.push_back(temp.getObjectForKey("Beschreibung"));
				
					char tempString[255];
					sprintf(tempString,"#b %s\n\n%s",temp.getObjectForKey("Name").cString(),(temp.getObjectForKey("Beschreibung")).cString());
				
					// Setup Text
					awx_trueTypeFont tempFont;
					tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,1,300,1,true);
					tempFont.init();
					tempFont.caption = tempString;
					tempFont.calculate();
					ttfFonts.push_back(tempFont);
				
				
				
					einheitennamen.push_back(temp.getObjectForKey("Name"));
					load_tga(("Data/buildings/" + levelPartnamen[i] + "/button.tga").cString(),currentID,false,true);
					tempButton.setTexture(currentID);
					currentID++;
				
					buttons.push_back(tempButton);
					
					tempButton.y+=tempButton.height + 0.0053333333333;
				}
			}
			
			
			return aax::openFile(filename);
		}
		
		void startFire();
		
		void renderButtons();
		
		void myWindowHeadPosition()
		{
			GLdouble mvmatrix[16];
			GLdouble winx,winy,winz;
	
			glPushMatrix();
			glTranslatef(x,y+7,z);
			glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
		
			gluProject(0,0,0,mvmatrix,project.projmatrix,project.viewport,&winx,&winy,&winz);
			glPopMatrix();
		
			myKopfPositionInWindowCoords = awx_vector2f(winx,winy);
			//cout << myKopfPositionInWindowCoords << endl;
		}
		
		
		void keyDown(int code)
		{
			for(int i = 0; i < textFields.size();i++)
			{
				if(textFields[i].pushedFlag)
				{
					textFields[i].addCharacter(code);
				}
			}
		}
		
		void mouseDown(int xi,int yi);
		void mouseOver(int xi , int yi);
		void mouseUp(int xi , int yi);
		
		void checkVisibility()
		{
			visibilityCheck::checkVisibility(x,y,z);
			
			if(myParticleSystem!=NULL) myParticleSystem->isVisible=visible;
		}
		
		void renderBlank()
		{
			glDisable(GL_LIGHTING);
			glColor4f(1.0,1.0,1.0,1.0);
				data[2].render();
			glEnable(GL_LIGHTING);
		}
		
		void updateBerufe();
		
		int render()
		{
			//cout << workingdirectory.lastPathComponent() << " " << myIndex << endl;
			for(int i = 0; i < berufe.size();i++)
			{
				berufe[i].buildingIndex = myIndex;
			}
			updateBerufe();
			
			glPushMatrix();

			glTranslatef(x,y,z);
			glRotatef(winkel_y,0.0,1.0,0.0);
	
			if(fortschritt>=1.0)
			{
				data[2].render();
				if(number_of_objects==4)
				{
					glPushMatrix();
					
					glTranslatef(0,4.280,1.32);
					
					glRotatef(specialAttribute,0.0,0.0,1.0);
					
					data[3].render();
					glPopMatrix();
					
					specialAttribute-=5.0f/(float)runtime.fps;
				}
			}
			else if(fortschritt>=0.5)
			data[1].render();
			else data[0].render();
	
			
			drawProgress();
		
	
			glPopMatrix();
			
			
			
			return 0;
		}
		
		void renderSpecialEffects()
		{
			if(fortschritt<1) return;
			if(myParticleSystem!=NULL)
			{
				glPushMatrix();
				
				/* LORTI-GEHACKE fŸr Positionen der Partikelsysteme */
				
				if(strcmp(meinName.cString(),"schmiede")==0)
				{
					myParticleSystem->position=awx_vector3f(x-2.1,y+3.5,z-0.562);
				} 
				else if(strcmp(meinName.cString(),"wirtshaus")==0)
				{
					myParticleSystem->position=awx_vector3f(x+1.3,y+5.0,z-0.5);
				}
				else if(strcmp(meinName.cString(),"brunnen")==0)
				{
					myParticleSystem->position=awx_vector3f(x,y+0.1,z);
				}

				glPushAttrib(GL_ALL_ATTRIB_BITS);
				myParticleSystem->render();
				
				glPopAttrib();
				
				glPopMatrix();
			}
			if(houseFire!=NULL)
			{
				glPushMatrix();
				
				float deltaX = (data[2].xmax-data[2].xmin)*0.25;
				float deltaZ = (data[2].zmax-data[2].zmin)*0.25;
				
				houseFire->position=awx_vector3f(x,y+1.5,z); //x-2.32,y+3.4,z-0.562
				houseFireSmoke->position=awx_vector3f(x,y+1.5,z);
				
				houseFire->plane = awx_vector3f(deltaX,1,deltaZ);
				houseFireSmoke->plane = awx_vector3f(deltaX*2.0,1,deltaZ*2.0);
				
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				
				
				//glDisable(GL_DEPTH_TEST);
				
				houseFireSmoke->render();
				houseFire->render();
				
				glPopAttrib();
				
				glPopMatrix();
				
								
				
				health -= 100.0f/(float)runtime.fps;
			}
		}
		

		void renderExtrusion()
		{
			glPushMatrix();
	
			glTranslatef(x,y,z);
			//glRotatef(winkel_y,0.0,1.0,0.0);
		
			if(fortschritt>=1.0)
			{
				data[2].renderExtrusion();
				if(number_of_objects==4) data[3].renderExtrusion();
			}
			else if(fortschritt>=0.5)
			data[1].renderExtrusion();
			else data[0].renderExtrusion();

	
			glPopMatrix();
		}
		
		void prepareShadowVolume()
		{
			for(u=0;u<number_of_objects;u++)
			{
				data[u].prepareShadowVolume(winkel_y);
			}
		}
		
		int renderStatus(int i)
		{
			glPushMatrix();

			glTranslatef(x,y,z);
			glRotatef(winkel_y,0.0,1.0,0.0);

			data[i].render();
			
			drawProgress();
	
			glPopMatrix();
			
			return 0;
		}
		
		void drawProgress()
		{
			double mvmatrix[16];
			glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
		
			gluProject(0,2,0,mvmatrix,project.projmatrix,project.viewport,&progressX,&progressY,&progressZ);
			
		}
		
		bool acceptsRessourceWithID(int ID)
		{
			//cout << "searchfor=" << ID << endl;
			//for(int i=0;i<ressIDs.size();i++) cout << "AV=" << ressIDs[i] << endl;
			if(find(ressIDs.begin(),ressIDs.end(),ID)==ressIDs.end()) return false;
			return true;
		}
		
		int writeToFile(FILE *file)
		{
			fprintf(file,"BUILDING;;%f;%f;%f;;%f;;%f;;%f;%i;",x,y,z,winkel_y,fortschritt,health,myPackageName.length());
			fwrite(myPackageName.cString(),1,myPackageName.length(),file);
			fprintf(file,";;");
			
			return 0;
		}
		
		int loadFromFile(FILE *file)
		{
			char path[512];
			char name[512];
			int l=0;
			int ret = fscanf(file,"BUILDING;;%f;%f;%f;;%f;;%f;;%f;%i;",&x,&y,&z,&winkel_y,&fortschritt,&health,&l);
			fread(name,1,l,file);
			name[l]='\0';
			fscanf(file,";;");
			cout << name << " " << x << "  " << y << "  " << z << "  " << winkel_y << "  " << fortschritt << "  " << health << endl;
			myFileName = path;
			myPackageName = name;
			return ret;
		}
};