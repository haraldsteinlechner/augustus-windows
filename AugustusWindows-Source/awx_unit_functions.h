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

void awx_unit::buildBuilding(awx_vector3f position, bool computerControlled, int buildUnits){

	player[playerIndex].buildings[player[playerIndex].buildings.size()-1].x = position.x;
	player[playerIndex].buildings[player[playerIndex].buildings.size()-1].y = position.y;
	player[playerIndex].buildings[player[playerIndex].buildings.size()-1].z = position.z;
	
	updateAStarGridAtObject(player[controlIndex].buildings[player[controlIndex].buildings.size() -1],position);
	
	if(!computerControlled)
	{
		for(int i=0;i<(int)controls.selection.size();i++)
		{
			player[playerIndex].units[controls.selection[i]].setTarget(position);
			player[playerIndex].units[controls.selection[i]].workingOnBuildingID = (int)player[playerIndex].buildings.size()-1;
		}
	} else
	{
		player[playerIndex].units[buildUnits].setTarget(position);
		player[playerIndex].units[buildUnits].workingOnBuildingID = (int)player[playerIndex].buildings.size()-1;
	}
	
	//player[playerIndex].buildings[player[playerIndex].buildings.size()-1].calculateShadow();
	
	if(project.shadowvolumes&&player[playerIndex].buildings.size()>=2&&player[playerIndex].buildings[player[playerIndex].buildings.size()-1].winkel_y!=player[playerIndex].buildings[player[playerIndex].buildings.size()-2].winkel_y)
	{
		player[playerIndex].buildings[player[playerIndex].buildings.size()-1].prepareShadowVolume();
	}
	else if(project.shadowvolumes&&player[playerIndex].buildings.size()<2) player[playerIndex].buildings[player[playerIndex].buildings.size()-1].prepareShadowVolume();
	
	player[playerIndex].rohstoffvalues[0]-=player[playerIndex].buildings[player[playerIndex].buildings.size()-1].holzkosten;
	player[playerIndex].rohstoffvalues[1]-=player[playerIndex].buildings[player[playerIndex].buildings.size()-1].goldkosten;
	player[playerIndex].rohstoffvalues[2]-=player[playerIndex].buildings[player[playerIndex].buildings.size()-1].steinkosten;

	player[playerIndex].flooring(player[playerIndex].buildings[player[playerIndex].buildings.size()-1]);
	player[playerIndex].buildings[player[playerIndex].buildings.size()-1].lichtRadius();
	
}

bool awx_unit::rohstoffCheck(int playerIndex,int index)
{
	if(player[playerIndex].rohstoffvalues[0]<player[playerIndex].buildings[index].holzkosten||
	   player[playerIndex].rohstoffvalues[1]<player[playerIndex].buildings[index].holzkosten||
	   player[playerIndex].rohstoffvalues[2]<player[playerIndex].buildings[index].steinkosten)
	{
		startCounter = true;
		return false;
	}

	return true;
}

void awx_unit::placeBuilding(char* name,bool computerControlled){

	int index=0;
	int playerInd=0;
	rohstoffabbau=false;
	rohstoffabbauIndirect=false;
	
	int loadedIndex = getLoadedIndex(name);
	
	awx_building tempb;
	
	if(index != -1){
		if(!rohstoffCheck(playerIndex,index)) return;
		tempb.myIndex = player[playerIndex].buildings.size();
		tempb.reinit(loadedBuildings[loadedIndex]);
		tempb.fortschritt = 0;
		tempb.playerIndex = playerIndex;
		player[playerIndex].buildings.push_back(tempb);
		player[playerIndex].buildings[player[playerIndex].buildings.size()-1].myIndex = player[playerIndex].buildings.size()-1;
	}
	else{
		if(!rohstoffCheck(playerIndex,(int)player[playerIndex].buildings.size()-1 )) return;
		tempb.myIndex = player[playerIndex].buildings.size();
		tempb.openFile(("Data/buildings/" + *(new awx_string(name)) + "/zusammenbau.aax").cString());
		tempb.fortschritt = 0;
		playerInd = playerIndex;
		player[playerIndex].buildings.push_back(tempb);
		player[playerIndex].buildings[player[playerIndex].buildings.size()-1].myIndex = player[playerIndex].buildings.size()-1;
		cout << "Neues building geladen. PlayerIndex=" << playerIndex << "  index=" << player[playerIndex].buildings.size() << endl;
	}
	
	if(!computerControlled)
	{
		controls.currentDragID = (int)player[playerInd].buildings.size()-1;
		controls.currentDragPlayer = playerInd;
	}
}


// Interne Funktion zum Setzen von einem Targets. L√∂st keinen Angriff oder hausbau aus und verwendet keine Pfadfindung
void awx_unit::setTargetFix(awx_vector3f fix)
{
    anvisiert = true;
	target.clear();
    target.push_back(fix);
    hasDir = false;
    data[0].gotoBezeichnung("laufen" , 15);
    attacksth = false;
	tempflag = status;
	wildflag = false;
		// Pr√ºfen ob auf LevelPart gezielt wird
	//rohstoffabbau=false;
	//myCurrentRessourceID=-1;
	
	for(int i = 0; i < (int)player[playerIndex].buildings.size();i++)
	{
		if(player[playerIndex].buildings[i].boundingCollision(fix) && isAbleToBuild(player[playerIndex].buildings[i].workingdirectory.lastPathComponent())){
			workingOnBuildingID = i;
			workingOnBuildingIDBefore=-1;
			break;
		}
	}
}


