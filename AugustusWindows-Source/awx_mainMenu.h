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



#include "mainMenuUltraRender.h"


// Controlprototypes
void renderMainMenu();
void idleMainMenu();
void processEventsMainMenu();
void cleanUpMenu();


// Fontsammlung
extern vector<fonts>loadedFonts;


//// Soundsystem
//SoundSystem *mainMenuSoundSystem;
//int SoundSystemID=-1;

// Skybox
awxSkyBox *mainMenuSkyBox;

int menuTexID=-1;

#ifdef GCCDebugCompile
#warning globalen Wassershader entfernen
#endif

awx_shader *waterShader=NULL;
GLuint tex1,tex2;

#ifdef GCCDebugCompile
#warning Textinputtest
#endif


// Menu
vector<awx_button> menuButtons;

const int maxButtonCount = 4;
awx_button *blankButton;
awx_button *back;

awx_window *campaignMapWindow;

int currentCampaignIndex=-1;

int *loadingButtonStates;
int *loadingButtonStatesPress;
int currentLoadGameSelection=-1;
float blankButtonStartY = 0.56;

awx_window *theme;
bool showMenu=true;
int currentMainMenuMode=0;
float yStep=-0.07;
int scrollPos=0;
float scrollAbstand=0.02;

awx_button *scrollDown,*scrollUp;
awx_button *campaignButtons;

awx_tgaAnimation *campaignFlag;

class campaignDescriptionDataset
{
	public:
	
	string description;
	int textTTFID;
	
	float x,y;
	
	campaignDescriptionDataset()
	{
		description = "none";
		x=0;
		y=0;
	}
	
	campaignDescriptionDataset(float x, float y)
	{
		this->x=x;
		this->y=y;
	}
	
	void set(string t, float x, float y,awx_trueTypeFont &tempFont)
	{
		this->x=x;
		this->y=y;
		description = t; 
		
		
		tempFont.setFormat(false,false,false,255,255,255,"Data/fonts/arial.ttf",15,0.7,570,1,true);
		tempFont.init();
		tempFont.calculate(t);
	

		tempFont.setRenderPosition((campaignMapWindow->x+campaignMapWindow->width*0.05)*(project.width/1.33)+5,project.height- (project.height*0.07 +tempFont.getLineHeight()*3.5)+5);
		textTTFID = (int)ttfFonts.size();
		ttfFonts.push_back(tempFont);
	}
	
	void transformMark()
	{
		glTranslatef(campaignMapWindow->x+((x/1024.0)*campaignMapWindow->width),0.15 + ((1024.0-y)/1024.0)*campaignMapWindow->height,0);
	}
	
} *campaignData;

void createProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,1,2.0,8000);
	glMatrixMode(GL_MODELVIEW);
}

void enterGame()
{
	cout << "Standard-enterGame()" << endl;
	
	cleanUpMenu();
	renderLoadingScreen("Init game environment...");
	
	eventFKTPointer = awx_events;
	renderFKTPointer = awx_rendering;
	idleFKTPointer = awx_idle_func;
	
	currentWeather.fog.farClippingPlane = 180;
	
	initOpenGL();	
	
	renderLoadingScreen("Loading game content...");		
	
	currentLevel->terrain.reloadLowDetailShaders();
	currentLevel->useCoolWater=false;
	
	initGame(0,"");		
}

void enterGameFromMenuCampaign(float x, float y, int button)
{
	useAutoCheckPoints=true;
	//enterGame();
	currentMainMenuMode = 2;
}

void quitGameFromMenu()
{
	cleanUpMenu();
	exit(0);
}

// Spiel beenden
void quitGameFromMenu(float x, float y, int button)
{
	quitGameFromMenu();
}

// Spiel starten
void enterGameFromMenu(float x, float y, int button)
{
	currentGameState=AWXStandardGame;
	enterGame();
}

// In ladeMenu swappen
void loadMenu(float x, float y, int button)
{
	currentMainMenuMode = 1;
}

void gotoMainMenu(float x, float y, int button)
{
	currentMainMenuMode = 0;
}

