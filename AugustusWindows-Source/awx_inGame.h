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


void setUpProjectiveMatrix(int w, int h)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, (GLfloat) w/(GLfloat) h, 10.0f, 1000.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Funktion zum wiederaufsetzen des viewports und der Projektionsmatrix nach Skalierung
void awx_reshape(int w, int h)
{

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   setUpProjectiveMatrix(w,h);
   project.width=w;
   project.height=h;
   project.aspect = (GLfloat) project.width/(GLfloat) project.height;
   
   project.viewport[0]=0;
   project.viewport[1]=0;
   project.viewport[2]=(int) w;
   project.viewport[3]=(int) h;

   glGetDoublev (GL_PROJECTION_MATRIX, project.projmatrix);
}

void setUpCamera()
{	
	awx_reshape(project.width,project.height);
	
	rendered_objects=0;
	
	if(!controls.egoMode)
	{

		glLoadIdentity();
	
		glPushMatrix();
			glRotatef(camera.calcRotAngle(controls.drag_dist_x)+camera.winkel_x,0.0,1.0,0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX,camera.rotMatrix);
		glPopMatrix();
			  
		camera.setCamera();
		
		glTranslatef(camera.direction.getX(),camera.direction.getY(),camera.direction.getZ());	  
		
			
		float rot_y = camera.calcRotAngle(controls.drag_dist_y)+camera.winkel_y;
		float rot_x = camera.calcRotAngle(controls.drag_dist_x)+camera.winkel_x;
	
		if(rot_y > 45){
			rot_y = 45;
			camera.winkel_y = 45;
		}
		else if(rot_y < 15){
			rot_y = 15;
			camera.winkel_y = 15;
		}

		//rot_x+=90;
		glRotatef(rot_y,1,0,0);
		glRotatef(rot_x,0.0,1.0,0.0);
		glTranslatef(-camera.direction.getX(),-camera.direction.getY(),-camera.direction.getZ());
		
	
	}
	else 
	{
		player[controlIndex].units[controls.egoModeUnitIndex].setEgoModeCamera();
	}
	
	{
		float modelview[16];
		glGetFloatv(GL_MODELVIEW_MATRIX,modelview);
		for(int i=0;i<16;i++) 
		{
			mvmatrix[i]=modelview[i];
			project.mvmatrix[i]=modelview[i];
		}
		awx_matrix modelviewMatrix(modelview);
		awx_vector3f pos(modelview[12]-0.01,modelview[13],modelview[14]);
		awx_vector3f forward(0.0f,0.0f,1.0f);
		awx_vector3f dir;
		
		modelviewMatrix = modelviewMatrix.transpose();
		
		pos = modelviewMatrix.vectorProduct(pos);
		dir = modelviewMatrix.vectorProduct(forward);
		pos = pos.product(-1.0f);
		
		camera.transformedCameraWorldSpace = pos;
		camera.transformedForwardVectorWorldSpace = dir;
		
		modelview[12]=0.0f;
		modelview[13]=0.0f;
		modelview[14]=0.0f;
		
		for(int i=0;i<16;i++)
		{
			camera.rotationMatrixForSpecialEffects[i] = modelview[i];
		}
	
	}
}