void awx_unit::setTarget(awx_vector3f ziel, int indexOfSecondarySelectionAI)
{
    anvisiert = true;
    hasDir = false;
    data[0].gotoBezeichnung("laufen" , 15);
    attacksth = false;
	tempflag = status;
	wildflag = false;
	egoController.rot_y=0;
	isWorking=false;
	rohstoffabbau=false;
	rohstoffabbauIndirect=false;
	
	// Prüfen ob auf LevelPart gezielt wird
	rohstoffabbau=false;
	myCurrentRessourceID=-1;
	myRessID = -1;
	
	//controls.secondarySelection=-1;
	if(indexOfSecondarySelectionAI!=-1)
	{
		target.clear();
		target.push_back(awx_vector3f(currentLevel->data[indexOfSecondarySelectionAI].x,currentLevel->data[indexOfSecondarySelectionAI].y,currentLevel->data[indexOfSecondarySelectionAI].z));
		if(currentLevel->data[indexOfSecondarySelectionAI].isAbbaubar())
		{
			myCurrentRessourceID = indexOfSecondarySelectionAI;
			myRessID = currentLevel->data[indexOfSecondarySelectionAI].resourceID;
		} else cout << "not abbaubbar"  << indexOfSecondarySelectionAI << endl;
		
		
		target.clear();
		target.push_back(ziel);
		return;
	}
	else if(controls.secondarySelection!=-1)
	{
		target.clear();
		target.push_back(awx_vector3f(currentLevel->data[controls.secondarySelection].x,currentLevel->data[controls.secondarySelection].y,currentLevel->data[controls.secondarySelection].z));
		if(currentLevel->data[controls.secondarySelection].isAbbaubar())
		{
			myCurrentRessourceID = controls.secondarySelection;
			myRessID = currentLevel->data[controls.secondarySelection].resourceID;
		} else cout << "not abbaubbar"  << controls.secondarySelection << endl;
		
		
		target.clear();
		target.push_back(ziel);
		return;
	}

    if(attacksth){
        if(attackIndex >= 0 && attackIndex < (int)player[attackPlayer].units.size()){
            for(int i = 0; i < (int)player[attackPlayer].units[attackIndex].attacked.size();i++){
                if(player[attackPlayer].units[attackIndex].attacked[i].unitID == myIndex &&
                   player[attackPlayer].units[attackIndex].attacked[i].playerID == playerIndex)
                {
                    player[attackPlayer].units[attackIndex].attacked.erase(
                        player[attackPlayer].units[attackIndex].attacked.begin() + i , 
                        player[attackPlayer].units[attackIndex].attacked.begin() + i+1
                    );
                    break;
                }
            }
        }
        else if(attackIndex >= (int)player[attackPlayer].units.size() && attackIndex < (int)player[attackPlayer].units.size() + (int)player[attackPlayer].buildings.size()){
            int buildingIndex = attackIndex - (int)player[attackPlayer].units.size();
            for(int i = 0; i < (int)player[attackPlayer].buildings[buildingIndex].attacked.size();i++){
                if(player[attackPlayer].buildings[buildingIndex].attacked[i].unitID == myIndex &&
                   player[attackPlayer].buildings[buildingIndex].attacked[i].playerID == playerIndex)
                {
                    player[attackPlayer].buildings[buildingIndex].attacked.erase(
                        player[attackPlayer].buildings[buildingIndex].attacked.begin() + i , 
                        player[attackPlayer].buildings[buildingIndex].attacked.begin() + i+1
                    );
                    break;
                }
            }
        }
    }

	
	for(int i = 0; i < (int)player[playerIndex].buildings.size();i++)
	{
		if(player[playerIndex].buildings[i].boundingCollision(ziel) && isAbleToBuild(player[playerIndex].buildings[i].workingdirectory.lastPathComponent())){
			workingOnBuildingID = i;
			workingOnBuildingIDBefore=-1;
			target.clear();
			target.push_back(ziel);
			return;
		}
	}
	
	if(useAStarAlgo)
	{
		target.clear();
		if(!generatePathAStar(position,ziel,target)) 
		{
			target.clear();

			target.push_back(ziel);
		}
	}
	else
	{
		target.clear();
		//if(!generatePathAStar(position,ziel,target)) return;

		//target.clear();
		target.push_back(ziel);
	}
	//for_each(target.begin(),target.end(),plot);
	//for(int i=0;i<target.size();i++) cout << target[i] << endl;

}


