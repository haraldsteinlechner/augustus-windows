///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   Â© Alphaworx Corporation //  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////


void awx_building::updateBerufe()
{
	//Ich habe absolut keine Ahnung warum das sein muss ... 
	//wenn man das nicht macht hšren Einheiten plštzlich auf zu arbeiten
	//wenn man ein neues Haus baut
	
	for(int i = 0; i < berufe.size();i++)
	{
		if(berufe[i].unitIndex >= 0 && berufe[i].unitIndex < player[playerIndex].units.size())
		{
			player[playerIndex].units[berufe[i].unitIndex].beruf = &berufe[i];
		}
	}

	if(controls.selection.size() == 0)
	{
		//cout << myIndex << endl;
		for(int i = 0; i < berufe.size();i++)
		{
			float value = textFields[i].caption.floatValue();
			berufe[i].einkommen = value;
			char temp[128];
			sprintf(temp , "%.2f" , value);
			
			if(!(textFields[i].caption == temp))
			{
				ttfFonts[textFields[i].ttfTextID].cleanup();
				textFields[i].setCaption(temp);
				ttfFonts[textFields[i].ttfTextID].calculate(textFields[i].caption);
				textFields[i].pushedFlag = false;
			}
			
		}
	}
	else
	{
		for(int i = 0; i < textFields.size();i++)
		{
			if(!textFields[i].pushedFlag)
			{
				float value = textFields[i].caption.floatValue();
				berufe[i].einkommen = value;
				char temp[128];
				sprintf(temp , "%.2f" , value);
				
				if(!(textFields[i].caption == temp))
				{
					ttfFonts[textFields[i].ttfTextID].cleanup();
					textFields[i].setCaption(temp);
					ttfFonts[textFields[i].ttfTextID].calculate(textFields[i].caption);
				}
			}
		}
	}
}

void awx_building::calculateQue(){
	myWindowHeadPosition();
	if(unitQue.size() > 0){
		player[playerIndex].units[unitQue[0]].fortschritt += ((1/player[playerIndex].units[unitQue[0]].unitValues.bauzeit)/runtime.fps)*100.0;
			
		if(player[playerIndex].units[unitQue[0]].fortschritt >= 100){
			awx_matrix a;
			a.rotatef(winkel_y,0,1,0);
			// FAHNE
			if(fahne.x==AWXNull)
			{ 
				player[playerIndex].units[unitQue[0]].target.clear();
				fahne = awx_vector3f(x+RANDOM_FLOAT*2,y,z+RANDOM_FLOAT*2) + a.vectorProduct(awx_vector3f(0,0,10));
				player[playerIndex].units[unitQue[0]].target.push_back(fahne);
			}
			else
			{
				player[playerIndex].units[unitQue[0]].target.clear();
				player[playerIndex].units[unitQue[0]].target.push_back(fahne);
			}
		
			player[playerIndex].units[unitQue[0]].anvisiert = true;
		
			vector <int> oldQue;
			for(int i = 0; i < (int)unitQue.size();i++){
				oldQue.push_back(unitQue[i]);
			}
			unitQue.clear();
			for(int i = 0; i < (int)oldQue.size()-1;i++){
				unitQue.push_back(oldQue[i+1]);
			}
		}
	}
}

void awx_building::eraseQue(){
	cout << "Vorher: " << player[playerIndex].units.size();
	for(int i = (int)unitQue.size()-1; i >= 0;i--){
		player[playerIndex].units.erase(player[playerIndex].units.begin() + unitQue[i] , player[playerIndex].units.begin() + unitQue[i]+1);
	}
	cout << "Nachher: " << player[playerIndex].units.size() << endl;
	unitQue.clear();
}