void enterGameFromMenuAndLoadGame(int x)
{
	cout << "load(enterGameFromMenuAndLoadGame): " << x << endl;
	if(currentMainMenuMode!=1) 
	{
		cout << "Doppelladeversuch erkannt." << endl;
		return;
	}
	currentMainMenuMode = -1;
	
	cleanUpMenu();
	
	renderLoadingScreen("Init game environment...");
	
	eventFKTPointer = awx_events;
	renderFKTPointer = awx_rendering;
	idleFKTPointer = awx_idle_func;
	
	currentWeather.fog.farClippingPlane = 180;
	
	initOpenGL();	
	
	renderLoadingScreen("Loading game content...");		
	
	currentLevel->terrain.reloadLowDetailShaders();
	currentLevel->useCoolWater=false;
	
	useAutoCheckPoints=true;
	initGame(1,saveGames[x].name);		
}

void scrollUpAction(float x, float y, int button)
{
	if(scrollPos<saveGames.size()-maxButtonCount) scrollPos++;
}

void scrollDownAction(float x, float y, int button)
{
	if(scrollPos>=1) scrollPos--;
}

void postCampaignStart(int index)
{	
	cout << "Start Kampagne Nummer:" << index << endl;
	
	useAutoCheckPoints=true;
	currentGameState = AWXPlayingIntroVideo;
	
	enterGame();
}

void mouseDown0(float x, float y, int button) { postCampaignStart(0); }
				

