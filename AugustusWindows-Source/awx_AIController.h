class buildingReihung
{
public:
	
	int lenght;
	int index;

	friend bool operator>(const buildingReihung &u, const buildingReihung &v);
	friend bool operator<(const buildingReihung &u, const buildingReihung &v);
};

bool operator>(const buildingReihung &u, const buildingReihung &v)
{
	return u.lenght<v.lenght;
}

bool operator<(const buildingReihung &u, const buildingReihung &v)
{
	return u.lenght>v.lenght;
}

int awx_player::buildBuilding(void *data)
{
	awx_string *argument = (awx_string*) data;
	
	cout << "build building" << *argument << endl;
	
	
	int i=0;
	for(i=0;i<units.size();i++)
	{
		if(units[i].meinName=="arbeiter"&&units[i].fortschritt>=100)
		{
			if(units[i].workingOnBuildingID==-1&&units[i].myCurrentRessourceID==-1)
			{
				break;
			}
		}
	}
	
	if(i>units.size()-1)
	{
		cout << "Warte auf einheitfertigstellung..." << endl;

		return 0;
	}

	awx_vector3f housePosition;
	myGrid.calculateNextFreeBlock(housePosition);
		
	units[i].placeBuilding(argument->cString(),true);

	bool ergebnisflag=false;
	int currentSektorX=-1, currentSektorZ=-1;
    int hitTriangle = currentLevel->terrain.hit(housePosition , housePosition.y , ergebnisflag, currentSektorX, currentSektorZ);
			
	if(hitTriangle<0) cout << "ERROR: Fehler bei Kolissionsberechnung für Hausplatzierung" << endl;

	units[i].buildBuilding(housePosition,true,i);

	return 0;
}

int awx_player::idle(void *data)
{
	cout << "idle" << endl;

	return 0;
}

int awx_player::buildUnit(void *data)
{
	awx_string *argument = (awx_string*) data;

	cout << "Build unit: " << *argument << endl;

	vector<buildingReihung> faehigeBuildings;
	buildingReihung tempReihung;

	for(int i=0;i<buildings.size();i++)
	{
		for(int u=0;u<buildings[i].namen.size();u++)
		{
			if(buildings[i].namen[u]==*argument)
			{
				tempReihung.index=i;
				tempReihung.lenght = buildings[i].unitQue.size();
				faehigeBuildings.push_back(tempReihung);
			}
		}
	}

	if(faehigeBuildings.size()>0)
	{
		sort(faehigeBuildings.begin(),faehigeBuildings.end());
		int indexOfBuilding = faehigeBuildings[0].index;

		if(indexOfBuilding>=0&&indexOfBuilding<buildings.size())
			buildings[indexOfBuilding].buildUnit(argument->cString(),false);
		else cout << "Fehler beim bauen einer einheit, da bauendes Haus nicht existiert." << endl;

	} else cout << "Kein Building für Produktion von: " << *argument << " gefunden." << endl;



	
	return 0;
}

int awx_player::baueRohstoffeAb(void *data)
{
	awx_string *argument = (awx_string*) data;
	
	if(argument==NULL) cout << "ERROR: Function Dictionary interpreter (argument is NULL)" << endl;
	
	cout << "Rohstoffabbau von: " << *argument << endl;
	
	cout << "Freien Sklaven finden..." << endl;
	
	int i=0;
	for(i=0;i<units.size();i++)
	{
		if(units[i].meinName=="arbeiter"&&units[i].fortschritt>=100)
		{
			if(units[i].workingOnBuildingID==-1&&units[i].myCurrentRessourceID==-1)
			{
				break;
			}
		}
	}
	
	if(i>units.size()-1)
	{
		cout << "Warte auf einheitfertigstellung..." << endl;
		return 0;
	}

	cout << "Freier Sklave:" << i << endl;


	cout << "Rohstoff identifizieren..." << endl;
	int rohstoff=-1;
		for(int i=0;i<rohstoffe.size();i++)
		{
			if(rohstoffe[i].text==*argument)
			{
				rohstoff=i;
			}
		}
	
	if(rohstoff==-1)
	{
		cout << "Rohstoff konnte nicht identifiziert werden." << endl;
		return -1;
	}

	
	cout << "Rohstoffquelle finden..." << endl;
	int nextTree=-1;
	if((nextTree=units[i].findRessourceWithID(rohstoff))==-1)
	{
		cout << "Kein passendes Resourceobjekt gefunden." << endl;
	}
	awx_vector3f ziel = awx_vector3f(currentLevel->data[nextTree].x,currentLevel->data[nextTree].y,currentLevel->data[nextTree].z);
	cout << "Rohstoffziel: " << ziel << endl;
	 
	units[i].setTarget(ziel,nextTree);

	//units[i].regnerateCorrectPath();
	
	kannKaserneBauen=2;
	
	return 0;
}