void renderGamePlayElements()
{
	if(controls.currentDragID!=-1)
	{
		glDisable(GL_ALPHA_TEST);
		glPushMatrix();
		glTranslatef(wx,wy,wz);
		glRotatef(player[controlIndex].buildings[player[controlIndex].buildings.size() -1].winkel_y,0.0,1.0,0.0);
		//player[controls.currentDragPlayer].buildings[controls.currentDragID].renderBlank();
		player[controlIndex].buildings[player[controlIndex].buildings.size() -1].renderBlank();
		glPopMatrix();
	}
	
	if(controls.currentLevelPartDragName!=NULL)
	{
		//currentLevel->addLevelPartFromBuilding(controls.currentLevelPartDragName);
		glPushMatrix();
		glTranslatef(wx,wy,wz);
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glCallList(currentLevel->apxPool[currentLevel->feld].stateChangeList);
		glCallList(currentLevel->apxPool[currentLevel->feld].IDs[0]);
		glPopAttrib();
		glPopMatrix();
	}

	// Fahne zeichnen
	if(controls.selection.size()==1&&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].buildings.size()+(int)player[controlIndex].units.size())
	{
		if(controls.draggingFlag)
		{
			player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].fahne = awx_vector3f(wx,wy,wz);
		}
		
		if(player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].fahne.x!=AWXNull) 
		{
			glPushMatrix();
			glTranslatef(player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].fahne.x,player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].fahne.y,player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].fahne.z);
			glRotatef(player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].winkel_y + 90 , 0 , 1 , 0);
			fahne_apx.play();
			glPopMatrix();
		}
	}
	
	controls.secondarySelection = -1;
	// Wasser und unabbauabfangen
	for(int i=1;i<currentLevel->number_of_objects;i++)
	{
		if(currentLevel->data[i].myHitTriangle!=NULL&&currentLevel->data[i].myHitTriangle->triangleSeen)
		{
			if(wx>currentLevel->data[i].xmin+currentLevel->data[i].x&&wx<currentLevel->data[i].xmax+currentLevel->data[i].x&&wz>currentLevel->data[i].zmin+currentLevel->data[i].z&&wz<currentLevel->data[i].z+currentLevel->data[i].zmax)
			{
				//currentLevel->data[i].renderWithoutTest();
				controls.secondarySelection = i; 
				break;
			}
		}
	}
	



	glDisable(GL_ALPHA_TEST);

	if(controls.auswahlRechteck)
	{
		for(int i=0;i<(int)player[controlIndex].units.size();i++) player[controlIndex].units[i].projectMe();
	}
}

void awx_events(void)
{
	SDL_Event event;

    while(SDL_PollEvent(&event)) 
	{
		if(controls.egoMode) player[controlIndex].units[controls.egoModeUnitIndex].doEgoControlls(event);
        switch( event.type ) 
		{
			case SDL_KEYDOWN:
			
				awx_keyboard_down(event.key.keysym.sym,0,0);
				
				break;
				
			case SDL_KEYUP:
			
				awx_keyboard_up(event.key.keysym.sym,0,0);
				break;
			
			case SDL_QUIT:
				awx_shutdown(0);
				break;
				
			case SDL_MOUSEMOTION:
			
				if(controls.mouse_down)
				{
					awx_motion_mouse(event.motion.x,event.motion.y);
				}
				else awx_passive_mouse(event.motion.x,event.motion.y);
			
				
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				awx_mouse(event.button.button,event.button.state,event.button.x,event.button.y);
				
				break;
			
			case SDL_MOUSEBUTTONUP:
				awx_mouse(event.button.button,event.button.state,event.button.x,event.button.y);
				break;
        }
    }
}

void kiGegnerNachdenkProcedure()
{
	if(!useAIEnemyTo) return;

	for(int i=0;i<(int)player.size();i++)
	{
		player[i].kiNachdenk();
	}
}

void idleEnvironmentAndAI()
{
	//cout << "update AI" << endl;

	float f = RANDOM_FLOAT;
	
	if(!currentWeather.fog.simulation && f > 0.998)currentWeather.fog.startSimulation();
	else 
	{
		f = RANDOM_FLOAT;
		//cout << f << endl; //0.9985
		/*if(f>0.9984)
		{
			if(player[0].buildings[0].houseFire==NULL)
			{
				player[0].buildings[0].startFire();
			}
		}*/
	}
	
	for(int i=0;i<(int)player.size();i++)
	{
		player[i].processKI();
	}
	
	for(int i=0;i<player.size();i++)
	{
		for(int u=0;u<player[i].buildings.size();u++)
		{
			if(player[i].buildings[u].health<=0)
			{
				cout << "building died" << endl;
				player[i].buildings[u].eraseQue();
				player[i].buildings.erase( player[i].buildings.begin()+u);
			}
		}
	}
}