void loadInterface()
{	
	theme = new awx_window();
	theme->width = 1.1;
	theme->height = theme->width*0.25;
	
	theme->x = project.aspect/2.0 - theme->width/2.0;
	theme->y = 0.7;
	
	theme->setTexture("Menu/augustusTheme.tga");

	float buttonHeight=0.28;
	float middle = (project.aspect-buttonHeight*2*(buttonHeight/4.8))*0.5;
	float middleInverted = (project.aspect+buttonHeight*2*(buttonHeight/4.8))*0.5;
	cout << middle << "  " << project.aspect*0.5 << endl;
	
	awx_button tempButton;
	
	// kampagne
	tempButton.setTexture("Menu/MenuKampagne_n.tga","Menu/MenuKampagne_r.tga","Menu/MenuKampagne_p.tga");
	tempButton.width = buttonHeight;
	tempButton.height = tempButton.width/4.8;
	tempButton.x = middleInverted - tempButton.width/2.0;
	tempButton.y = 0.56;
	tempButton.enabled = true;
	tempButton.hidden = false;
	tempButton.hasRollOverState = true;
	tempButton.setMouseUpAction(enterGameFromMenuCampaign);
	
	menuButtons.push_back(tempButton);
	
	// Gefecht
	tempButton.y+= yStep;
	tempButton.enabled = true;
	tempButton.setTexture("Menu/MenuGefecht_n.tga","Menu/MenuGefecht_r.tga","Menu/MenuGefecht_p.tga");
	tempButton.setMouseUpAction(enterGameFromMenu);
	
	menuButtons.push_back(tempButton);
	
	// Mehrspieler
	tempButton.y+= yStep;
	tempButton.enabled=false;
	tempButton.mdown=false;
	tempButton.setTexture("Menu/MenuMehrspieler_n.tga","Menu/MenuMehrspieler_r.tga","Menu/MenuMehrspieler_p.tga");
	
	menuButtons.push_back(tempButton);
	
	// Laden
	tempButton.y+= yStep;
	tempButton.enabled=true;
	tempButton.setTexture("Menu/MenuLaden_n.tga","Menu/MenuLaden_r.tga","Menu/MenuLaden_p.tga");
	tempButton.setMouseUpAction(loadMenu);
	
	menuButtons.push_back(tempButton);

	// beenden
	tempButton.y+= yStep;
	tempButton.enabled=true;
	tempButton.setTexture("Menu/MenuBeenden_n.tga","Menu/MenuBeenden_r.tga","Menu/MenuBeenden_p.tga");
	tempButton.setMouseUpAction(quitGameFromMenu);
	
	menuButtons.push_back(tempButton);
	
	back = new awx_button;
	back->enabled=true;
	back->hidden=false;
	back->setTexture("Menu/back_n.tga","Menu/back_r.tga","Menu/back_p.tga");
	back->setMouseUpAction(gotoMainMenu);
	back->width=tempButton.width;
	back->height=tempButton.height;
	back->x=tempButton.x;
	back->y=0.2;
	
	blankButton = new awx_button;
	blankButton->width = 0.28;
	blankButton->height = blankButton->width/4.8;
	blankButton->x = project.aspect/2.0 - blankButton->width/2.0;
	blankButton->y = 0.56;
	blankButton->setTexture("Menu/blankNormal.tga","Menu/blankRollover.tga","Menu/blankPressed.tga");
	
	
	campaignMapWindow = new awx_window;
	campaignMapWindow->width = 0.6*1.382;
	campaignMapWindow->height = 0.6;
	campaignMapWindow->x = 0.40;
	campaignMapWindow->y = 0.15;
	campaignMapWindow->setTexture("Menu/map.tga");
	

	cout << "Rendering fonts" << endl;
	
	cout << "OpenSaveGames" << endl;
	
	awx_string saveGameRoot = "Savegames/save";
	awx_string saveGameName;
	awx_string ext = ".awx";
	
	int gameIndex=0;
	FILE *file;
	awx_dictionary dict;
	saveGame tempSaveGame;
	
	awx_trueTypeFont tempFont;
	ttfFonts.clear();
	float y = project.height*0.6;
	
	cout << "Check savegames:" << endl;
	while( (file = fopen(((saveGameName = (saveGameRoot.stringByAppendingInteger(gameIndex))+ext)).cString(),"r")) != NULL)
	{
		gameIndex++;
		fclose(file);
		
		/*if(!dict.initWithFile(saveGameName.cString())) 
		{
			cout << "Fehler in SavegameFile. continue." << endl;
		}
		
		tempSaveGame.path = saveGameName;
		tempSaveGame.name = dict.getObjectForKey("name");
		cout << saveGameName << "  " << tempSaveGame.name << endl;*/
	
		
		cout << "rendering font..." << endl;
		awx_string renderText = awx_string("#b ") + awx_string("savegame").stringByAppendingInteger(gameIndex);
		
		tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,150,1,true);
		tempFont.init();
		tempFont.calculate(renderText.cString());
		tempFont.setRenderPosition(project.width*0.5-tempFont.getLineWidth(1)*0.5,y);
		tempSaveGame.fontIndex = (int)ttfFonts.size();
		ttfFonts.push_back(tempFont);

		tempSaveGame.name = saveGameName;
		saveGames.push_back(tempSaveGame);
		
		y-=tempFont.getLineHeight()*1.5;
	}
	
	loadingButtonStates = new int[maxButtonCount];
	loadingButtonStatesPress = new int[maxButtonCount];
	/*	for(int i=0;i<maxButtonCount*2;i++)
	{
		loadingButtonStates[i]=false;
	}*/
	
	scrollDown = new awx_button();
	scrollUp = new awx_button();
	
	scrollDown->setTexture("Menu/scrollDown.tga");
	scrollUp->setTexture("Menu/scrollUp.tga");

	scrollDown->x=(middleInverted+blankButton->width*0.5);
	scrollDown->width=tempButton.height;
	scrollDown->height=tempButton.height;
	scrollDown->y=0.56+yStep*(maxButtonCount-1);
	scrollDown->setMouseUpAction(scrollUpAction);
	
	scrollUp->x=(middleInverted+blankButton->width*0.5);
	scrollUp->width=tempButton.height;
	scrollUp->height=tempButton.height; 
	scrollUp->y=0.56;//-scrollDown->width*0.5;
	scrollUp->setMouseUpAction(scrollDownAction);
	
	
	campaignButtons = new awx_button[8];
	for(int i=0;i<8;i++)
	{
		campaignButtons[i].setTexture((char*)("Menu/campaignButtons/" + intToString(i+1) + string("K_n.tga")).c_str(),
									  (char*)("Menu/campaignButtons/" + intToString(i+1) + string("K_r.tga")).c_str(),
									  (char*)("Menu/campaignButtons/" + intToString(i+1) + string("K_p.tga")).c_str());
									  
		campaignButtons[i].height = 0.05;
		campaignButtons[i].width = campaignButtons[i].height*4.8f;
		campaignButtons[i].x = 0.15;
		campaignButtons[i].y = 1 - (i*(campaignButtons[i].height+0.01)+0.37);
		campaignButtons[i].setMouseDownAction(mouseDown0);
		//(i==0) ? campaignButtons[i].enabled =true : campaignButtons[i].enabled = false;
	}
	
	
	// Untertitel initialisieren
	initUntertitelObjects();
	
	mainMenuSkyBox = new awxSkyBox;
	mainMenuSkyBox->initSkyBox();
	
	
	campaignData = new campaignDescriptionDataset[8];
	campaignData[0].set("Die römischen Schiffe landen an der griechischen Küste. In sicherer Entfernung zur Stadt Methone muss ein Stützpunkt errichtet und die Befreiung von den Terroristen vorbereitet werden. ('Grafikfratz')",517,589,tempFont);
	campaignData[1].set("Dieses Kapitel ist noch nicht freigespielt.",517,589,tempFont);
	campaignData[2].set("Dieses Kapitel ist noch nicht freigespielt.",466,478,tempFont);
	campaignData[3].set("Dieses Kapitel ist noch nicht freigespielt.",491,506,tempFont);
	campaignData[4].set("Dieses Kapitel ist noch nicht freigespielt.",878,622,tempFont);
	
	campaignData[5].set("Dieses Kapitel ist noch nicht freigespielt.",832,755,tempFont);
	campaignData[6].set("Dieses Kapitel ist noch nicht freigespielt.",771,834,tempFont);
	campaignData[7].set("Dieses Kapitel ist noch nicht freigespielt.",735,792,tempFont);
	
	
	campaignFlag = new awx_tgaAnimation;
	campaignFlag->loadTGASeries("Data/interface/menu/fahne/fahne",1,20);																// Only numbers, Backspace und Return beachten
}