void awx_unit::attack(int playerID , int selectionID){
	attacksth = true;
	attackPlayer = playerID;
	attackIndex = selectionID;
	
	rohstoffabbau=false; 
	rohstoffabbauIndirect=false;
	
	opponentUnit temp;
	temp.unitID = myIndex;
	temp.playerID = playerIndex;
	
	if(selectionID >= 0 && selectionID < (int)player[playerID].units.size()){
		awx_vector3f dir = (player[playerID].units[selectionID].position - position).normalize();
		
		target.clear();
		target.push_back(player[playerID].units[selectionID].position - dir*1.0);
		
		player[playerID].units[selectionID].attacked.push_back(temp);
		
		
		anvisiert = true;
		data[0].gotoBezeichnung("laufen" , 15);
	}
	else if(selectionID >= (int)player[playerID].units.size() && selectionID < (int)player[playerID].units.size() +  (int)player[playerID].buildings.size()){
		float attackX = player[playerID].buildings[selectionID - player[playerID].units.size()].x;
		float attackY = player[playerID].buildings[selectionID - player[playerID].units.size()].y;
		float attackZ = player[playerID].buildings[selectionID - player[playerID].units.size()].z;
		
		awx_vector3f dir = (awx_vector3f(attackX , attackY , attackZ) - position).normalize();
		
		player[playerID].buildings[selectionID - player[playerID].units.size()].attacked.push_back(temp);


		target.clear();
		target.push_back(awx_vector3f(attackX , attackY , attackZ) - dir*unitValues.reichweite/2.0);
		anvisiert = true;
		data[0].gotoBezeichnung("laufen" , 15);
	}
}

bool awx_unit::proveProducts()
{
	/*for(int i = 0;i < unitValues.rohstoffe.size();i++)
	{
		awx_string str = unitValues.rohstoffe[i];
		for(int u = 0; u < player[playerIndex].produkte.keys.size();u++)
		{
			if(player[playerIndex].produkte.keys[u] == unitValues.rohstoffe[i])
			{
				if(player[playerIndex].produkte.values[u].floatValue() < 1)
				{
					return false;
				}
			}
		}
	}*/
	
	return true;
}

void awx_unit::reduceRohstoffe()
{
/*	for(int i = 0;i < unitValues.rohstoffe.size();i++)
	{
		awx_string str = unitValues.rohstoffe[i];
		for(int u = 0; u < player[playerIndex].produkte.keys.size();u++)
		{
			cout << player[playerIndex].produkte.keys[u] << " " << player[playerIndex].produkte.values[u] << endl;
			if(player[playerIndex].produkte.keys[u] == unitValues.rohstoffe[i])
			{
				if(player[playerIndex].produkte.values[u].floatValue() > 1)
				{
					player[playerIndex].produkte.values[u] = awx_string(player[playerIndex].produkte.values[u].floatValue()-1);
				}
			}
		}
	}*/	
}

