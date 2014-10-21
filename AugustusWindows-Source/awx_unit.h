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

#include "awx_unit_ego.h"
#include "awx_unit_utilities.h"

class awx_player;
class awx_unit;
class awx_building;

vector <awx_player> player;


/*vector<awx_building> buildings;
vector<awx_unit> units;*/

typedef struct{
	int playerID;
	int unitID;
}opponentUnit;

/*class fuzzyLogicValueContainer
{
	public:
	
	float muedigkeit;
	float baugeschwindigkeit;
	
	fuzzyLogicValueContainer()
	{
		muedigkeit=0;
		baugeschwindigkeit = 1.0;
	}
};*/

class awx_beruf;

class awx_unit: public aax, public awx_blob, public visibilityCheck
{
	public:
	
	int playerIndex;
	
	vector<awx_vector3f> target;	// Derzeit anvisierte position
	bool ergebnisflag;				// AufTerrainFlag
	awx_vector3f richtungsvektor;	// Derzeitiger richtungsvektor (wenn anvisiert==true)
	awx_vector3f vec;
	bool anvisiert;
	bool hasDir;
	float sichtweite;
	
	vector <awx_button> buttons;
	vector <awx_string> namen;
	vector <awx_string> hausnamen;
	vector <awx_string> descriptions;
	
	int descriptionIndex;
	int myButtonID;
	
	int formation;
	
	#ifdef GCCDebugCompile
	#warning Blut entfernt
	#endif
	//awx_particleSystem blood;
	
	int tempflag;
	
	// Physikalische Gr√∂√üen
	float t;						// 1/fps
	
	awx_unit_values unitValues;
	awx_unit_mode mode;
	
	float fortschritt;
	
	bool attacking;
	int attackPlayer;
	int attackIndex;
	bool attacksth;
	bool wildflag;
	
	bool bringStuffHome;		// Werden rohstoffe zum haus gezerrt?
	
	bool updateFogOfWarFlag;
	
	vector <opponentUnit> attacked;
	
	int status;
	
	
	int myFirstTextFieldID;
	
	int myIndex;
	
	awx_vector3f futurePoint;
	
	awx_vector3f bildschirmkoordinaten;
	int workingOnBuildingID;
	int workingOnBuildingIDBefore;
	
	int myCurrentRessourceID;
	
	awx_vector2f myKopfPositionInWindowCoords;
	
	int currentSektorX,currentSektorZ;		// Stores currentSektor identifier
	int hitTriangle;
	
	
	bool rohstoffabbau;
	bool rohstoffabbauIndirect;
	awx_vector3f oldabbaustatte;
	int myRessID;
	bool findNextTree; // Flag ob bei ankunft an haus überprüft werden soll ob baum noch näherster baum ist
	
	CEgoController egoController;
	
	float loadedCurrentFrame;
	bool isWorking;
	
	bool buerger;
	awx_beruf* beruf;
	bool working;
	awx_vector3f oldPosition;
	awx_vector3f eintritt;
	bool invisible;
	bool inFirma;
	
	bool pennt;
	bool inWohnung;
	
	awx_building* brunnen;
	bool drinking;
	bool inBrunnen;
	
	awx_building* wirtshaus;
	bool eating;
	bool inWirtshaus;
	
	float rand2;
	float rand3;
	float rand4;
	
	

	// KI
	fuzzyLogicValueContainer fuzzyLogicValues;
	
	awx_vector3f lastPositionNotNAN;
	float lastModelviewNotNAN[16];
	
	awx_string meinName;

	awx_unit()
	{	
		pennt = false;
		inWohnung = false;
		inFirma = false;
		invisible = false;
		formation = AWX_NONE;
		wildflag = false;
		status = AWX_AGGRESSIVE;
		
		bringStuffHome=false;
		
		//abbauspeed = 14;
		myRessID=-1;
		findNextTree=false;
		
		updateFogOfWarFlag=false;
		
		aax::aax();
		awx_blob::awx_blob();
		
		isWorking=false;
	
		anvisiert = false;
		
		modelview[0] = 1;	
		modelview[5] = 1;
		modelview[10] = 1;
		modelview[15] = 1;
		
		for(int i=0;i<16;i++) lastModelviewNotNAN[i] = 0.0;
		
		myCurrentRessourceID = -1;
	
		
		descriptionIndex = -1;
		
		fortschritt = 0;
		
		playerIndex = 0;
		attacksth = false;
		attacking = false;
		attackIndex = 0;
		attackPlayer = 0;
		
		workingOnBuildingID=-1;
		myCurrentRessourceID=-1;
		rohstoffabbau = false;
		rohstoffabbauIndirect = false;
		
		buerger = false;
		beruf = NULL;
		working = false;
		
		
		brunnen = NULL;
		drinking = false;
		inBrunnen = false;
	
		wirtshaus = NULL;
		eating = false;
		inWirtshaus = false;
		
		rand2 = RANDOM_FLOAT*2 -1;
		rand3 = RANDOM_FLOAT*2 -1;
		rand4 = RANDOM_FLOAT*2 -1;
		
		meinName = "notSet";
	}
	 