void mainMenuMouseOver(int x, int y)
{	
	float xi=x/(float)project.width*project.aspect;
	float yi=(project.height-y)/(float)project.height;		
		
	if(currentMainMenuMode==0)
	{
		for(int i=0;i<menuButtons.size();i++)
		{
			menuButtons[i].mouseOver(xi,yi);
		}
		
	} else if(currentMainMenuMode==1)
	{
		for(int i=scrollPos;i<(int)ttfFonts.size();i++)
		{
			if(i>=maxButtonCount+scrollPos) break;
			
			blankButton->y=blankButtonStartY+yStep*(i-scrollPos);
			blankButton->mouseOver(xi,yi);
			loadingButtonStates[i-scrollPos] = blankButton->mouseOverFlag;
			loadingButtonStatesPress[i-scrollPos] = blankButton->pushedFlag;
		}
		
		back->mouseOver(xi,yi);
		scrollUp->mouseOver(xi,yi);
		scrollDown->mouseOver(xi,yi);
	} else if(currentMainMenuMode==2)
	{
		currentCampaignIndex=-1;
		for(int i=0;i<8;i++)
		{
			campaignButtons[i].mouseOver(xi,yi);
			if(campaignButtons[i].mouseOverFlag)
			{
				currentCampaignIndex = i;
				break;
			}
		}
	}
}

void mainMenuMouseDown(int x, int y)
{
	float xi=x/(float)project.width*project.aspect;
	float yi=(project.height-y)/(float)project.height;	
			
	if(currentMainMenuMode==0)
	{
		for(int i=0;i<menuButtons.size();i++)
		{
			if(menuButtons[i-scrollPos].mouseOverFlag)
			menuButtons[i-scrollPos].mouseDown(xi,yi,1);
		}
	} else if(currentMainMenuMode==1)
	{		
		for(int i=scrollPos;i<(int)ttfFonts.size();i++)
		{
			if(i>=maxButtonCount+scrollPos) break;
			
			if(loadingButtonStates[i-scrollPos])
			{
				blankButton->y=blankButtonStartY+yStep*(i-scrollPos);
				blankButton->mouseDown(xi,yi,1);
				loadingButtonStates[i-scrollPos] = blankButton->mouseOverFlag;
				loadingButtonStatesPress[i-scrollPos] = blankButton->pushedFlag;
			}
		}
		if(back->mouseOverFlag)
			back->mouseDown(xi,yi,1);
			
		
		if(scrollUp->mouseOverFlag)
			scrollUp->mouseDown(xi,yi,1);
			
		if(scrollDown->mouseOverFlag)
			scrollDown->mouseDown(xi,yi,1);
	} else if(currentMainMenuMode==2)
	{
		campaignButtons[0].mouseDown(xi,yi,1);
	}
}

