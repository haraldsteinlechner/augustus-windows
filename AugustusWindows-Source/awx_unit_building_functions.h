void awx_unit::buildBuilding(awx_vector3f position){

	player[playerIndex].buildings[player[playerIndex].buildings.size()-1].x = position.x;
	player[playerIndex].buildings[player[playerIndex].buildings.size()-1].y = position.y;
	player[playerIndex].buildings[player[playerIndex].buildings.size()-1].z = position.z;
	
	for(int i=0;i<(int)controls.selection.size();i++)
	{
		player[playerIndex].units[controls.selection[i]].setTarget(position);
		player[playerIndex].units[controls.selection[i]].workingOnBuildingID = (int)player[playerIndex].buildings.size()-1;
	}
	
	//player[playerIndex].buildings[player[playerIndex].buildings.size()-1].calculateShadow();
	
	if(player[playerIndex].buildings[player[playerIndex].buildings.size()-1].winkel_y!=0)
	player[playerIndex].buildings[player[playerIndex].buildings.size()-1].prepareShadowVolume();
	
	player[playerIndex].rohstoffvalues[0]-=player[playerIndex].buildings[player[playerIndex].buildings.size()-1].goldkosten;
	player[playerIndex].rohstoffvalues[1]-=player[playerIndex].buildings[player[playerIndex].buildings.size()-1].holzkosten;
	player[playerIndex].rohstoffvalues[2]-=player[playerIndex].buildings[player[playerIndex].buildings.size()-1].steinkosten;
}

bool awx_unit::rohstoffCheck(int playerIndex,int index)
{
	if(player[playerIndex].rohstoffvalues[0]<player[playerIndex].buildings[index].goldkosten||
	   player[playerIndex].rohstoffvalues[1]<player[playerIndex].buildings[index].holzkosten||
	   player[playerIndex].rohstoffvalues[2]<player[playerIndex].buildings[index].steinkosten)
	{
		startCounter = true;
		return false;
	}

	return true;
}

void awx_unit::placeBuilding(char* name){

	bool flag = false;
	int index=0;
	int playerInd=100;
	rohstoffabbau=false;
	rohstoffabbauIndirect=false;
	
	for(int u = 0; u < (int)player.size();u++){
		for(int i = 0; i < (int)player[u].buildings.size();i++){
			if(strcmp(player[playerIndex].buildings[i].workingdirectory.lastPathComponent().cString() , name)==0){
				index = i;		
				playerInd = u;
				flag = true;
				break;
			}
		}
	}
	
	awx_building tempb;
	
	if(flag){
		tempb.reinit(player[playerInd].buildings[index]);
		if(!rohstoffCheck(playerIndex,index)) return;
		tempb.fortschritt = 0;
		tempb.playerIndex = playerIndex;
		player[playerIndex].buildings.push_back(tempb);
	}
	else{
		tempb.openFile(("Data/buildings/" + awx_string(name) + "/zusammenbau.aax").cString());
		if(!rohstoffCheck(playerIndex,(int)player[playerIndex].buildings.size() )) return;
		tempb.fortschritt = 0;
		playerInd = playerIndex;
		player[playerIndex].buildings.push_back(tempb);
		cout << "Neues building geladen. PlayerIndex=" << playerIndex << "  index=" << player[playerIndex].buildings.size() << endl;
	}
	
	controls.currentDragID = (int)player[playerInd].buildings.size()-1;
	controls.currentDragPlayer = playerInd;
}

void awx_unit::setTargetFix(awx_vector3f fix)
{
    anvisiert = true;
    target = fix;
    hasDir = false;
    data[0].gotoBezeichnung("laufen" , 15);
    attacksth = false;
	tempflag = status;
	wildflag = false;
	
		// Prüfen ob auf LevelPart gezielt wird
	rohstoffabbau=false;
	myCurrentRessourceID=-1;
	
	for(int i = 0; i < (int)player[playerIndex].buildings.size();i++)
	{
		if(player[playerIndex].buildings[i].boundingCollision(fix) && isAbleToBuild(player[playerIndex].buildings[i].workingdirectory.lastPathComponent())){
			workingOnBuildingID = i;
			workingOnBuildingIDBefore=-1;
			break;
		}
	}
}