// wenn userControlled -> einheit wird sofort ausgewÃ¤hlt
void awx_building::buildUnit(char* name, bool userControlled){
	awx_string namestr(name);
	int unitIndex = getLoadedUnitIndex(namestr);

	
	awx_unit tempu;
	tempu.playerIndex = playerIndex;
	
	int hausindex = -1;
	if(unitIndex == -1)
	{
		
		
		tempu.openFile((("Data/units/" + namestr) + "/zusammenbau.aax").cString());
		tempu.loadBlobs("Data/textures/blob.tga",1.2);
		tempu.position.set(x,y,z);
		tempu.initPosition();
		
		if(namestr == "buerger")
		{
			for(int i = 0; i < player[playerIndex].buildings.size();i++)
			{
				if(player[playerIndex].buildings[i].freierPlatz > 0)
				{
					hausindex = i;
					break;
				}
			}
			
			if(hausindex != -1)
			{
				player[playerIndex].buildings[hausindex].freierPlatz--;
				tempu.unitValues.wohnhausIndex = hausindex;
			}
			else tempu.unitValues.wohnhausIndex = -1;
			
			cout << "load: Unit " << player[playerIndex].units.size() << "  wohnt in Haus " << hausindex << endl; 

			tempu.buerger = true;
		}
		else tempu.buerger = false;
		
		
		player[playerIndex].units.push_back(tempu);
		
		player[playerIndex].units[player[playerIndex].units.size()-1].position.x=x;
		player[playerIndex].units[player[playerIndex].units.size()-1].position.y=y;
		player[playerIndex].units[player[playerIndex].units.size()-1].position.z=z;

		player[playerIndex].units[(int)player[playerIndex].units.size()-1].anvisiert = false;
		player[playerIndex].units[(int)player[playerIndex].units.size()-1].hasDir = false;
		player[playerIndex].units[(int)player[playerIndex].units.size()-1].myIndex = (int)player[playerIndex].units.size()-1;
		//	player[playerIndex].units[player[playerIndex].units.size()-1].myButtonID = buttons[0].texID;
		for(int i = 0; i < (int)namen.size();i++){
			if(namen[i].compare(name) == 0){
				player[playerIndex].units[player[playerIndex].units.size()-1].myButtonID = buttons[i].texID;
				break;
			}
		}
		if(playerIndex == controlIndex&&userControlled){
			for(int i = 0; i < controls.selection.size();i++)
			{
				if(controls.selection[i] >= player[playerIndex].units.size() &&
				   controls.selection[i] < player[playerIndex].units.size() + player[playerIndex].buildings.size())
				{
					controls.selection[i]+= 1;
				}
			}
		}
		
		unitQue.push_back((int)player[playerIndex].units.size()-1);
	}
	else
	{
		bool flag = true;
		float preis = 0;
		for(int i = 0; i < loadedUnits[unitIndex].unitValues.rohstoffe.size();i++)
		{
			if(loadedUnits[unitIndex].unitValues.rohstoffe[i].length() > 0)
			{
				if(player[playerIndex].produkte[loadedUnits[unitIndex].unitValues.rohstoffe[i]].floatValue() < 1)
				{
					flag = false;
					break;
				}
				else
				{
					preis += player[playerIndex].getPriceFor(loadedUnits[unitIndex].unitValues.rohstoffe[i]);
				}
			}
		}
		
		if(flag && preis < player[playerIndex].rohstoffvalues[AWXGold])
		{
			for(int i = 0; i < loadedUnits[unitIndex].unitValues.rohstoffe.size();i++)
			{
				if(loadedUnits[unitIndex].unitValues.rohstoffe[i].length() > 0)
				{
					float oldValue = player[playerIndex].produkte[loadedUnits[unitIndex].unitValues.rohstoffe[i]].floatValue();
					awx_string newValue(oldValue-1);
					player[playerIndex].produkte.setObject(newValue , loadedUnits[unitIndex].unitValues.rohstoffe[i].cString());
					//preis += player[playerIndex].getPriceFor(loadedUnits[unitIndex].unitValues.rohstoffe[i]);
				}
			}
			//cout << "Kostet: " << preis << endl;
			player[playerIndex].rohstoffvalues[AWXGold] -= preis;
			
			tempu.reinit(loadedUnits[unitIndex]);
			
			awx_string namestr(name);
			if(namestr == "buerger")
			{
				int hausindex = -1;
				for(int i = 0; i < player[playerIndex].buildings.size();i++)
				{
					if(player[playerIndex].buildings[i].freierPlatz > 0)
					{
						hausindex = i;
						break;
					}
				}
			
				if(hausindex != -1)
				{
					player[playerIndex].buildings[hausindex].freierPlatz--;
					tempu.unitValues.wohnhausIndex = hausindex;
				}
				else tempu.unitValues.wohnhausIndex = -1;
			
			
				//cout << "reinit: Unit " << player[playerIndex].units.size() << "  wohnt in Haus " << index << endl; 
			
				tempu.buerger = true;
			}
			else tempu.buerger = false;
		
						

			player[playerIndex].units.push_back(tempu);
		
			player[playerIndex].units[player[playerIndex].units.size()-1].blobtexturID = loadedUnits[unitIndex].blobtexturID;
			player[playerIndex].units[player[playerIndex].units.size()-1].blobsizex = loadedUnits[unitIndex].blobsizex;
			player[playerIndex].units[player[playerIndex].units.size()-1].blobsizey=loadedUnits[unitIndex].blobsizey;
			player[playerIndex].units[player[playerIndex].units.size()-1].position.x=x;
			player[playerIndex].units[player[playerIndex].units.size()-1].position.y=y;
			player[playerIndex].units[player[playerIndex].units.size()-1].position.z=z;
			player[playerIndex].units[player[playerIndex].units.size()-1].anvisiert = false;
			player[playerIndex].units[player[playerIndex].units.size()-1].hasDir = false;
		
			player[playerIndex].units[player[playerIndex].units.size()-1].buttons = loadedUnits[unitIndex].buttons;
			player[playerIndex].units[player[playerIndex].units.size()-1].attributes = loadedUnits[unitIndex].attributes;
			player[playerIndex].units[player[playerIndex].units.size()-1].workingdirectory = loadedUnits[unitIndex].workingdirectory;
			player[playerIndex].units[player[playerIndex].units.size()-1].namen = loadedUnits[unitIndex].namen;
			player[playerIndex].units[player[playerIndex].units.size()-1].myIndex = (int)player[playerIndex].units.size()-1;
		
		
		
		
		
			for(int i = 0; i < (int)namen.size();i++){
				if(namen[i].compare(name) == 0){
					player[playerIndex].units[player[playerIndex].units.size()-1].myButtonID = buttons[i].texID;
					break;
				}
			}
			
			if(playerIndex == controlIndex&&userControlled){
				controls.selection.clear();
				controls.selection.push_back(player[playerIndex].units.size() + myIndex);
			}
			unitQue.push_back((int)player[playerIndex].units.size()-1);
		}
	}
}