void mainMenuMouseUp(int x, int y)
{	
	float xi=x/(float)project.width*project.aspect;
	float yi=(project.height-y)/(float)project.height;
	
	if(currentMainMenuMode==0)
	{
		for(int i=0;i<menuButtons.size();i++)
		{
			if(menuButtons[i].pushedFlag)
			menuButtons[i].mouseUp(xi,yi,1);
		}
		

	} else if(currentMainMenuMode==1)
	{
		for(int i=scrollPos;i<(int)ttfFonts.size();i++)
		{
			if(i>=maxButtonCount+scrollPos) break;
			
			if(loadingButtonStatesPress[i-scrollPos])
			{
				blankButton->y=blankButtonStartY+yStep*(i-scrollPos);
				blankButton->mouseUp(xi,yi,1);
				if(blankButton->mouseOverFlag) enterGameFromMenuAndLoadGame(i-scrollPos);
				loadingButtonStates[i-scrollPos] = blankButton->mouseOverFlag;
				loadingButtonStatesPress[i-scrollPos] = blankButton->pushedFlag;
				currentLoadGameSelection=i;
			}
		}
		
		if(back->pushedFlag)
			back->mouseUp(xi,yi,1);
			
		if(scrollUp->pushedFlag)
			scrollUp->mouseUp(xi,yi,1);
			
		if(scrollDown->pushedFlag)
			scrollDown->mouseUp(xi,yi,1);
	} else if(currentMainMenuMode==2)
	{
		for(int i=0;i<8;i++)
		{
			campaignButtons[i].mouseUp(xi,yi,1);
		}
	}
}

void initOpenGLForMenu()
{
	loadingTexture = load_tgaAndReturnObject("Data/textures/loading.tga",false,false);
	
	renderLoadingScreen("loading main menu data...");

	//enterGame();
	//return;
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	//enterGame();
	//mainMenuSoundSystem = new SoundSystem(1);
	//SoundSystemID = mainMenuSoundSystem->AddMusic("Data/music/floyd.mp3");
	//mainMenuSoundSystem->PlayMusic(SoundSystemID);

	glGenTextures(1500,textur_liste);
	
	load_tga("Data/textures/sky.tga",currentID,true,true);
	menuTexID = currentID;
	currentID++;

	
	waterShader = new awx_shader();
	waterShader->set("Data/shaders/water.vert","Data/shaders/water.frag");
	waterShader->texture2 = glGetUniformLocationARB(waterShader->p,"skyTexture");
	waterShader->texture0 = glGetUniformLocationARB(waterShader->p,"diffuse");
	waterShader->texture1 = glGetUniformLocationARB(waterShader->p,"normal");
	waterShader->uniform0 = glGetUniformLocationARB(waterShader->p,"timer");

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	Uint32 levelTime=SDL_GetTicks();
	cout << endl << endl << "Loading level..." << endl;
	currentLevel = new level();			cout << "V6=" << glGetError() << endl;
	currentLevel->load_level("Data/level/griechenland/level");	
	cout << endl << "Level loaded in: " << (levelTime-SDL_GetTicks())/1000.0 << " secs." << endl << endl << endl;
	setFogOfWar(1);			
	cout << "FogOfWar Updated" << endl;
	glPopAttrib();
	
	currentLevel->watershader = waterShader;
	

	load_tga("Data/Interface/pressed.tga",currentID,false,false);
	pressedID = currentID;
	currentID++;
	
	load_tga("Data/Interface/rollover.tga",currentID,false,false);
	rollOverID = currentID;
	currentID++;


	createProjectionMatrix();
	
	startParticleSystemThread();
	
	
	currentWeather.setup();
	currentWeather.fog.farClippingPlane=200;
	
	loadInterface();



	initUltraRender();
}

float renderedMenuFrames=0;

void startInterfaceDrawMode()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
    gluOrtho2D(0.0,project.aspect,0.0,1.0);
	
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
    glLoadIdentity();
	
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
}