void awx_unit::setTarget(awx_vector3f ziel){


    anvisiert = true;
    target = ziel;
    hasDir = false;
    data[0].gotoBezeichnung("laufen" , 15);
    attacksth = false;
	tempflag = status;
	wildflag = false;
	
	// Prüfen ob auf LevelPart gezielt wird
	rohstoffabbau=false;
	myCurrentRessourceID=-1;
	myRessID = -1;
	carriedUnits = 0;
	
	if(controls.secondarySelection!=-1)
	{
		target = awx_vector3f(level10.data[controls.secondarySelection].x,0,level10.data[controls.secondarySelection].z);
		
		if(level10.data[controls.secondarySelection].isAbbaubar())
		{
			myCurrentRessourceID = controls.secondarySelection;
			myRessID = level10.data[controls.secondarySelection].resourceID;
		}else cout << "notabbaubar" << endl;
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
			break;
		}
	}
	
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
		
		target = player[playerID].units[selectionID].position - dir*1.0;
		
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


		
		target = awx_vector3f(attackX , attackY , attackZ) - dir*reichweite/2.0;
		anvisiert = true;
		data[0].gotoBezeichnung("laufen" , 15);
	}
}

void awx_unit::idle()
{	
    awx_vector3f tempVector;
    awx_vector3f dir;
	
	t = 1/float(runtime.fps);
	
	if(fortschritt < 100)return;

	if(rohstoffabbau)
	{
		bool u=false;
		rohstoffabbau = level10.data[myCurrentRessourceID].abbau(abbauspeed*t*level10.data[myCurrentRessourceID].abbaufaktor);
		carriedUnits += level10.data[myCurrentRessourceID].platzbedarf * level10.data[myCurrentRessourceID].abbaufaktor;
		if(carriedUnits>=rucksackgroesse)
		{
			rohstoffabbau=false;
			rohstoffabbauIndirect = true;
			u = true;
		}
	
		if(!rohstoffabbau)
		{
			float minDist=FLT_MAX,dist;
			int distIn=0;
			
			if(u) //||(carriedUnits/rucksackgroesse)>0.5 wäre sinnvoll
			{
				// nächstes haus
				for(int i=0;i<(int)player[playerIndex].buildings.size();i++)
				{
					if(player[playerIndex].buildings[i].ressID!=myRessID||player[playerIndex].buildings[i].fortschritt<1) continue;
					dist = fabs( (awx_vector3f(player[playerIndex].buildings[i].x,player[playerIndex].buildings[i].y,player[playerIndex].buildings[i].z) - position).getLength());
					if(dist<minDist)
					{
						minDist = dist;
						distIn = i;
					}
				}
				
				if(distIn>=0)
				{
				
				int ui=myCurrentRessourceID;
				setTargetFix(awx_vector3f(player[playerIndex].buildings[distIn].x,player[playerIndex].buildings[distIn].y,player[playerIndex].buildings[distIn].z));
				if(u) myCurrentRessourceID=ui;
				}
			}
			else if(!u)
			{
				// Nächsten Baum finden.
				for(int i=0;i<level10.number_of_objects;i++)
				{
					dist = fabs( (awx_vector3f(level10.data[i].modelview[12],level10.data[i].modelview[13],level10.data[i].modelview[14])-position).getLength());
					if(dist<minDist&&i!=myCurrentRessourceID&&level10.data[i].resourceID==myRessID&&level10.data[i].capacity>0)
					{
						minDist = dist;
						distIn = i;
					}
				}
				
				if(distIn<0)
				{
					minDist=FLT_MAX;
					distIn=0;
			
					for(int i=0;i<(int)player[playerIndex].buildings.size();i++)
					{
						if(player[playerIndex].buildings[i].ressID!=myRessID||player[playerIndex].buildings[i].fortschritt<1) continue;
						dist = fabs( (awx_vector3f(player[playerIndex].buildings[i].x,player[playerIndex].buildings[i].y,player[playerIndex].buildings[i].z) - position).getLength());
						if(dist<minDist)
						{
							minDist = dist;
							distIn = i;
						}
					}
					
					setTarget(awx_vector3f(player[playerIndex].buildings[distIn].x,player[playerIndex].buildings[distIn].y,player[playerIndex].buildings[distIn].z));
				}
				
				oldabbaustatte = awx_vector3f(level10.data[distIn].modelview[12],level10.data[distIn].modelview[13],level10.data[distIn].modelview[14]);
				
				int arr=distIn;
				minDist=FLT_MAX;
				
				findNextTree = false;
				
				if((carriedUnits/rucksackgroesse)>0.5)
				{
					findNextTree = true;
				}
				
				else 
				{  
					setTargetFix(awx_vector3f(level10.data[distIn].modelview[12],level10.data[distIn].modelview[13],level10.data[distIn].modelview[14]));
				}
				
				myCurrentRessourceID=arr;
				
				rohstoffabbauIndirect = true;
			}
		}
		return;
	}

	if(myCurrentRessourceID!=-1&&!rohstoffabbau&&fabs((position-target).getLength())<1.8)
	{
		data[0].gotoBezeichnung("arbeiten");
		rohstoffabbau = true;
		rohstoffabbauIndirect = true;
		anvisiert = false;
		oldabbaustatte = target;
		return;
	}
		
	else if(workingOnBuildingID!=-1&&player[playerIndex].buildings[workingOnBuildingID].boundingCollision(position+richtungsvektor*1.2))
	{
		if(player[playerIndex].buildings[workingOnBuildingID].fortschritt==0||workingOnBuildingIDBefore!=workingOnBuildingID) data[0].gotoBezeichnung("arbeiten",2);
		player[playerIndex].buildings[workingOnBuildingID].fortschritt+=((1.0/player[playerIndex].buildings[workingOnBuildingID].bauzeit)/runtime.fps);
		anvisiert = false;
		if(player[playerIndex].buildings[workingOnBuildingID].fortschritt>=1.0)
		{
			data[0].gotoBezeichnung("stehen");
			workingOnBuildingID = -1;
		}
			workingOnBuildingIDBefore = workingOnBuildingID;
			
		if(rohstoffabbauIndirect&&!rohstoffabbau)
		{
			player[playerIndex].rohstoffvalues[myRessID]+=int(carriedUnits);//int(carriedUnits/level10.data[myCurrentRessourceID].platzbedarf);
		
			if(findNextTree)
			{
				float minDist=FLT_MAX,dist;
				int distIn=0;
			
				for(int i=0;i<level10.number_of_objects;i++)
				{
					dist = fabs( (awx_vector3f(level10.data[i].x,level10.data[i].y,level10.data[i].z)-position).getLength());
					if(dist<minDist&&level10.data[i].resourceID==myRessID&&level10.data[i].capacity>0)
					{
						minDist = dist;
						distIn = i;
					}
				}
				
				oldabbaustatte = awx_vector3f(level10.data[distIn].x,level10.data[distIn].y,level10.data[distIn].z);
				myCurrentRessourceID = distIn;
			}
			
			int u=myCurrentRessourceID;
			setTargetFix(oldabbaustatte);
			carriedUnits = 0;
			myCurrentRessourceID = u;
			
		}
	}
	
	// WICHTIG: immer wenn man target auf hausbounding setzt kann workingOnBuildingID auf valid springen. WARUM? damit sie immer vor häuser stehen bleiben. muss noch verbessert werden (von EITSCH)
	
	
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
					if(dif.getLength() < reichweite){
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
	
	if(attacksth && data[0].aktionen[int(data[0].current_frame)].aktionsID == 1 && dir.getLength() < reichweite){
		if(attackIndex >= 0 && attackIndex < (int)player[attackPlayer].units.size()){
			//Einheit angreifen
			if(player[attackPlayer].units[attackIndex].health > 0)
			{
				float test = RANDOM_FLOAT * angriffswert;
				if(test > player[attackPlayer].units[attackIndex].armor || RANDOM_FLOAT > 0.80){
					player[attackPlayer].units[attackIndex].health -= schaden_min + RANDOM_FLOAT*(schaden_max - schaden_min);
					blood.running=true;
					blood.x = modelview[12];
					blood.y = modelview[13];
					blood.z = modelview[14];
				}
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
				float test = RANDOM_FLOAT * angriffswert;
				if(test > player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].armor || RANDOM_FLOAT > 0.75){
					player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].health -= schaden_min + RANDOM_FLOAT*(schaden_max - schaden_min);
					cout << player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].health << endl;
				}
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
	
    if(anvisiert || (attacksth && dir.getLength() > reichweite) && !wildflag)
    {
        t = 1/float(runtime.fps);
        
        if(attacksth){
			if(attackIndex >= 0 && attackIndex < (int)player[attackPlayer].units.size())
			{
				dir = player[attackPlayer].units[attackIndex].position - position;
				wildflag = false;
				target = player[attackPlayer].units[attackIndex].position - dir.normalize()*reichweite/2.0;
			}
			else if(attackIndex >= (int)player[attackPlayer].units.size() && attackIndex < (int)player[attackPlayer].units.size() + (int)player[attackPlayer].buildings.size())
			{
				awx_vector3f attackPos(player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].x , player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].y , player[attackPlayer].buildings[attackIndex - player[attackPlayer].units.size()].z);
				wildflag = false;
				dir = attackPos - position;
				target = attackPos - dir.normalize()*reichweite/2.0;
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
        
        vec = target - position;
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
            
            position = position + richtungsvektor*v*t;
            
            hastToCheckVisibility = true;
            
            awx_vector3f radius = target - position;
				
            if(vec.getLength()<v*t ||
			   (attacksth && attackIndex >= (int)player[attackPlayer].units.size() && attackIndex < (int)player[attackPlayer].units.size() + (int)player[attackPlayer].buildings.size()
			   && player[attackPlayer].buildings[attackIndex - (int)player[attackPlayer].units.size()].boundingCollision(position + richtungsvektor * reichweite)))
            {
				// ZIEL ERREICHT
                anvisiert = false;
				//dir.set(0,0,0);
                hasDir = false;
                
				if(attacksth||rohstoffabbau){
					data[0].gotoBezeichnung("stechen" , 7);
					//position = position - richtungsvektor * v*t/2.0;
				}
                else{
					wildflag = true;
					//position = target;
					data[0].gotoBezeichnung("stehen" , 10);
                }
            }
            
            
            int hitTriangle = level10.terrain.hit(position , position.y , ergebnisflag, currentSektorX, currentSektorZ);
			
			if(hitTriangle>=0)
			{
				level10.terrain.doFogOfWarForTriangleInSektor(hitTriangle,currentSektorX,currentSektorZ,position,1,5,0.9);
			}
		 }
    }
    modelview[12] = position.x;
    modelview[13] = position.y;
    modelview[14] = position.z;
}

