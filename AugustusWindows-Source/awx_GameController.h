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

void setFogOfWar(float value)
{
	currentLevel->terrain.fillFogOfWar(value);
	currentLevel->terrain.fillFogOfWarForAllSektors(value);
}

void setDebug(bool f)
{
	controls.debugshader=f;
}

void setRohstoff(int ID,float val)
{
	player[controlIndex].rohstoffvalues[ID] = val;
}

void enterEgoMode()
{
	// Noch nicht richtig abgefangen
	if(controls.selection[0]>=0&&controls.selection[0]<player[controlIndex].units.size())
	{
		controls.egoModeUnitIndex=controls.selection[0];
		controls.egoMode=true;
	}
}

void leaveEgoMode()
{
	controls.egoMode=false;
	
	hastToCheckVisibility=true;
	checkVisibilities();
}

void setCameraToEnemy()
{
    /*position.setX(-143.0);
    position.setY(56.0);
    position.setZ(62.0);
	direction.setX(-143);
    direction.setY(-6.4);
    direction.setZ(76.14);*/
	camera.position = awx_vector3f(100.95,60.863798,12.6657);
	camera.direction = awx_vector3f(100.95,0.863798,23.6657);
}

void setCameraToPlayer()
{
	camera.setup();
}

void openSteuerWindow()
{
	interface_gui.windows[steuerFensterID].hidden=false;
	cout << interface_gui.windows[steuerFensterID].sliders[0].width << endl;
	interface_gui.windows[steuerFensterID].sliders[0].width+=0.01;
}

void setSteuer(float steuer)
{
	player[controlIndex].steuerSatz= steuer;
}

void setWeather(float alpha)
{
	cout << "weather=" << alpha << endl;
	currentWeather.fog.alpha=alpha;
}

void startRain()
{
	currentWeather.fog.startSimulation();
}

void setTerrainRenderMode(bool a)
{
	allowTerrainCulling = a;
}

void awx_resetScene()
{
	player[controlIndex].units.clear();
	player[controlIndex].buildings.erase(player[controlIndex].buildings.begin()+1, player[controlIndex].buildings.end());
	
	camera.setup();
	
	checkVisibilities();
	firstFrame=0;
}

void saveGameFile(awx_string filename)
{
	filename = "Savegames/save" + filename + ".awx";
	FILE *file = fopen(filename.cString(),"wb");
	
	if(file==NULL)
	{
		cout << "Savegame konnte nicht gespeichert werden." << endl;
	}
	
	cout << "Saving game..." << endl;
	cout << "Writing global states..." << endl;
	camera.writeToFile(file);
	cout << "Writing players.." << endl;
	fprintf(file,";;players=%i;;",player.size());
	for(int i=0;i<player.size();i++)
	{
		player[i].writeToFile(file);
	}
	
		
	cout << "Writing FOW" << endl;
	fprintf(file,"###TERRAIN###");
	
	currentLevel->terrain.writeFogOfWarToFile(file);
	
	fclose(file);
}

void loadGameFile(awx_string filename)
{
	int size=0;
	filename = filename;
	cout <<"file=" << filename << endl;
	FILE *file = fopen(filename.cString(),"rb");
	
	if(file==NULL)
	{
		cout << "Savegame konnte nicht geöffnet werden." << endl;
	}
	
	cout << "Loading game..." << endl;
	
	cout << "Loading global states..." << endl;	
	camera.loadFromFile(file);
	cout << "Loading players..." << endl;
	fscanf(file,";;players=%i;;",&size);
	
	awx_player tempPlayer;
	for(int i=0;i<size;i++) player.push_back(tempPlayer);
	for(int i=0;i<size;i++)
	{
		player[i].loadFromFile(file,i);
	}
	
	cout << "Reading FOW" << endl;
	fscanf(file,"###TERRAIN###");
	
	currentLevel->terrain.readFogOfWarFromFile(file);
	
	fclose(file);
}

void useAStar(bool aStar)
{
	useAStarAlgo = aStar;
}

void makeLevelPart()
{
	currentLevel->addLevelPartFromBuilding("feld");
}

void startFire(int houseIndex)
{
	player[0].buildings[houseIndex].startFire();
}