void awx_building::buildLevelPart(string name)
{
	controls.currentLevelPartDragName=new string(name);
}

void awx_building::lichtRadius()
{
	awx_vector3f ps = awx_vector3f(x,y,z);
	int currentSektorX,currentSektorZ;
	bool ergebnisflag;
	hitTriangle=currentLevel->terrain.hit(ps , ps.y , ergebnisflag, currentSektorX, currentSektorZ);
	if(hitTriangle>=0) currentLevel->terrain.doFogOfWarForTriangleInSektor(hitTriangle,currentSektorX,currentSektorZ,ps,3,20,1.0,true);
}

		void awx_building::mouseDown(int xi,int yi)
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
					buttons[i].mouseDown(x , y , 1);
				}	
			}	
			
			for(int i = 0; i < (int)textFields.size();i++)
			{
				if(x  >= textFields[i].x &&
					   y  >= textFields[i].y &&
				       x <= textFields[i].x + textFields[i].width &&
				       y  <= textFields[i].y + textFields[i].height)
				{
					textFields[i].mouseDown(x , y , 1);
				}	
				else
				{
					textFields[i].mouseOverFlag = false;
					textFields[i].pushedFlag = false;
				}
			}	
			
		}
		
		void awx_building::mouseOver(int xi , int yi){
			float x = (xi/float(project.width))*project.aspect;
			float y = (project.height - yi)/float(project.height);
			
			for(int i = 0; i < (int)buttons.size();i++)
			{
				buttons[i].mouseOver(x ,y);
				if(x  >= buttons[i].x &&
					   y  >= buttons[i].y &&
				       x <= buttons[i].x + buttons[i].width &&
				       y  <= buttons[i].y + buttons[i].height)
				{
					descriptionIndex = i;
						
					int index = -1;
					if(i < namen.size())index = getLoadedUnitIndex(namen[i]);
					if(index != -1)
					{
						#ifdef GCCCompile
						#warning anzeigen
						#endif
						float preis = 0;
						unitPrice = "";
						for(int i = 0; i < loadedUnits[index].unitValues.rohstoffe.size();i++)
						{
							preis += player[playerIndex].getPriceFor(loadedUnits[index].unitValues.rohstoffe[i]);
							unitPrice = unitPrice + loadedUnits[index].unitValues.rohstoffe[i] + "\n";
							//cout << i << ": " << loadedUnits[index].unitValues.rohstoffe[i] << endl;
						}
						char temp[128];
						sprintf(temp , "$%.2f" , preis);
						unitPrice = unitPrice + temp;
						cout << "Kosten: " << preis << endl;
					}
					return;
				}
			}
			
			
			for(int i = 0; i < (int)textFields.size();i++)
			{
				textFields[i].mouseOver(x ,y);
			}
			
			descriptionIndex = -1;
	}
	
	void awx_building::mouseUp(int xi , int yi)
	{
		float x = (xi/float(project.width))*project.aspect;
		float y = (project.height - yi)/float(project.height);
		
		for(int i = 0; i < (int)buttons.size();i++){
			buttons[i].mouseUp(x,y , 1);
			if(x  >= buttons[i].x &&
					   y  >= buttons[i].y &&
				       x <= buttons[i].x + buttons[i].width &&
				       y  <= buttons[i].y + buttons[i].height)
				{
					if(i>=namen.size())
					{
						cout << "build " << levelPartnamen[i-namen.size()] << endl;
						 buildLevelPart(string(levelPartnamen[i-namen.size()].cString()));
					}
					
					else buildUnit(namen[i].cString(),true);

					return;
				}
		}
		
		for(int i = 0; i < textFields.size();i++)
		{
			textFields[i].mouseUp(x,y,1);
		}
	}