void endInterfaceDrawMode()
{
	glPopAttrib();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void stampVersion()
{	
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0,1.0,1.0,1.0);
	outputBitmapFont(project.aspect-0.05,0.005,"4.01");
}

inline void renderInterfaceForMenu()
{
	startInterfaceDrawMode();
	
	theme->render();
	
	
	for(int i=0;i<menuButtons.size();i++)
	{
		menuButtons[i].render();
	}
	
	stampVersion();
	
	endInterfaceDrawMode();
}

inline void renderInterfaceForLoadMenu()
{		
	startInterfaceDrawMode();
	glDepthMask(GL_FALSE);
	
	theme->render();
	
	for(int i=scrollPos;i<(int)ttfFonts.size();i++)
	{
		if(i>=maxButtonCount+scrollPos) break;
		
		blankButton->mouseOverFlag=loadingButtonStates[i-scrollPos];
		blankButton->pushedFlag=loadingButtonStatesPress[i-scrollPos];
		blankButton->y=blankButtonStartY+yStep*(i-scrollPos);
		blankButton->render();
	}
	
	back->render();
	
	scrollDown->render();
	scrollUp->render();
	
	stampVersion();
	
	endInterfaceDrawMode();

	SDL_GL_Enter2DMode();

	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glColor4f(1.0,1.0,1.0,1.0);
	float y = (1.0-(blankButtonStartY+blankButton->height*0.5))*project.height-ttfFonts[0].getLineHeight()*0.5;
	int counter=0;
	for(int i=scrollPos;i<(int)ttfFonts.size();i++)
	{
		if(counter>=maxButtonCount) break;
		ttfFonts[i].setRenderPositionY(y);
		ttfFonts[i].registerRender();
		ttfFonts[i].renderDirect();
		y-=yStep*project.height;
		counter++;
	}
	
	SDL_GL_Leave2DMode();
	
	glDepthMask(GL_TRUE);
}