void awx_idle_func()
{
	if(controls.egoMode) player[controlIndex].units[controls.egoModeUnitIndex].doEgoControllsIdle();
	awx_mouse_idle();


	if(controls.up_flag)
	{
		checkVisibilities();
		camera.position = (((camera.position.substract(camera.direction)).normalize()).product(-50.0/float(runtime.fps))).addition(camera.position);
	}
	else if(controls.down_flag)
	{
		checkVisibilities();
		camera.position = (((camera.position.substract(camera.direction)).normalize()).product(50.0/float(runtime.fps))).addition(camera.position);
	}
	               
	if(controls.middle)
	{
	}
}

void loadGameFile(awx_string filename);

void initGame(int mode, awx_string saveGameName)
{	
	initBerufe();
	renderLoadingScreen("Init shaders...");

	cout << "======================" << endl << "Init Game Content..." << endl << endl << endl;
	Uint32 start = SDL_GetTicks();
	
	terrainshader.set("Data/shaders/terrain4.vert","Data/shaders/terrain4.frag");
	AWXTexture0 = glGetUniformLocationARB(terrainshader.p,"texture0");
	AWXTexture1 = glGetUniformLocationARB(terrainshader.p,"texture1");
	AWXTexture2 = glGetUniformLocationARB(terrainshader.p,"texture2");
	AWXTexture3 = glGetUniformLocationARB(terrainshader.p,"texture3");
	AWXAlphaAttrib = glGetAttribLocationARB(terrainshader.p,"alphaData");
	AWXFogOfWarAttrib = glGetAttribLocationARB(terrainshader.p,"fogOfWar");
	cout << "Shaders loaded in " << (SDL_GetTicks()-start)/1000.0 << " secs." << endl << endl;
	
	Uint32 kartenSetup = SDL_GetTicks();
	
	renderLoadingScreen("Loading game content...");
	load_tga("Data/textures/selection_circle.tga",currentID,true,true);
	selectedID = currentID;
	currentID++;
	
	pressedTextFieldID = load_tgaAndReturnObject("Data/interface/pressed.tga",false,false);
	
	load_tga("Data/interface/tickbox_off.tga",currentID,false,false);
	buergerButtonID = currentID;
	currentID++;
	
	load_tga("Data/interface/tickbox_on.tga",currentID,false,false);
	buergerHereButtonID = currentID;
	currentID++;
	//buergerButtonID = load_tgaAndReturnObject("Data/interface/tickbox_on.tga",false,false);
//	currentID++;
	
	load_tga("Data/textures/selection_quad.tga",currentID,true,true);
	selectedQuadID = currentID;
	currentID++;
	
	glPushAttrib(GL_LIGHTING);
	glEnable(GL_LIGHTING);
	
	GLfloat no_mat[] = {0.05,0.05,0.05,1.0};
	GLfloat diffuse[] = {1.0,1.0,1.0,1.0};
	GLfloat specular[] = {1.0,1.0,1.0,1.0};
	GLfloat shininess[] = {15.0};
		
	glMaterialfv(GL_FRONT,GL_AMBIENT,no_mat);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,shininess);
	
	
	renderLoadingScreen("Prerendering content...");
	kartentextur = renderTop(awx_vector3f(0,150,0),awx_vector3f(0,0,0));
	//map.texID=kartentextur;
	cout << "Kartentextur liegt auf:" << kartentextur << endl;
	currentID++;
	
	cout << "Kartensetup and lighting in " << (SDL_GetTicks()-kartenSetup)/1000.0 << " secs." << endl;
	
	renderLoadingScreen("Init FogOfWar...");
	cout << "Fog of War setup.." << endl;
	currentLevel->terrain.fillFogOfWar(fowDarkValue);

	currentLevel->terrain.darkenEdge();
	
	
	renderLoadingScreen("Init game objects...");
	fahne_apx.load_apx("Data/doodads/flag/part0.apx");
	fahne_apx.fillVRAM(awx_vector3f(0,0,0) , false , 0.0, 0);
	initRohstoffe();
	
	awx_building haupthaus;
	
	if(mode==0)
	{
		Uint32 startBuildings = SDL_GetTicks();
	
		awx_player temp_player;
		player.push_back(temp_player);

		vector <awx_string> filenames;
		subdirs("Data/buildings/",filenames);
		
		for(int i = 0; i < filenames.size();i++)
		{
			awx_string name = filenames[i].deleteAllAppearances(" ");
			awx_building tempb;
			tempb.openFile(("Data/buildings/" + name + "/zusammenbau.aax").cString());
			
			loadedBuildings.push_back(tempb);
		}
		
		
		filenames.clear();
		subdirs("Data/units/",filenames);
		
		for(int i = 0; i < filenames.size();i++)
		{
			awx_string name = filenames[i].deleteAllAppearances(" ");
			cout << i << ": " << name << endl;
			awx_unit tempu;
			tempu.openFile(("Data/units/" + name + "/zusammenbau.aax").cString());
			tempu.loadBlobs("Data/textures/blob.tga",1.2);
			
			loadedUnits.push_back(tempu);
		}
		//vector <awx_building> loadedBuildings;

		renderLoadingScreen("Generating players...");
		awx_player gegner;
		player.push_back(gegner);
		cout << "Gegner pushed." << endl << endl;
		
		currentLevel->getStartpointForPlayer("player1",player[1].startpunkt);
		currentLevel->getStartpointForPlayer("player0",player[0].startpunkt);
		
		int index = getLoadedIndex("towncenter");
		if(index != -1)haupthaus.reinit(loadedBuildings[index]);
		haupthaus.x=-134;
		haupthaus.y=-6.4;
		haupthaus.z=76.14;
		haupthaus.winkel_y=180;
		haupthaus.rot_y=180;
		awx_vector3f senseless = awx_vector3f(0,0,0);
		haupthaus.x = player[0].startpunkt.x;
		haupthaus.y = player[0].startpunkt.y;
		haupthaus.z = player[0].startpunkt.z;
		//haupthaus.openFile("Data/buildings/towncenter/zusammenbau.aax");
		haupthaus.playerIndex = 0;
		haupthaus.fortschritt=1;
		player[controlIndex].buildings.push_back(haupthaus);
		player[controlIndex].buildings[player[controlIndex].buildings.size()-1].prepareShadowVolume();
		player[controlIndex].flooring(player[controlIndex].buildings[player[controlIndex].buildings.size()-1]);
		
		
		player[controlIndex].myPlayerIndex=0;
		player[controlIndex].initAIStrategies();
		currentLevel->getStartpointForPlayer("player0",player[0].startpunkt);
		player[controlIndex].myGrid.generate(7,3.4,player[0].startpunkt);
		player[controlIndex].myGrid.calculateNextFreeBlock(senseless);
	
		/*awx_unit tempu;
		tempu.playerIndex = 0;
		tempu.myIndex = 0;
		tempu.openFile("Data/units/buerger/zusammenbau.aax");
		tempu.fortschritt = 0;
		tempu.invisible = true;
		tempu.loadBlobs("Data/textures/blob.tga",1.2);
		player[controlIndex].units.push_back(tempu);*/
	
		renderLoadingScreen("Preparing player...");
		player[controlIndex].prepareGameForPlayer();

		renderLoadingScreen("Preparing enemy...");
		awx_building tempBuilding;
		tempBuilding.reinit(haupthaus);
		tempBuilding.playerIndex = 1;
		tempBuilding.fortschritt = 1;
		player[1].buildings.push_back(tempBuilding);
		player[1].buildings[player[1].buildings.size()-1].x=107.498;
		player[1].buildings[player[1].buildings.size()-1].y=2.51637;
		player[1].buildings[player[1].buildings.size()-1].z=17.3937;
		player[1].buildings[player[1].buildings.size()-1].buttons = haupthaus.buttons;
		player[1].buildings[player[1].buildings.size()-1].attributes = haupthaus.attributes;
		player[1].buildings[player[1].buildings.size()-1].workingdirectory = haupthaus.workingdirectory;
		player[1].buildings[player[1].buildings.size()-1].namen = haupthaus.namen;
		player[1].buildings[player[1].buildings.size()-1].winkel_y=-50;
		player[1].buildings[player[1].buildings.size()-1].rot_y=-50;
		player[1].myGrid.generate(7,3.4,player[1].startpunkt);
		player[1].myGrid.calculateNextFreeBlock(senseless);
		player[1].buildings[player[1].buildings.size()-1].x=player[1].startpunkt.x;
		player[1].buildings[player[1].buildings.size()-1].y=player[1].startpunkt.y;
		player[1].buildings[player[1].buildings.size()-1].z=player[1].startpunkt.z;
		player[1].flooring(player[1].buildings[player[1].buildings.size()-1]);
		

		
		player[1].myPlayerIndex=1;
		player[1].initAIStrategies();

		
		renderLoadingScreen("Generating shadows...");
		player[1].buildings[player[1].buildings.size()-1].prepareShadowVolume();
		controls.winkel_y=0;
	
		cout << "Buildings loaded in: " << (SDL_GetTicks()-startBuildings)/1000.0 << " secs." << endl << endl;
	}
	else
	{
		loadGameFile(saveGameName);
	
		mode=0;
	}


	
	//player[1].buildings[player[1].buildings.size()-1].buildUnit("ar");
	

	renderLoadingScreen("Preparing A*...");
	Uint32 gridTime = SDL_GetTicks();
	prepareAStarGrid();
	cout << "A Star grid allocated in: " << (SDL_GetTicks()-gridTime)/1000 << "secs. " << endl << endl;

	
	glPopAttrib();

	
	renderLoadingScreen("Loading GUI...");
	Uint32 startTimeGUI=SDL_GetTicks();
	initGUI(mode);
	cout << "GUI loaded in " << (SDL_GetTicks()-startTimeGUI)/1000.0 << endl;
	

	renderLoadingScreen("Allocationg sektors...");
	cout << "Allocating player-gameplay-sektors..." << endl;
	for(int i=0;i<player.size();i++)
	{
		player[i].allocateSektoren(currentLevel->terrain.sektorenX,currentLevel->terrain.sektorenZ);
	}
	
	#ifdef GCCDebugCompile
	#warning wrongPosition
	#endif
	renderLoadingScreen("Update FogOfWar...");
	if(mode==0)
	{
		updateAStarGridAtObject(player[0].buildings[0],awx_vector3f(player[0].buildings[0].x,player[0].buildings[0].y,player[0].buildings[0].z));
		updateAStarGridAtObject(player[1].buildings[0],awx_vector3f(player[1].buildings[0].x,player[1].buildings[0].y,player[1].buildings[0].z));
	}
	
	cout << endl << endl << "Game Content loaded in " << (SDL_GetTicks()-start)/1000.0 << " seconds." << endl << endl;
	
	
	
	// KI Debug setup
	if(mode==0)
	player[1].notabbauing=true;
	
	
	
	//setCurrentPlayer(1);
	//setCameraToEnemy();
	//setFogOfWar(1);
	
	//setupThreads();
	renderLoadingScreen("Finalize...");
	
	
	float light_position[]={0.0,10.0,50.0,1.0};
	//float direction[] = {0.0,1.0,0.0};
	float diffusel[] = {255.0/255.0,200.0/255.0,200.0/255.0,1};
	float ambient[] = {1.0,1.0,1.0,1.0};
	//float diffuse[] = {1,1,1,0.5};


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient); 
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffusel);
	//glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, direction);
	glLightfv(GL_LIGHT0,GL_SPECULAR,ambient);
	glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION, 0);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	
	
	startAIAndIdleThread();
	startTimerIdleThread(); 
	
	// Init KI
	initKI();
	
	if(mode==0)
	{
		player[0].buildings[0].lichtRadius();
		player[1].buildings[0].lichtRadius();
		
	}

	if(currentGameState==AWXPlayingIntroVideo)
		currentLevel->initUnteritelTexte();
	
	
	//globalFire
	globalFire = new awx_particleSystem();
	globalFire->loadFromFile("Data/ParticleSystems/fire5.apsys");
	
	globalFireSmoke = new awx_particleSystem();
	globalFireSmoke->loadFromFile("Data/ParticleSystems/fire4Smoke1.apsys");
	
	globalRandomGenerator = new awxRandom();
	globalRandomGenerator->initialize();
	
	
	farbAusgleichQuad = glGenLists(1);
	
	farbausgleichsShader.set("Data/shaders/farbausgleich.vert","Data/shaders/farbausgleich.frag");
	
	glNewList(farbAusgleichQuad,GL_COMPILE);
		
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0,1.0,0.0,1.0);
	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		
		glDisable(GL_DEPTH_TEST);

		farbausgleichsShader.install();
		
		glBegin(GL_QUADS);
			glVertex3f(0.0,0.0,0.0);
			glVertex3f(1.0,0.0,0.0);
			glVertex3f(1.0,1.0,0.0);
			glVertex3f(0.0,1.0,0.0);
		glEnd();
		
		glPopAttrib();
		
		glUseProgramObjectARB(NULL);
	
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		
	glEndList();

	cout << "Creating occlusion query object..." << endl;
	glGenQueriesARB(1,&iAmInInterfaceOcclusionQueryObject);
	cout << "Query created." << endl;
}