void awx_building::calculateQue(){
	if(unitQue.size() > 0){
		player[playerIndex].units[unitQue[0]].fortschritt += ((1/player[playerIndex].units[unitQue[0]].bauzeit)/runtime.fps)*100.0;
			
		if(player[playerIndex].units[unitQue[0]].fortschritt >= 100){
			awx_matrix a;
			a.rotatef(winkel_y,0,1,0);
			// FAHNE
			if(fahne.x==AWXNull) player[playerIndex].units[unitQue[0]].target = awx_vector3f(x+RANDOM_FLOAT*2,y,z+RANDOM_FLOAT*2) + a.vectorProduct(awx_vector3f(0,0,10));
			else player[playerIndex].units[unitQue[0]].target = fahne;
		
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

void awx_building::buildUnit(char* name){
	bool flag = false;
	int index;
	int playerInd;
	
	for(int u = 0; u < (int)player.size();u++){
		for(int i = 0; i < (int)player[u].units.size();i++){
			if(strcmp(player[u].units[i].workingdirectory.lastPathComponent().cString() , name) == 0){
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
		tempu.openFile(("Data/units/" + awx_string(name) + "/zusammenbau.aax").cString());
		tempu.loadBlobs("Data/textures/blob.tga",1.2);
		tempu.position.set(x,y,z);
		tempu.initPosition();
		
		player[playerIndex].units.push_back(tempu);
		
		player[playerIndex].units[player[playerIndex].units.size()-1].position.x=x;
		player[playerIndex].units[player[playerIndex].units.size()-1].position.y=y;
		player[playerIndex].units[player[playerIndex].units.size()-1].position.z=z;

		player[playerIndex].units[(int)player[playerIndex].units.size()-1].anvisiert = false;
		player[playerIndex].units[(int)player[playerIndex].units.size()-1].hasDir = false;
		player[playerIndex].units[(int)player[playerIndex].units.size()-1].myIndex = (int)player[playerIndex].units.size()-1;
		
		for(int i = 0; i < (int)namen.size();i++){
			if(strcmp(namen[i].value , name) == 0){
				player[playerIndex].units[player[playerIndex].units.size()-1].myButtonID = buttons[i].texID;
				break;
			}
		}
		if(playerIndex == controlIndex){
			int tempInt = controls.selection[0];
			controls.selection.clear();
			controls.selection.push_back(tempInt+1);
		}
	}
	else
	{
		tempu.reinit(player[playerInd].units[index]);
		player[playerIndex].units.push_back(tempu);
		
		player[playerIndex].units[player[playerIndex].units.size()-1].blobtexturID = player[playerInd].units[index].blobtexturID;
		player[playerIndex].units[player[playerIndex].units.size()-1].blobsizex = player[playerInd].units[index].blobsizex;
		player[playerIndex].units[player[playerIndex].units.size()-1].blobsizey=player[playerInd].units[index].blobsizey;
		player[playerIndex].units[player[playerIndex].units.size()-1].position.x=x;
		player[playerIndex].units[player[playerIndex].units.size()-1].position.y=y;
		player[playerIndex].units[player[playerIndex].units.size()-1].position.z=z;
		player[playerIndex].units[player[playerIndex].units.size()-1].anvisiert = false;
		player[playerIndex].units[player[playerIndex].units.size()-1].hasDir = false;
		
		player[playerIndex].units[player[playerIndex].units.size()-1].buttons = player[playerInd].units[index].buttons;
		player[playerIndex].units[player[playerIndex].units.size()-1].attributes = player[playerInd].units[index].attributes;
		player[playerIndex].units[player[playerIndex].units.size()-1].workingdirectory = player[playerInd].units[index].workingdirectory;
		player[playerIndex].units[player[playerIndex].units.size()-1].namen = player[playerInd].units[index].namen;
		player[playerIndex].units[player[playerIndex].units.size()-1].myIndex = (int)player[playerInd].units.size()-1;
		
		for(int i = 0; i < (int)namen.size();i++){
			if(strcmp(namen[i].value , name) == 0){
				player[playerIndex].units[player[playerIndex].units.size()-1].myButtonID = buttons[i].texID;
				break;
			}
		}
		if(playerIndex == controlIndex){
			int tempInt = controls.selection[0];
			controls.selection.clear();
			controls.selection.push_back(tempInt+1);
		}
	}
	
	unitQue.push_back((int)player[playerIndex].units.size()-1);
}