void awx_unit::idle()
{	
    awx_vector3f tempVector;
    awx_vector3f dir;
	
	t = 1/float(runtime.fps);
	
	fuzzyLogicValues.processMuedigkeit(t,mode);
	fuzzyLogicValues.unitValues = &unitValues;
	
	mode = IDLE;
	
	//if(fuzzyLogicValues.muedigkeit - 0.07*t > 0)fuzzyLogicValues.muedigkeit-=0.07*t;
	
	if(fortschritt < 100)return;

	// Wenn abgebaut wird
	if(rohstoffabbau)
	{
		mode = HARVESTING;
		bool u=false;
		// Rohstoff in Rucksack f√ºllen

		float tempf = fuzzyLogicValues.baugeschwindigkeit;//calcBaugeschwindigkeit();
		//tempf = unitValues.min_abbauspeed + tempf*(1-min_abbauspeed);
		//fuzzyLogicValues.baugeschwindigkeit = tempf;
		
		rohstoffabbau = currentLevel->data[myCurrentRessourceID].abbau(tempf*unitValues.abbauspeed*t*currentLevel->data[myCurrentRessourceID].abbaufaktor);
		unitValues.getrageneRohstoffe += currentLevel->data[myCurrentRessourceID].platzbedarf * currentLevel->data[myCurrentRessourceID].abbaufaktor*tempf;
		// Wenn rucksack mittlerweile voll -> aufh√∂ren abzubauen, aber in passier rohstoffabbaufindirect flag merken dass man nur rucksack leert
		if(unitValues.getrageneRohstoffe >= unitValues.rucksackgroesse)
		{
			rohstoffabbau=false;
			rohstoffabbauIndirect = true;
			u = true;
		}
	
		// Wenn jetzt nicht mehr aubgebaut wird
		if(!rohstoffabbau)
		{
			int distIn=0;
			
			// Wenn dies aufgrund √ºberf√ºlltem Rucksack passiert
			if(u)
			{
				distIn=findNextHouse();
				
				// Wenn es ein Haus gibt
				if(distIn>=0)
				{
					int ui=myCurrentRessourceID;
					cout << "setTarget" << awx_vector3f(player[playerIndex].buildings[distIn].x,player[playerIndex].buildings[distIn].y,player[playerIndex].buildings[distIn].z) << endl;
					setTargetFix(awx_vector3f(player[playerIndex].buildings[distIn].x,player[playerIndex].buildings[distIn].y,player[playerIndex].buildings[distIn].z));
					if(u) myCurrentRessourceID=ui;
					//rohstoffabbauIndirect=true;
					bringStuffHome=true;
				}
			}
			// Wenn nicht mehr abgebaut wird, weil es keine Rohstoffe mehr gibt
			else if(!u)
			{
				int nextTree=findNextRessourceObject();
				
				// Wenn es keine Rohstoffe mehr gibt -> wieder zum haus laufen
				if(distIn<0)
				{
					distIn=findNextHouse();
					
					if(distIn<0)
					{
						cout << "Kein Haus gefunden (rohstofffunktionen zeile 269)" << endl;
						return;
					}
					setTarget(awx_vector3f(player[playerIndex].buildings[distIn].x,player[playerIndex].buildings[distIn].y,player[playerIndex].buildings[distIn].z));
				}
				
				oldabbaustatte = awx_vector3f(currentLevel->data[nextTree].x,currentLevel->data[nextTree].y,currentLevel->data[nextTree].z);
				
				int arr=nextTree;
				
				findNextTree = false;
				
				// Vor zum n√§chsten baum gelaufen wird nochmal rucksack leeren weil er recht voll ist
				if((unitValues.getrageneRohstoffe/float(unitValues.rucksackgroesse))>0.5)
				{
					distIn=findNextHouse();
					
					findNextTree = true;
					setTargetFix(awx_vector3f(player[playerIndex].buildings[distIn].x,player[playerIndex].buildings[distIn].y,player[playerIndex].buildings[distIn].z));
				}
				
				else 
				{  
					setTargetFix(awx_vector3f(currentLevel->data[nextTree].x,currentLevel->data[nextTree].y,currentLevel->data[nextTree].z));
				}
				
				myCurrentRessourceID=arr;
				
				rohstoffabbauIndirect = true;
			}
		}
		return;
	}

	// ----------------- ABBAUEN STARTEN
	if(myCurrentRessourceID!=-1&&!bringStuffHome&&!rohstoffabbau&&currentLevel->data[myCurrentRessourceID].boundingCollision(position))
	{
		data[0].gotoBezeichnung("arbeiten");
		rohstoffabbau = true;
		rohstoffabbauIndirect = true;
		anvisiert = false;
		oldabbaustatte = target[0];
		return;
	}
		
	else if(workingOnBuildingID!=-1&&player[playerIndex].buildings[workingOnBuildingID].boundingCollision(position+richtungsvektor*1.2))
	{
		if(player[playerIndex].buildings[workingOnBuildingID].fortschritt==0||workingOnBuildingIDBefore!=workingOnBuildingID) data[0].gotoBezeichnung("arbeiten",2);
		
		//HAUS BAUEN:
		
		mode = BUILDING;
		float tempf = fuzzyLogicValues.baugeschwindigkeit;
		player[playerIndex].buildings[workingOnBuildingID].fortschritt+=((1.0/player[playerIndex].buildings[workingOnBuildingID].bauzeit)/runtime.fps) * tempf;

		
		anvisiert = false;
		if(player[playerIndex].buildings[workingOnBuildingID].fortschritt>=1.0)
		{
			data[0].gotoBezeichnung("stehen");
			workingOnBuildingID = -1;
		}
			workingOnBuildingIDBefore = workingOnBuildingID;
			
		// Rohstoffe zu Haus zur√ºckgebracht.
		if(rohstoffabbauIndirect&&!rohstoffabbau)
		{
			mode = HARVESTING;
			if(myRessID!=1)		// Wenn es kein Gold ist, direkt √ºbertragen
			player[playerIndex].rohstoffvalues[myRessID]+=int(unitValues.getrageneRohstoffe);//int(carriedUnits/currentLevel->data[myCurrentRessourceID].platzbedarf);
			else
			{
				// bei gold hat man zus√§tzlich zu dem Gold auch noch einen gewissen Teil an Steinen, der √ºber die abbauf√§higkeiten bestimmt wird
				player[playerIndex].rohstoffvalues[1]+=int(float(unitValues.getrageneRohstoffe)*0.6);
				player[playerIndex].rohstoffvalues[2]+=int(float(unitValues.getrageneRohstoffe)*0.4);
			}
			bringStuffHome=false;
		
			if(findNextTree)
			{ 
				int distIn=findNextRessourceObject();
				
				if(distIn==-1) cout << "Keine Rohstoffe mehr vorhanden." << endl;
				else
				{	
					oldabbaustatte = awx_vector3f(currentLevel->data[distIn].x,currentLevel->data[distIn].y,currentLevel->data[distIn].z);
					myCurrentRessourceID = distIn;
				}
			}
			
			int u=myCurrentRessourceID;
			setTargetFix(oldabbaustatte);
			unitValues.getrageneRohstoffe = 0;
			myCurrentRessourceID = u;
			
		}
	}
	
	// WICHTIG: immer wenn man target auf hausbounding setzt kann workingOnBuildingID auf valid springen. WARUM? damit sie immer vor h√§user stehen bleiben. muss noch verbessert werden (von EITSCH)
	
	
	if(!attacksth){
		awx_vector3f dif;
		if(status == AWX_AGGRESSIVE){
            for(int i = 0; i < (int)player.size();i++){
                if(i != playerIndex){
                    for(int u = 0; u < (int)player[i].units.size();u++){
                        dif = player[i].units[u].position - position;
                        if(dif.getLength() < sichtweite && player[i].units[u].fortschritt >= 100){
                            attack(i , u);
                            break;
                        }
                    }
                }
            }
		}
		else if(status == AWX_DEFENSIVE){
			if(attacked.size() > 0){
				for(int i = 0; i < (int)attacked.size();i++){
					dif = player[attacked[i].playerID].units[attacked[i].unitID].position - position;
					if(dif.getLength() < unitValues.reichweite){
						attack(attacked[i].playerID , attacked[i].unitID);
						break;
					}
				}
			}
		}
		else if(status == AWX_PASSIVE){
			if(attacked.size() > 0){
			}
		}
	}
	
	if(attacksth && attackPlayer >= 0 && attackPlayer < (int)player.size()){
		if(attackIndex >= 0 && attackIndex < (int)player[attackPlayer].units.size())
		{
			dir = player[attackPlayer].units[attackIndex].position - position;
		}
		else if(attackIndex >= (int)player[attackPlayer].units.size() && attackIndex < (int)player[attackPlayer].units.size() + (int)player[attackPlayer].buildings.size()
			    && !player[attackPlayer].buildings[attackIndex - (int)player[attackPlayer].units.size()].boundingCollision(position))
		{
			awx_vector3f attackPos(player[attackPlayer].buildings[attackIndex].x , player[attackPlayer].buildings[attackIndex].y , player[attackPlayer].buildings[attackIndex].z);
			dir = attackPos - position;
		}
	}
	
	if(attacksth && data[0].aktionen[int(data[0].current_frame)].aktionsID == 1 && dir.getLength() < unitValues.reichweite){
		if(attackIndex >= 0 && attackIndex < (int)player[attackPlayer].units.size()){
			mode = ATTACKING;
			//Einheit angreifen
			if(player[attackPlayer].units[attackIndex].unitValues.leben > 0)
			{
				awx_unit* opp = &player[attackPlayer].units[attackIndex];
				opp->unitValues.leben -= unitValues.calcSchaden(opp->unitValues);
					#ifdef GCCDebugCompile
					#warning Blut entfernt
					#endif
					/*blood.running=true;
					blood.x = modelview[12];
					blood.y = modelview[13];
					blood.z = modelview[14];*/
			}
			else{
				
				for(int i=0;i<(int)player[playerIndex].units.size();i++)
				{
					if(player[playerIndex].units[i].attackIndex == attackIndex)
					{
						player[playerIndex].units[i].data[0].gotoBezeichnung("stehen");
						player[playerIndex].units[i].attacksth = false;
						player[playerIndex].units[i].anvisiert = false;	
					}
				}
				
				player[attackPlayer].units.erase(player[attackPlayer].units.begin() + attackIndex , player[attackPlayer].units.begin() + attackIndex+1);
			}
		}
		else if(attackIndex >= (int)player[attackPlayer].units.size() && attackIndex < (int)player[attackPlayer].units.size() + (int)player[attackPlayer].buildings.size())
		{
			//haus angreifen
			if(player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].health > 0)
			{
				mode = ATTACKING;
				player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].health -= unitValues.calcHouseSchaden();
			}
			else{
				for(int i = 0; i < (int)player[playerIndex].units.size();i++){
					if(player[playerIndex].units[i].attackIndex == attackIndex){
						player[playerIndex].units[i].data[0].gotoBezeichnung("stehen" , 10);
						player[playerIndex].units[i].attacksth = false;
					}
				}
				// BUG // BUG BUG BUG
				int buildingIndex = attackIndex - (int)player[attackPlayer].units.size();
				player[attackPlayer].buildings[buildingIndex].eraseQue();
				player[attackPlayer].buildings.erase(player[attackPlayer].buildings.begin() + buildingIndex , player[attackPlayer].buildings.begin() + buildingIndex+1);
			}
		}
	}
	
    if(anvisiert || (attacksth && dir.getLength() > unitValues.reichweite) && !wildflag)
    {
        t = 1.0/float(runtime.fps);
        mode = RUNNING;
		
        if(attacksth){
			if(attackIndex >= 0 && attackIndex < (int)player[attackPlayer].units.size())
			{
				dir = player[attackPlayer].units[attackIndex].position - position;
				wildflag = false;
				target.clear();
				target.push_back(player[attackPlayer].units[attackIndex].position - dir.normalize()*unitValues.reichweite/2.0);
			}
			else if(attackIndex >= (int)player[attackPlayer].units.size() && attackIndex < (int)player[attackPlayer].units.size() + (int)player[attackPlayer].buildings.size())
			{
				awx_vector3f attackPos(player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].x , player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].y , player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].z);
				wildflag = false;
				dir = attackPos - position;
				target.clear();
				target.push_back(attackPos - dir.normalize()*unitValues.reichweite/2.0);
			}
			
			awx_vector3f y1(0,1,0);
            awx_vector3f x1 = (y1 * richtungsvektor).normalize();

			if(richtungsvektor.getLength() > 0){
				modelview[0] = x1.x;
				modelview[1] = x1.y;
				modelview[2] = x1.z;
        
				modelview[4] = y1.x;
				modelview[5] = y1.y;
				modelview[6] = y1.z;
        
				modelview[8] = richtungsvektor.x;
				modelview[9] = richtungsvektor.y;
				modelview[10] = richtungsvektor.z;
			}
        
            hasDir = true;
        }
        
        vec = target[0] - position;
        vec.y=0;
        richtungsvektor = vec.normalize();
        
        futurePoint = awx_vector3f(modelview[12] , 0 , modelview[14])+richtungsvektor;

        if(!hasDir){
            awx_vector3f y1(0,1,0);
            awx_vector3f x1 = (y1 * richtungsvektor).normalize();

            if(richtungsvektor.getLength() > 0){
				modelview[0] = x1.x;
				modelview[1] = x1.y;
				modelview[2] = x1.z;
        
				modelview[4] = y1.x;
				modelview[5] = y1.y;
				modelview[6] = y1.z;
        
				modelview[8] = richtungsvektor.x;
				modelview[9] = richtungsvektor.y;
				modelview[10] = richtungsvektor.z;
			}
        
            hasDir = true;

        }
        else{
            
			float min = 0.5;
			float tempf = min + (1-fuzzyLogicValues.muedigkeit)*(1-min);
            position = position + tempf*richtungsvektor*unitValues.geschwindigkeit*t;
            
            hastToCheckVisibility = true;
            
            awx_vector3f radius = target[0] - position;
				
            if(vec.getLength()<unitValues.geschwindigkeit*t ||
			   (attacksth && attackIndex >= (int)player[attackPlayer].units.size() && attackIndex < (int)player[attackPlayer].units.size() + (int)player[attackPlayer].buildings.size()
			   && player[attackPlayer].buildings[attackIndex - (int)player[attackPlayer].units.size()].boundingCollision(position + richtungsvektor * unitValues.reichweite)))
            {
				// Ist noch ein Ziel in der targetliste?
				if(target.size()>1)
				{
					target.erase(target.begin());
					
					vec = target[0] - position;
					vec.y=0;
					richtungsvektor = vec.normalize();
					
					awx_vector3f y1(0,1,0);
					awx_vector3f x1 = (y1 * richtungsvektor).normalize();

					if(richtungsvektor.getLength() > 0){
						modelview[0] = x1.x;
						modelview[1] = x1.y;
						modelview[2] = x1.z;
        
						modelview[4] = y1.x;
						modelview[5] = y1.y;
						modelview[6] = y1.z;
        
						modelview[8] = richtungsvektor.x;
						modelview[9] = richtungsvektor.y;
						modelview[10] = richtungsvektor.z;
					}
				} else
				{
					// ZIEL ERREICHT
					anvisiert = false;
					//dir.set(0,0,0);
					hasDir = false;
					
					//cout << rohstoffabbauIndirect <<"  " << rohstoffabbau << endl;
					if(rohstoffabbauIndirect)
					{
						data[0].gotoBezeichnung("arbeiten" , 7);
					}
                
					if(attacksth||rohstoffabbau){
						data[0].gotoBezeichnung("stechen" , 7);
						//position = position - richtungsvektor * v*t/2.0;
					}
					else
					{
						wildflag = true;
						//position = target;
						data[0].gotoBezeichnung("stehen" , 10);
					}
				}
            }
            
            
            hitTriangle = currentLevel->terrain.hit(position , position.y , ergebnisflag, currentSektorX, currentSektorZ);
			
			if(hitTriangle>=0)
			{
				player[playerIndex].gamePlaySektoren[currentSektorX][currentSektorZ].setVisited();
				updateFogOfWarFlag=true;
			}
		 }
    }
	
	if(position.isNan()) 
	{
		position = lastPositionNotNAN;
	} else lastPositionNotNAN = position;
	
	bool isIllegal=false;
	
	for(int i=0;i<16;i++)
	{
		if(isnan(modelview[i]))
		{
			for(int u=0;u<16;u++)
			{
				modelview[u] = lastModelviewNotNAN[u];
				isIllegal=true;
				//cout << "err" << endl;
			}
			break;
		}
	}
	if(!isIllegal)
	{
			for(int u=0;u<16;u++)
			{
				lastModelviewNotNAN[u] = modelview[u];
				isIllegal=true;
				//cout << "err" << endl;
				break;
			}
	}
	
	
	if(unitValues.durst <= 1 && !inFirma && !pennt)unitValues.durst += 0.02*t;
	//cout << myIndex << " " << unitValues.durst << endl;
	
	if(buerger && (!anvisiert || working || pennt))
	{
		
		int t_aufstehen;
		if(beruf != NULL && beruf->startArbeit*3600 - 3600 < 9*3600)
		{
			t_aufstehen = beruf->startArbeit*3600 - 3600 + rand3*1800;
		}
		else t_aufstehen = 9*3600 + rand3*3600;

		if(t_aufstehen < 0) t_aufstehen = 0;
		
		//cout << myIndex << "muss um " << t_aufstehen/3600.0 << " aufstehen und um " << unitValues.schlafenszeit.getDaytime()/3600.0 << " schlafen gehen -> " << unitValues.wohnhausIndex  << endl;				
								
		if(!(unitValues.schlafenszeit.getDaytime() > playedTime.getDaytime() && t_aufstehen < playedTime.getDaytime()) && 
		   unitValues.wohnhausIndex != -1)
		{
			//cout << myIndex << " -> " << unitValues.wohnhausIndex << endl;
			
			awx_building* wohnung = &player[playerIndex].buildings[unitValues.wohnhausIndex];
			awx_vector3f hausPos = awx_vector3f(wohnung->x , wohnung->y , wohnung->z);
			
			if(!pennt)
			{
				cout << myIndex << "geht pennen" << endl;
				//Init
				oldPosition = position;
				useAStarAlgo = false;
				setTarget(hausPos);
				working = false;
				inFirma = false;
				
				drinking = false;
				inBrunnen = false;
				
				eating = false;
				inWirtshaus = false;
				
				pennt = true;
			}
			else
			{
				if(wohnung->boundingCollision(position+richtungsvektor*1.3))
				{
					eintritt = position;
					position = hausPos;
					invisible = true;
					inWohnung = true;
				}
			
				if(inWohnung && pennt)
				{
					if(fuzzyLogicValues.muedigkeit > 0)fuzzyLogicValues.muedigkeit -= 0.7*t;
				}
			}
			
			working = false;
			inFirma = false;
			mode = IDLE;
		}
		else
		{
			if(pennt)
			{
				cout << myIndex << "steht auf" << endl;
				//Deinit
				useAStarAlgo = true;
				pennt = false;
				mode = RUNNING;
				position = eintritt;
				setTarget(oldPosition);
				inWohnung = false;
				invisible = false;
			}
		}
		
		if(!pennt)
		{
		    if(beruf != NULL && !pennt)
            {
                awx_time startTime2(int(beruf->startArbeit*3600+rand4*1800));
                awx_time endTime(int(beruf->arbeitsdauer*3600+rand2*3600 + startTime2.getDaytime()));
                
                startTime2.tag = 0;
                startTime2.monat = 0;
                startTime2.jahr = 0;
                
                endTime.tag = 0;
                endTime.monat = 0;
                endTime.jahr = 0;
                
                awx_building* firma;
                if(beruf->buildingIndex >= 0 && beruf->buildingIndex < player[playerIndex].buildings.size())
                {
                    firma = &player[playerIndex].buildings[beruf->buildingIndex];
                }
                else
				{
					firma = NULL;
				}
                
                if(startTime2.getDaytime() <= playedTime.getDaytime() &&
                   endTime.getDaytime() >= playedTime.getDaytime() && firma != NULL)
                {
                    //cout << "arbeiten" << endl;
                    awx_vector3f hausPos = awx_vector3f(firma->x , firma->y , firma->z);
                    
                    if(!working && beruf->buildingIndex != -1)
                    {
						cout << myIndex << "geht arbeiten" << endl;
                        //Initialisierung:
						
						drinking = false;
						inBrunnen = false;
				
						eating = false;
						inWirtshaus = false;
				
                        oldPosition = position;
                        useAStarAlgo = false;
                        setTarget(hausPos);
                    }
                    
                    //cout << fuzzyLogicValues.muedigkeit << endl;
                    //cout << position - hausPos << " "  << firma->boundingCollision(position+richtungsvektor*1.3)<< endl;
                    if(firma->boundingCollision(position+richtungsvektor*1.3))
                    {
                        inFirma = true;
                        eintritt = position;
                        position = hausPos;
                        invisible = true;
                    }
                    
                    if(inFirma && working)
                    {
                        mode = WORKING;
                        
                        for(int i = 0; i < beruf->produktIndices.size();i++)
                        {
							int ind2 = beruf->produktIndices[i];
							
							float prevValue;
							if(beruf->produktIndices[i] < produkte.size())
							{
								prevValue = player[playerIndex].produkte[produkte[beruf->produktIndices[i]]].floatValue();
								float faktor = beruf->produktMenge[i];
								awx_string newValue = awx_string(float(prevValue + faktor*(t*zeitfaktor)/3600.0));
								char* key = produkte[beruf->produktIndices[i]].cString();
                            
								player[playerIndex].produkte.setObject(newValue , key);
							}
                        }
                        
                        /*for(int u = 0; u < player[playerIndex].produkte.keys.size();u++)
                        {
                            cout << "Produkt["<< u << "] = " << player[playerIndex].produkte.keys[u] <<  " :" << player[playerIndex].produkte.values[u] << endl;
                        }*/
                        
                        
                        fuzzyLogicValues.processMuedigkeit(t,mode);
                    }
                    working = true;	
                    
                    //cout << myIndex << " geht arbeiten" << endl;
                }
                else
                {
                    //cout << "nicht arbeiten" << endl;
                    if(working)
                    {
						cout << myIndex << "geht nicht mehr arbeiten" << endl;
                        unitValues.verdient(beruf->einkommen);
						player[playerIndex].rohstoffvalues[AWXGold] += beruf->einkommen*einkommenssteuer;
                        
                        position = eintritt;
                        setTargetFix(oldPosition);
                        useAStarAlgo = true;
                        invisible = false;
                        mode = RUNNING;
                        inFirma = false;
                    }
                    working = false;
					
					if(unitValues.durst >= 1 || drinking)
					{
						if(!drinking)
						{
							float min_dif = FLT_MAX;
							int index = -1;
							
							for(int i = 0; i < player[playerIndex].buildings.size();i++)
							{
								if(player[playerIndex].buildings[i].workingdirectory.lastPathComponent() == "brunnen")
								{
									awx_vector3f hausPos(player[playerIndex].buildings[i].modelview[12] , player[playerIndex].buildings[i].modelview[13] , player[playerIndex].buildings[i].modelview[14]);
									if((hausPos - position).getLength() < min_dif)
									{
										min_dif = (hausPos - position).getLength();
										index = i;
									}
								}
							}
							
							if(index > -1)
							{
								//Initialisierung:
								oldPosition = position;
								useAStarAlgo = false;
								brunnen = &player[playerIndex].buildings[index];
								awx_vector3f hausPos(brunnen->modelview[12] , brunnen->modelview[13] , brunnen->modelview[14]);
								setTarget(hausPos);
								
								drinking = true;
								inBrunnen = false;
								
								eating = false;
								inWirtshaus = false;
							}
							else cout << "Kein Brunnen gefunden" << endl;
							
						}
						
						if(brunnen != NULL)
						{
							if(brunnen->boundingCollision(position+richtungsvektor*1.3))
							{
								
								//setTarget(position);
								//data[0].gotoBezeichnung("stehen");
								//anvisiert = false;
								target.clear();
								anvisiert = false;
								data[0].gotoBezeichnung("stehen");
								
								inBrunnen = true;
							}	
							
							if(inBrunnen && drinking)
							{
								unitValues.durst -= 0.1*t;
							}
						}
					}
					
					if(drinking && unitValues.durst <= 0)
					{
						drinking = false;
						inBrunnen = false;
						setTarget(oldPosition);
					}
                }
			}
			
			
			//if(beruf != NULL)cout << "Unit #" << beruf->unitIndex << " arbeitet in Haus #" << beruf->buildingIndex << " als " << beruf->name << endl;
			
			/*if(!working)
			{
				if(durst >= 1)
				{
					float min = FLT_MAX;
					int index = -1;
				
					for(int i = 0; i < player[playerIndex].buildings.size();i++)
					{
						if(player[playerIndex].buildings[i].workingdirectory.lastPathComponent() == "wirtshaus")
						{
							awx_vector3f dif = player[playerIndex].buildings[i] - position;
							if(dif.getLength() < min)
							{
								min = dif.getLength();
								index = i;
							}
						}
					}
				}
				
				//setTarget(player[playerIndex].buildings[index])
			}*/
        }
	}

    modelview[12] = position.x;
    modelview[13] = position.y;
    modelview[14] = position.z;
}