void awx_shutdown(int code)
{
	renderLoadingScreen("Cleanup...");

	cout << endl << "Cleanup:" << endl << endl;
	for(int i=0;i<(int)ttfFonts.size();i++)
	{
		ttfFonts[i].cleanup();
	}
	ttfFonts.clear();

	cout << "Fonts clean." << endl;
	
	
	player[controlIndex].units.clear();
	player[controlIndex].buildings.clear();
	
	cleanupAStarGrid();
	
	
	cout << "SDL clean." << endl;
	// GLListen entfernen
	glDeleteTextures(currentID,textur_liste);
	cout << "Textures clean" << endl;
	
	
	renderLoadingScreen("Kill Threads...");
	cout << "Waiting Threads" << endl;
	waitAllUserThreads();
	
	
	cout << "Kill Threads" << endl;
	//killAIAndIdleThread();
	//killParticleSystemThread();
	//killTimerIdleThread();
	//delete particleSystemHandler;
	
	renderLoadingScreen("Cleanup...");

	delete currentLevel;
	
	delete globalRandomGenerator;
	
	cout << "Dealloc KI" << endl;
	deallocKI();
	cout << "Dealloc Savegames" << endl;
	if(saveGames.size()>0) saveGames.clear();
	cout << "SDL Quit" << endl;
	

	// Originalvideoeinstellungen übernehmen
    SDL_Quit();
	
	cout << "Exit no\n\n";
	// Programm beenden
    if(code!=5) exit(code);
}

