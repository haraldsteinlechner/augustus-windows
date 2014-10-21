void awx_player::initAIStrategies()
{
	cout << "Init AI Strategies..." << endl;

	// KI-Strategien Laden
	isAIControlled = true;
	
	data = new awx_dictionary;
	AIStrategies = new awx_decision_network;
	
	
	string filename = "Data/ki/computergegner/main.alx";
	FILE *file = fopen((char*) filename.c_str(),"rb");
	
	if(file != NULL)
	{
		int temp  = 0;
		awx_fread(&temp,sizeof(temp),1,file);
		awx_vector3f *points = new awx_vector3f[temp];
			
		for(int i = 0; i < temp;i++)
		{
			awx_fread(&points[i].x , sizeof(points[i].x) , 1 , file);
			awx_fread(&points[i].y , sizeof(points[i].y) , 1 , file);
			cout << points[i] << endl;
		}
		AIStrategies->readFromFile(file);
		fclose(file);
		
		cout << "Strategies loaded" << endl;
		
	} else cout << "Computergegnerstrategies no found. " << filename << endl;
	
	
}

void awx_player::deallocAIStrategies()
{
	if(AIStrategies!=NULL && isAIControlled)
	
	{
		delete AIStrategies;
		delete data;
	}
}


void awx_player::processAIStrategies()
{
	if(myPlayerIndex==0||myPlayerIndex==-1) return;
	//if(!isAIControlled&&myPla) return;

	cout << "Calculating Strategy for player: " << myPlayerIndex << endl;
	
	
	
	// -------- Init Functions
	
	dictKeys.clear();
	dictFunctions.clear();
	
	addFunctionWithKey(&awx_player::buildBuilding,"buildBuilding");
	addFunctionWithKey(&awx_player::idle,"idle");
	addFunctionWithKey(&awx_player::buildUnit,"buildUnit");
	addFunctionWithKey(&awx_player::baueRohstoffeAb,"abbau");
	
	// -------- Init Functions
	

	
	// -------- Init Dictionary
	
	int sklaven=0;
	int soldaten=0;
	int einwohner=0;
	for(int i=0;i<units.size();i++)
	{
		if(units[i].meinName=="arbeiter") sklaven++;
		else if(units[i].meinName=="soldat") soldaten++;
		else if(units[i].meinName=="buerger") einwohner++;
	}
	
	int kasernen=0;
	int wohnungen=0;
	int wirtshaus=0;
	int brunnen=0;
	
	for(int i=0;i<buildings.size();i++)
	{
		if(buildings[i].meinName=="kaserne")
		{
			kasernen++;
		}
		else if(buildings[i].meinName=="familienhaus") wohnungen++;
		else if(buildings[i].meinName=="brunnen") brunnen++;
		else if(buildings[i].meinName=="wirtshaus") wirtshaus++;
	}

	int wohnungenSoll = 5;		// errechnen
	int einwohnerSoll = 5;		// erfuzzln
	int wirtshausSoll = 1;
	int brunnenSoll = 2;

	int kannHausBauen = 1;
	
	data->clear();
	data->setObject(rohstoffvalues[AWXHolz],"$holz");
	data->setObject(rohstoffvalues[AWXGold],"$gold");
	data->setObject(rohstoffvalues[AWXSteine],"$steine");
	data->setObject(rohstoffvalues[AWXNahrung],"$nahrung");

	data->setObject(rohstoffvalues[AWXHolz]*2,"$holzSoll");
	data->setObject(rohstoffvalues[AWXGold]*2,"$goldSoll");
	data->setObject(rohstoffvalues[AWXSteine]*2,"$steineSoll");
	data->setObject(rohstoffvalues[AWXNahrung]*2,"$nahrungSoll");

	data->setObject(kasernen,"$kasernen");
	data->setObject(kannKaserneBauen,"$aKaserne");
	data->setObject(sklaven,"$sklaven");
	data->setObject(soldaten,"$soldaten");

	data->setObject(einwohner,"$einwohner");
	data->setObject(einwohnerSoll,"$einwohnerSoll");
	data->setObject(wohnungen,"$wohnungen");
	data->setObject(wohnungenSoll,"$wohnungenSoll");
	data->setObject(kannHausBauen,"$aHaus");

	data->setObject(brunnen,"$brunnen");
	data->setObject(brunnenSoll,"$brunnenSoll");
	data->setObject(wirtshaus,"$wirtshaus");
	data->setObject(wirtshausSoll,"$wirtshausSoll");


	// -------- Init Dictionary		
	
	
	

	AIStrategies->setDictionary(*data);
	
	
	AICommands.clear();
	awx_string command,arguments,objective,resultStrategy;
	awxAICommand temp;
	
	objective = "heer";
	
	resultStrategy = AIStrategies->getFullScriptFor(objective);
	cout << "Strategiescript-result(" << objective << "): " << resultStrategy << endl;

	if(resultStrategy.length()<2)
	{
		objective = "stadt";
		resultStrategy = AIStrategies->getFullScriptFor(objective);
		cout << "Strategiescript-result(" << objective << "): " << resultStrategy << endl;
	}

	resultStrategy.parseFunctionSyntax(command,arguments);

	temp.command = command;
	temp.arguments = arguments;
	AICommands.push_back(temp);


	objective = "mining";
	resultStrategy = AIStrategies->getFullScriptFor(objective);
	resultStrategy.parseFunctionSyntax(command,arguments);
	cout << "Strategiescript-result(" << objective << "): " << resultStrategy << endl;

	if(resultStrategy.length()>2)
	{
		temp.command = command;
		temp.arguments = arguments;
		AICommands.push_back(temp);
	}
}

void awx_player::runAICommandList()
{
	if(myPlayerIndex==0||myPlayerIndex==-1) return;
	
	for(int i=0;i<AICommands.size();i++)
	{
		executeFunctionWithKey(AICommands[i].command, (void*) &AICommands[i].arguments);
	}
	
	AICommands.clear();
}


void awx_player::addFunctionWithKey(int (awx_player::*fp)(void* data), awx_string key)
{
	dictKeys.push_back(key);
		
	simplePlayerFunction tempFunctionStruct;
	tempFunctionStruct.fp = fp;
	tempFunctionStruct.me = this;
	dictFunctions.push_back(tempFunctionStruct);
}

int awx_player::executeFunctionWithKey(awx_string key, void *data)
{
	for(int i=0;i<dictKeys.size();i++)
	{
		if(key==dictKeys[i])
		{	
			return executeFunctionWithIndex(i,data);
		}
	}
		
	cout << "Requested Function for key: " << key << " not found." << endl;
	return -1;
}

int awx_player::executeFunctionWithIndex(int index, void *data)
{
	if(index<0||index>=dictFunctions.size()) 
	{
		cout << "Function of functionDictionary out of bounds. Stop." << endl;
		return -1;
	}
		
	return dictFunctions[index].exec(data);
}
		