int awx_unit::findNextRessourceObject()
{
	float minDist=FLT_MAX,dist;
	int distIn=-1;
			
	for(int i=0;i<currentLevel->number_of_objects;i++)
	{
		dist = fabs( (awx_vector3f(currentLevel->data[i].x,currentLevel->data[i].y,currentLevel->data[i].z)-position).getLength());
		if(dist<minDist&&currentLevel->data[i].resourceID==myRessID&&currentLevel->data[i].capacity>0)
		{
			minDist = dist;
			distIn = i;
		}
	}
	
	return distIn;
}

int awx_unit::findNextHouse()
{
	float minDist=FLT_MAX,dist;
	int distIn=-1;
	
	for(int i=0;i<(int)player[playerIndex].buildings.size();i++)
	{
		if(player[playerIndex].buildings[i].fortschritt<1||!player[playerIndex].buildings[i].acceptsRessourceWithID(myRessID)) continue;
		dist = fabs( (awx_vector3f(player[playerIndex].buildings[i].x,player[playerIndex].buildings[i].y,player[playerIndex].buildings[i].z) - position).getLength());
		if(dist<minDist)
		{
			minDist = dist;
			distIn = i;
		}
	}
	
	return distIn;
}

int awx_unit::findRessourceWithID(int ID)
{
	float minDist=FLT_MAX,dist;
	int distIn=-1;
			
	for(int i=0;i<currentLevel->number_of_objects;i++)
	{
		dist = fabs( (awx_vector3f(currentLevel->data[i].x,currentLevel->data[i].y,currentLevel->data[i].z)-position).getLength());
		if(dist<minDist&&currentLevel->data[i].resourceID==ID&&currentLevel->data[i].capacity>0)
		{
			minDist = dist;
			distIn = i;
		}
	}
	
	return distIn;
}

void awx_unit::mineLevelPart(int ID)
{
	anvisiert = true;
    hasDir = false;
    data[0].gotoBezeichnung("laufen" , 15);
    attacksth = false;
	tempflag = status;
	wildflag = false;
	egoController.rot_y=0;
	
	// Pr√ºfen ob auf LevelPart gezielt wird
	rohstoffabbau=false;
	unitValues.getrageneRohstoffe = 0;
	
	target.clear();
	target.push_back(awx_vector3f(currentLevel->data[ID].x,currentLevel->data[ID].y,currentLevel->data[ID].z));
	cout << "TARGET=" << target[0] << endl;
	myCurrentRessourceID = ID;
	myRessID = currentLevel->data[ID].resourceID;
}