	bool proveProducts();
	void reduceRohstoffe();
	
	void buildBuilding(awx_vector3f position, bool computerControlled=false, int buildUnits=-1);
	void placeBuilding(char* name, bool computerControlled=false);
	void attack(int playerID , int selectionID);
	void idle();
	void setTarget(awx_vector3f ziel, int indexOfSecondarySelectionAI=-1);
	void setTargetFix(awx_vector3f fix);
	bool rohstoffCheck(int playerIndex,int index);
	void regnerateCorrectPath();
	
	void showInfo(int index){
		if(index >= 0){ 
			awx_show_info(myFirstTextFieldID+descriptionIndex);
		}
	}
	
		void renderButtons()
		{
				glPushMatrix();
					for(int i = 0; i < (int)buttons.size();i++){
						buttons[i].render();
					}
					showInfo(descriptionIndex);
					
					awx_progress progress;
					progress.texID = progressTexID;
					progress.x = 0.4;
					progress.y = 0.07;
					progress.width = 0.2;
					progress.setProgress(fuzzyLogicValues.muedigkeit);
					progress.render();
					
					if(workingOnBuildingID!=-1)
					{
						progress.x = 0.4;
						progress.y = 0.1;
						progress.width = 0.2;
						progress.setProgress(fuzzyLogicValues.baugeschwindigkeit);
						progress.render();
					}
					
					else if(rohstoffabbau || rohstoffabbauIndirect)
					{
						progress.x = 0.4;
						progress.y = 0.1;
						progress.width = 0.2;
						progress.setProgress(fuzzyLogicValues.baugeschwindigkeit);
						progress.render();
						
						progress.x = 0.4;
						progress.y = 0.13;
						progress.width = 0.2;
						progress.setProgress(unitValues.getrageneRohstoffe/float(unitValues.rucksackgroesse));
						progress.render();
					}
					
				glPopMatrix();
		}
	