void awx_rendering()
{
	runtime.genFPS();

	if(project.shadowvolumes)
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);				// Farb und Tiefenbuffer l‚àö‚àÇschen
	else glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	


	if(currentGameState==AWXPlayingIntroVideo)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		string script="";
		int i=currentLevel->findCameraDriveByIdentifier("story");
		if(i!=-1) currentLevel->setUpMatriceWithCameraDrive(i,script);

		if(script=="startGame")
		{
			currentGameState = AWXStandardGame;
		}
		
		glGetDoublev(GL_MODELVIEW_MATRIX,project.mvmatrix);
		glGetDoublev(GL_PROJECTION_MATRIX,project.projmatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
		checkVisibilities();
	} else if(currentGameState==AWXExplainingGameToUser)
	{
		cout << "explainging" << endl;
	} else if(currentGameState==AWXStandardGame)
	{
		setUpCamera();
	} else cout << "ERROR: undefined gamestate." << endl;
	
	if(firstFrame<5)
	{
		checkVisibilities();
		firstFrame++;
	}
	glPushMatrix();
	{
		float light_position[]={0.0,500,000.0,0.0};
		glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	}
	glPopMatrix();	
 
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	render_level();

	{
		GLfloat ambient[] = {0.2,0.2,0.2,1.0};
		glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	
	}
	

	glDisable(GL_CULL_FACE);
	renderBlobs();

	renderLevelObjects();
	glEnable(GL_CULL_FACE);


	
	for(int u = 0; u < (int)player.size();u++){
		for(int i = 0; i < (int)player[u].units.size();i++)
		{
			if(player[u].units[i].fortschritt >= 100){
				player[u].units[i].playAll();
				//player[u].units[i].blood.render();
			}
		} 
	
		for(int i=0;i<(int)player[u].buildings.size();i++)
		{
			player[u].buildings[i].calculateQue();
			player[u].buildings[i].render();
		}
	}

	for(int u = 0; u < (int)player.size();u++){	
		for(int i=0;i<(int)player[u].buildings.size();i++)
		{
			player[u].buildings[i].renderSpecialEffects();
		}
	}

	#ifdef aStarDebug
	if(useAStarAlgo) renderCurrentPath();
	displayAStarGrid();
	#endif

	//player[0].myGrid.display();

	if(project.shadowvolumes)
	{
		if(project.shadowvolumeMode==AWXStaticVolume) shadowPass();
		else if(project.shadowvolumeMode==AWXDynamicVolume) shadowPassDynamic();
	}

	if(currentGameState==AWXStandardGame)
	{
		currentWeather.render();
	}
	
	renderGamePlayElements();

	if(visibitilityCheckInNextFrame){
		checkVisibilities();
		visibitilityCheckInNextFrame = false;
	}
	
	if(currentGameState==AWXPlayingIntroVideo)
	{
		currentLevel->findCameraDriveByIdentifier("story");

		renderUntertitelDisplay();
		
		SDL_GL_Enter2DMode();
			glColor4f(1.0,1.0,1.0,1.0);
			ttfFonts[currentLevel->untertitelTextField0].renderDirect();
			ttfFonts[currentLevel->untertitelTextField1].renderDirect();
	
		SDL_GL_Leave2DMode();
		
	}
	
	else if(currentGameState==AWXStandardGame) render_interface();
		
	if(currentGameState==AWXStandardGame) 
	{
		Uint32 diff = SDL_GetTicks()-lastFOWUpdate;
		if(diff>200||project.instandFOWUpdate)
		{
			updateFogOfWarForAllUnits();
			lastFOWUpdate=SDL_GetTicks();
		
			runtime.passiveFPS = 1000.0/diff;
			
				// AI Befehle ausf√ºhren
			for(int u = 0; u < (int)player.size();u++)
			{
					player[u].runAICommandList();
			}
		}
	}

	/*diff = SDL_GetTicks()-lastFOWDowngrade;
	if(diff>700)
	{
		currentLevel->terrain.doUnvisibleTerrain();
		lastFOWDowngrade=SDL_GetTicks();
	}*/
	
	
	
	SDL_GL_SwapBuffers();	

	if(currentGameState==AWXStandardGame) 
	{
		// Get query
		GLint result=0;
		glGetQueryObjectivARB(iAmInInterfaceOcclusionQueryObject,GL_QUERY_RESULT_ARB,&result);
	
		iAmInInterface=result;
	}

}