inline void renderInterfaceForCampaignMenu()
{		
	startInterfaceDrawMode();
	glDepthMask(GL_FALSE);
	glDisable(GL_ALPHA_TEST);
	
	theme->render();
	
	glColor4f(1.0f,1.0f,1.0f,0.95f);

	campaignMapWindow->render();

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	
	for(int i=0;i<8;i++)
	{
		(i==0) ? campaignButtons[i].enabled=true : campaignButtons[i].enabled=false;
		campaignButtons[i].render();
	}
	
	for(int i=0;i<8;i++)
	{
		campaignButtons[i].enabled=true;
	}
	

	
	if(currentCampaignIndex>=0&&currentCampaignIndex<8) 
	{
		
		glPushMatrix();
		
		campaignData[currentCampaignIndex].transformMark();
		glScalef(0.1,0.1,1.0);
		
		campaignFlag->play(float(runtime.fps));
		campaignFlag->useCurrentTexture();
	
		if(currentCampaignIndex!=0) glColor4f(1.0,1.0,1.0,0.5);
	
		glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-0.5,0.0,0.0);
		glTexCoord2f(1.0,0.0);
		glVertex3f(0.5,0.0,0.0);
		glTexCoord2f(1.0,1.0);
		glVertex3f(0.5,1.0,0.0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(-0.5,1.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-0.5,0.0,0.0);
		glEnd();
		
		glPopMatrix();
	
		if(currentCampaignIndex!=0) glColor4f(1.0,1.0,1.0,0.5);
	}
	
	
	
	glDisable(GL_TEXTURE_2D);
	glColor4f(0.0f,0.0f,0.0f,0.4);
	glPushMatrix();
	glTranslatef(campaignMapWindow->x+campaignMapWindow->width*0.05,0.07,0.0);
	glScalef(campaignMapWindow->width*0.9,0.09,1.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(1.0,0.0,0.0);
	glVertex3f(1.0,1.0,0.0);
	glVertex3f(0.0,1.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glEnd();
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	
	stampVersion();
	
	endInterfaceDrawMode();

	SDL_GL_Enter2DMode();

	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glColor4f(1.0,1.0,1.0,1.0);


	if(currentCampaignIndex>=0&&currentCampaignIndex<8)
	{
		ttfFonts[campaignData[currentCampaignIndex].textTTFID].registerRender();
		ttfFonts[campaignData[currentCampaignIndex].textTTFID].renderDirect();
	}
	
	SDL_GL_Leave2DMode();
	
	glDepthMask(GL_TRUE);
}

float mainMenuXMin=-1;
float mainMenuXMax=1;
float mainMenuX=0, mainMenuY=0;
float mainMenuXSpeed=0,mainMenuYSpeed=0;
bool mainMenuXTrend=true;

void renderMainMenu()
{
	runtime.genFPS();

	if(renderedMenuFrames==0) initOpenGLForMenu();
	
	createProjectionMatrix();
	
	renderedMenuFrames+=0.6;
	
	if(mainMenuX<mainMenuXMin)
	{
		mainMenuXTrend=true;
	}
	if(mainMenuX>mainMenuXMax)
	{
		mainMenuXTrend=false;
	}
	
	if(mainMenuXTrend)
	{
		mainMenuXSpeed+=0.02/(float)runtime.fps;
	} else mainMenuXSpeed-=0.02/(float)runtime.fps;
	
	mainMenuX+=mainMenuXSpeed;
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	mainMenuX=0.0;
	
	if(1==2)
	{
	
		gluLookAt(170.0 - mainMenuX*0.6,50.0 - mainMenuX*0.5,80.0 + mainMenuX*0.2,
				  0.0,0.0,0.0,
				  0.0,1.0,0.0);

		glRotatef(mainMenuX*0.1,0.1,0.15,0.1);
	}
	
	else 
	{
		int i=currentLevel->findCameraDriveByIdentifier("Standard-Kamerafahrt");
		string script="";
		if(i!=-1) currentLevel->setUpMatriceWithCameraDrive(i,script);
	}

	GLfloat p[] = {0,9.0,1.0,1.0};

	//glutSolidCube(30);

	//renderUltraRender();
	
	//SDL_GL_SwapBuffers();
	//return;
	
	//glPushMatrix();
	//glRotatef(renderedMenuFrames,1.0,0.8,0.0);
	glLightfv(GL_LIGHT0,GL_POSITION,p);
	
	/*glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(renderedMenuFrames/1000.0,renderedMenuFrames/1320.0,0.0);
	glMatrixMode(GL_MODELVIEW);
	glBindTexture(GL_TEXTURE_2D,textur_liste[menuTexID]);
	glPushMatrix();
	glTranslatef(-150,-130,-100);
	glScalef(5500,1800,2000);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(1.0,0.0,-1.0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(1.0,1.0,-1.0);
	glTexCoord2f(0.0,1.0);
	glVertex3f(0.0,1.0,0.0);
	glEnd();
	glPopMatrix();
	glPopAttrib();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);*/

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	{
		GLfloat no_mat[] = {0.05,0.05,0.05,1.0};
		GLfloat diffuse[] = {1.0,1.0,1.0,1.0};
		GLfloat specular[] = {0.6,0.6,0.6,1.0};
		GLfloat shininess[] = {120.0};
		
		glMaterialfv(GL_FRONT,GL_AMBIENT,no_mat);
		glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
		glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
		glMaterialfv(GL_FRONT,GL_SHININESS,shininess);
		
		GLfloat p[] = {-40.0,80.0,100.0,1.0};
		glLightfv(GL_LIGHT0,GL_POSITION,p);
	}
	
	if(currentLevel!=NULL) currentLevel->render();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonOffset(-10.0,-1.0);
	glDepthMask(GL_FALSE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glUseProgramObjectARB(NULL);
	glDisable(GL_LIGHTING);
	glColor4f(1.0,1.0,1.0,1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	currentLevel->renderBlobsAllUnperformant();
	glPopAttrib();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	{
		GLfloat ambient[] = {0.5,0.5,0.5,1.0};
		glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	
		GLfloat p[] = {-40.0,1580.0,100.0,1.0};
		glLightfv(GL_LIGHT0,GL_POSITION,p);
	}
	
	currentLevel->renderLevelObjectsOptimized(false);
	glPopAttrib();
	
	mainMenuSkyBox->render();	
	
	//currentWeather.render();
	
	//renderUntertitelDisplay();
	
	SDL_GL_Enter2DMode();

	glColor4f(1.0,1.0,1.0,1.0);
	for(int i=0;i<(int)ttfFonts.size();i++)
	{
		ttfFonts[i].renderDirect();
	}
	
	SDL_GL_Leave2DMode();
	
	
	if(showMenu)
	{
		if(currentMainMenuMode==0)
			renderInterfaceForMenu();
		else if(currentMainMenuMode==1)
			renderInterfaceForLoadMenu();
		else if(currentMainMenuMode==2)
			renderInterfaceForCampaignMenu();
	}
	SDL_GL_SwapBuffers();
}

void idleMainMenu()
{
	//cout << "doing crazy idle stuff" << endl;
}

void cleanUpMenu()
{
	cout << "------------ Deallocating Main Menu ------------" << endl;
	
	renderLoadingScreen("Deallocating...");

	//delete mainMenuSoundSystem; 
	
	
	glDeleteTextures(1,&textur_liste[menuTexID]);
	
	for(int i=0;i<menuButtons.size();i++)
	{
		menuButtons[i].dealloc();
	}
	
	menuButtons.clear();
	
	for(int i=0;i<8;i++)
	{
		campaignButtons[i].dealloc();
	}
	delete[] campaignButtons;
	
	delete[] campaignData;
	
	for(int i=0;i<(int)ttfFonts.size();i++)
	{
		ttfFonts[i].cleanup();
	}
	ttfFonts.clear();
	
	campaignFlag->dealloc();
	delete campaignFlag;
	

	blankButton->dealloc();
	delete blankButton;
	
	back->dealloc();
	delete back;
	
	cout << "deallocating font collection" << endl;
	loadedFonts.clear();
	
	theme->dealloc();
	delete theme;
	
	campaignMapWindow->dealloc();
	delete campaignMapWindow;
	
	delete[] loadingButtonStates;
	delete[] loadingButtonStatesPress;
	
	delete 	mainMenuSkyBox;
	
	//#warning killThread
	/*killParticleSystemThread();
	delete particleSystemHandler;*/
	
	scrollDown->dealloc();
	scrollUp->dealloc();
	delete scrollDown;
	delete scrollUp;
	
	
	#ifdef GCCDebugCompile
	#warning Textinputtest
	#endif
	

	deallocUltraRender();
	
	cout << endl << endl << endl;
}

void processEventsMainMenu()
{
	if(currentGameState!=AWXInMainMenu)
	{
		return;
	}

	SDL_Event event;

    while(SDL_PollEvent(&event)) 
	{
		SDLKey k= event.key.keysym.sym;
		
		switch( event.type ) 
		{
			case SDL_KEYDOWN:
				
				if(k==SDLK_g)
				{
					if(currentMode==AWXMenu)
					{
						enterGame();
						currentMode=AWXGame;
					}
				} else if(k==SDLK_ESCAPE)
				{
					if(currentMainMenuMode==0)
					{
						cleanUpMenu();
						exit(0);
					} else if(currentMainMenuMode==1)
					{
						currentMainMenuMode=0;
					} else if(currentMainMenuMode==2)
					{
						currentMainMenuMode=0;
					}
				} else if(k==SDLK_s)
				{
					showMenu=!showMenu;
				} else if(k==SDLK_UP)
				{
					if(currentMainMenuMode==1&&scrollPos>=1)
					scrollPos--;
				} else if(k==SDLK_DOWN)
				{
					if(currentMainMenuMode==1&&scrollPos<maxButtonCount)
					scrollPos++;
				}
				
				break;
				
			case SDL_KEYUP:
			
				break;
			
			case SDL_QUIT:
				quitGameFromMenu();
				
				
				break;
				
			case SDL_MOUSEMOTION:
			
				mainMenuMouseOver(event.motion.x,event.motion.y);
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				
				mainMenuMouseDown(event.button.x,event.button.y);
				if(currentMainMenuMode==-1) return;
				break;
			
			case SDL_MOUSEBUTTONUP:
			
				mainMenuMouseUp(event.button.x,event.button.y);
				
				if(currentMainMenuMode==-1) return;
				break;
						
        }
    }
}