		int openFile(const char* filename){
			awx_string pfad = filename;
			pfad = pfad.stringByDeletingLastPathComponent();
			meinName = pfad.lastPathComponent();
			workingdirectory = pfad;
			pfad = pfad.stringByAppendingPathComponent(*(new awx_string("config.att")));
			
			if(!attributes.initWithFile(pfad.cString())){
				cout << ".att Datei Konnte nicht gefunden werden!" << endl;
			}
			
			//Attribute setzen:
				unitValues.setDictionary(attributes);
				if(!proveProducts())return -1;
				reduceRohstoffe();
				/*bauzeit = attributes.getObjectForKey("Bauzeit").floatValue();
				v = attributes.getObjectForKey("Geschwindigkeit").floatValue();
				health_max = attributes.getObjectForKey("Lebenspunkte").floatValue();
				health = health_max;
				schaden_min = attributes.getObjectForKey("Schaden_min").floatValue();
				schaden_max = attributes.getObjectForKey("Schaden_max").floatValue();
				reichweite = attributes.getObjectForKey("Reichweite").floatValue();
				armor = attributes.getObjectForKey("Ruestung").floatValue();
				angriffswert = attributes.getObjectForKey("Angriffswert").floatValue();
				sichtweite = attributes.getObjectForKey("Sichtweite").floatValue(); */
				
			awx_string temp = attributes.getObjectForKey("buildings");
			
			temp.split(',' , namen);
								
			awx_button tempButton;
			
			tempButton.hasPushedState = true;
			tempButton.hasRollOverState = true;
	
			tempButton.width = 0.0457;
			tempButton.height = 0.0457;
			
			tempButton.x = 0.0175 * 1.33;
			tempButton.y = 0.05;
			
			myFirstTextFieldID = (int)ttfFonts.size();

			for(int i = 0; i < (int)namen.size();i++){
				if(namen[i].length()<1)break;
				
				awx_dictionary dict;
				dict.initWithFile(("Data/buildings/" + namen[i] + "/config.att").cString());
				awx_string temp2 = dict.getObjectForKey("Beschreibung");
				descriptions.push_back(temp2);
				hausnamen.push_back(dict.getObjectForKey("Name"));
				
				char tempString[255];
				sprintf(tempString,"#b %s\n\n%s",dict.getObjectForKey("Name").cString(),dict.getObjectForKey("Beschreibung").cString());
		
				
				awx_trueTypeFont tempFont;
				tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,1,300,1,true);
				tempFont.init();
				tempFont.caption = tempString;
				tempFont.calculate();
				ttfFonts.push_back(tempFont);
				
				load_tga(("Data/buildings/" + namen[i] + "/button.tga").cString(),currentID,false,true);
				tempButton.setTexture(currentID);
				currentID++;
				
				buttons.push_back(tempButton);
				
				tempButton.y+=tempButton.height + 0.0053333333333;
				
				if((i+1)%3==0)
				{
					tempButton.y=0.05;
					tempButton.x+=tempButton.width + 0.0053333333333;
				}
			}
			return aax::openFile(filename);
		}
		
	void reinit(awx_unit &wild){
		number_of_objects = wild.number_of_objects;
		data = new apx[wild.number_of_objects];
		attributes = wild.attributes;
		
		
		//Attribute kopieren:
			unitValues = awx_unit_values(wild.unitValues);
			if(!proveProducts())return;
			reduceRohstoffe();
			//abbauspeed = wild.abbauspeed;
			
		myFirstTextFieldID = wild.myFirstTextFieldID;
			
		
		for(int i=0;i<wild.number_of_objects;i++)
		{
			data[i] = wild.data[i];
			data[i].current_frame=0;
		}
		for(int i = 0; i < (int)wild.namen.size();i++){
			if(wild.namen[i].length()<=0) continue;
			namen.push_back(wild.namen[i]);
			if(wild.descriptions.size()>=i) descriptions.push_back(wild.descriptions[i]);
			if(wild.hausnamen.size()>=i) hausnamen.push_back(wild.hausnamen[i]);
		}
		
		meinName = wild.meinName;
	}
	
	bool mouseDown(int xi,int yi)
	{
			float x = (xi/float(project.width))*project.aspect;
			float y = (project.height - yi)/float(project.height);
			
			for(int i = 0; i < (int)buttons.size();i++)
			{
				if(x  >= buttons[i].x &&
					   y  >= buttons[i].y &&
				       x <= buttons[i].x + buttons[i].width &&
				       y  <= buttons[i].y + buttons[i].height)
				{
					placeBuilding(namen[i].cString());
					
					return true;
				}
			}
			return false;
	}
	
	void mouseOver(int xi , int yi){
			float x = (xi/float(project.width))*project.aspect;
			float y = (project.height - yi)/float(project.height);
		
			for(int i = 0; i < (int)buttons.size();i++)
			{
				buttons[i].mouseOver(x , y);
				if(x  >= buttons[i].x &&
					   y  >= buttons[i].y &&
				       x <= buttons[i].x + buttons[i].width &&
				       y  <= buttons[i].y + buttons[i].height)
				{
					descriptionIndex = i;
					return;
				}
			}
			
			descriptionIndex = -1;
	}
	
	void mouseUp(int xi , int yi)
	{
		float x = (xi/float(project.width))*project.aspect;
		float y = (project.height - yi)/float(project.height);
		
		for(int i = 0; i < (int)buttons.size();i++){
			buttons[i].mouseUp(x,y , 1);
		}
	}
		
	bool isAbleToBuild(awx_string name){
		for(int i = 0; i < (int)namen.size();i++){
			if(strcmp(namen[i].cString() , name.cString())==0)return true;
		}
		return false;
	}
	
	void projectMe()
	{
		GLdouble mvmatrix[16];
		GLdouble winx,winy,winz;
	
		glPushMatrix();
		glTranslatef(position.x,position.y,position.z);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
		
		gluProject(0,0,0,mvmatrix,project.projmatrix,project.viewport,&winx,&winy,&winz);
		glPopMatrix();
		
		bildschirmkoordinaten = awx_vector3f(winx,winy,winz);
	}
	
	void myWindowHeadPosition()
	{
		GLdouble mvmatrix[16];
		GLdouble winx,winy,winz;
	
		glPushMatrix();
		glTranslatef(position.x,position.y+2,position.z);
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
		
		gluProject(0,0,0,mvmatrix,project.projmatrix,project.viewport,&winx,&winy,&winz);
		glPopMatrix();
		
		myKopfPositionInWindowCoords = awx_vector2f(winx,winy);
	}

	void initPosition()
	{
		if(currentLevel==NULL) return;
		currentLevel->terrain.hit(position , position.y , ergebnisflag, currentSektorX, currentSektorZ);
	}
	
	int render()
	{
		if(invisible)return -1;
		if(fortschritt < 100)return -1;
		
		rendered_objects++;

		glPushMatrix();

		glMultMatrixf(modelview);

		for(u=0;u<number_of_objects;u++)
		{
		
			data[u].render(); 
		}
	
		glPopMatrix();
	
		return 0;
	}
	
	int playAll()
	{
		if(invisible)return -1;
		if(fortschritt < 100)return -1;
		
		if(!isVisible()&&isCheckEnabled()) 
		{
			return -1;
		}
		
		rendered_objects++;

		glPushMatrix();

		glMultMatrixf(modelview);

		for(u=0;u<number_of_objects;u++)
		{
			data[u].play();
		}
	
		glPopMatrix();
		
		return 0;
	}
	
	void renderExtrusion()
	{
		glPushMatrix();
		glMultMatrixf(modelview);
		for(u=0;u<number_of_objects;u++)
		{
			//data[u].renderExtrusion();
			glCallList(74);
		}
		
		glPopMatrix();
	}
	
	void checkVisibility()
	{
		visibilityCheck::checkVisibility(modelview[12],modelview[13],modelview[14]);
	}
	
	float calcForce()
	{
		if(fortschritt<100) return 0;
		
		return unitValues.calcForce();
	}
	
	void setEgoModeCamera()
	{
		awx_matrix rotMatrix;
		glLoadIdentity(); 
		
		glPushMatrix();
		glLoadMatrixf(modelview);
		glRotatef(-float(egoController.rot_y)/2.0,0,1,0);
		glRotatef(-float(egoController.rot_x)/2.1,0,0,0);
		float modelviewIntern[] = {1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,1.0};
		glGetFloatv(GL_MODELVIEW_MATRIX,modelviewIntern);
		
		rotMatrix.setMatrix(modelviewIntern);
		glPopMatrix();
		

		awx_vector3f standardPos = awx_vector3f(0,6,-10)*egoController.zoomFactor;
		standardPos = rotMatrix.vectorProduct(standardPos);
		awx_vector3f standardDir = awx_vector3f(0,2,1);
		standardDir = rotMatrix.vectorProduct(standardDir);
		
		
		gluLookAt(position.x+standardPos.x,position.y+standardPos.y,position.z+standardPos.z,
				  position.x+standardDir.x,position.y+standardDir.y,position.z+standardDir.z,
				  0,1,0);
		
		
		currentLevel->checkVisibilityAll();
		//glRotatef(float(egoController.rot_y)/5.0,0,1,0);
		
	}
	
	void doEgoControlls(SDL_Event &event)
	{
		egoController.poolEvents(event);
	}
	
	void doEgoControllsIdle()
	{
		egoController.idle();
	
		if(egoController.forward)
		{
			position.z-=0.1;
		}
		else if(egoController.backwards)
		{
			position.z+=0.1;
		}
		
		if(egoController.right)
		{
			position.x+=0.1;
		}
		else if(egoController.left)
		{
			position.x-=0.1;
		}
		
	}
	
	int findNextRessourceObject();
	int findNextHouse();
	int findRessourceWithID(int ID);
	void mineLevelPart(int ID);
	
	int writeToFile(FILE *file)
	{
		fprintf(file,"UNIT;;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;",modelview[0],modelview[1],modelview[2],modelview[3],modelview[4],modelview[5],modelview[6],modelview[7],modelview[8],modelview[9],modelview[10],modelview[11],modelview[12],modelview[13],modelview[14],modelview[15]);
		fprintf(file,";%f;;%i;;%f",unitValues.leben,workingdirectory.length(),data[0].current_frame);
		fwrite(workingdirectory.cString(),1,workingdirectory.length(),file);
		fprintf(file,";;");
		
		return 0;
	}
		
	int loadFromFile(FILE *file)
	{
		char name[512];
		int l=0;
		int ret = fscanf(file,"UNIT;;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;",&modelview[0],&modelview[1],&modelview[2],&modelview[3],&modelview[4],&modelview[5],&modelview[6],&modelview[7],&modelview[8],&modelview[9],&modelview[10],&modelview[11],&modelview[12],&modelview[13],&modelview[14],&modelview[15]);
		ret |= fscanf(file,";%f;;%i;;%f",&unitValues.leben,&l,&loadedCurrentFrame);
		fread(name,1,l,file);
		name[l]='\0';
		fscanf(file,";;");
		workingdirectory = name;		
		return ret;
	}
};