void intersectViewRayWithTerrain()
{
	awxRay ray;
	
	GLdouble nearf[3];
	GLdouble farf[3];
	awx_vector3f result;


	gluUnProject((GLdouble) controls.x,project.height - (GLdouble) controls.y,0.0,mvmatrix,project.projmatrix,project.viewport,&nearf[0],&nearf[1],&nearf[2]);
	gluUnProject((GLdouble) controls.x,project.height - (GLdouble) controls.y,1.0,mvmatrix,project.projmatrix,project.viewport,&farf[0],&farf[1],&farf[2]);
			
	ray.R0.x = nearf[0];
	ray.R0.y = nearf[1];
	ray.R0.z = nearf[2];
			
	ray.Rd.x = farf[0] - nearf[0];
	ray.Rd.y = farf[1] - nearf[1];
	ray.Rd.z = farf[2] - nearf[2];
			
	float len = sqrt(ray.Rd.x*ray.Rd.x + ray.Rd.y*ray.Rd.y + ray.Rd.z*ray.Rd.z);
			
	ray.Rd.x/=len;
	ray.Rd.y/=len;
	ray.Rd.z/=len;
	
	

	result = currentLevel->terrain.intersect(ray);

	wx=result.x;
	wy=result.y;
	wz=result.z;
}