void awx_building::renderButtons()
{
	
    glPushMatrix();
        int i=0;
        for(i = 0; i < (int)buttons.size();i++){
			bool flag = true;

			if(i < namen.size() && i >= 0)
			{
				int unitIndex = getLoadedUnitIndex(namen[i]);
				for(int u = 0; u < loadedUnits[unitIndex].unitValues.rohstoffe.size();u++)
				{
					if(loadedUnits[unitIndex].unitValues.rohstoffe[u].length() > 0)
					{
						if(player[playerIndex].produkte[loadedUnits[unitIndex].unitValues.rohstoffe[u]].floatValue() < 1)
						{
							flag = false;
							break;
						}
					}
				}
			}
			
			if(!flag)buttons[i].enabled = false;
			else buttons[i].enabled = true;
            buttons[i].render();
        }
        if(descriptionIndex >= 0){
            awx_show_info(myFirstTextFieldID+descriptionIndex);
			if(!(unitPrice == ttfFonts[unitPriceIndex].caption))
			{
				ttfFonts[unitPriceIndex].setCaption(unitPrice);
				ttfFonts[unitPriceIndex].calculate();
			}
			
			ttfFonts[unitPriceIndex].registerRender();
        }
    glPopMatrix();
    
	for(int i = 0; i < textFields.size();i++)
	{
		textFields[i].render();
	}
    
    
    glDisable(GL_TEXTURE_2D);
    
    float y = 0.12;
	
    for(int i = 0; i < berufe.size();i++)
    {

		for(int u = 0; u < berufTextIndices.size();u++)
		{
			//ttfFonts[berufTextIndices[i]].setRenderPosition(0.5*project.aspect,y);
			ttfFonts[berufTextIndices[u]].registerRender();
		}
		
        glEnable(GL_TEXTURE_2D);
		glColor3f(1,1,1);
        if(berufe[i].isVergeben())
        {
            awx_button but;
            but.width = 0.03;
            but.height = but.width;
            
            but.x = 0.5*project.aspect - but.width - 0.01;
            but.y = y+0.005;

			if(!player[playerIndex].units[berufe[i].unitIndex].inFirma)but.setTexture(buergerButtonID);
			else but.setTexture(buergerHereButtonID);
			
            but.render();
        }
        glDisable(GL_TEXTURE_2D);
        
        y -= 0.035;
    }
    
    
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
}

void updateYValuesAtObject(awx_building &object, awx_vector3f reference)
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

void awx_building::startFire()
{
	cout << "start fire" << endl;
	houseFire = new awx_particleSystem();
	houseFire->position = awx_vector3f(x,y,z);
	houseFire = globalFire;
	
	houseFireSmoke = new awx_particleSystem();
	houseFireSmoke->position = awx_vector3f(x,y,z);
	houseFireSmoke = globalFireSmoke;
}
		
