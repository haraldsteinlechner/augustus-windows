


void initBerufe()
{
	produkte.push_back("Schwert");
	produkte.push_back("Brot");
	produkte.push_back("Bier");
	produkte.push_back("Helm");
	produkte.push_back("Schild");
}

class awx_beruf
{
	public:
		awx_string name;					//Name des Berufes
		awx_string description;				//Beschreibung des Berufes
		
		float einkommen;					//Einkommen pro Monat												[0-1000]
		float anstrengung;					//Masszahl für die Anstrengung die der Beruf mit sich bringt		[0-1]
		
		float startArbeit;					//Arbeitszeitbeginnzeit												[5-12]
		float arbeitsdauer;					//Arbeitsdauer pro tag												[0-12]
		
		int buildingIndex;					//In welchem Haus existiert dieser beruf
		int unitIndex;						//Welche Einheit hat diesen Beruf
		
		
		vector <int> produktIndices;		//Welche Produkte werden hergestellt
		vector <int> produktMenge;			//Analog zu produktIndices ... Wie viel des Produktes wird pro Arbeitsstunde hergestellt
		
		
		awx_beruf()
		{
			name = "";
			description = "";
			
			einkommen = 0;
			anstrengung = 0;
			
			startArbeit = 0;
			arbeitsdauer = 0;
			
			buildingIndex = -1;
			unitIndex = -1;
			
			produktIndices.clear();
			produktMenge.clear();
		}
		
		awx_string toString()
		{
			char* produktestr = new char[256];
			produktestr[0] = '\0';
			for(int i = 0; i < produktIndices.size();i++)
			{
				sprintf(produktestr , "%s%s:%d#" , produktestr , produkte[produktIndices[i]].cString() , produktMenge[i]);
			}
		
			char* temp = new char[512];
			sprintf(temp , "%s;%s;einkommen:%f;anstrengung:%f;startArbeit:%f;arbeitsdauer:%f;%s" , name.cString() , 
																									description.cString() , 
																									einkommen ,
																									anstrengung ,
																									startArbeit ,
																									arbeitsdauer , 
																									produktestr);
		
			awx_string retstr = temp;
			delete[] temp;
			delete[] produktestr;
					
			return retstr;																																																																																							
		}
		
		void interpretString(awx_string &str)
		{
			produktIndices.clear();
			produktMenge.clear();
			
			vector <awx_string> parts;
			str.splitExact(';',parts);

			if(parts.size() == 8)
			{
				name = parts[0];
				description = parts[1];
				sscanf(parts[2].cString() , "einkommen:%f" , &einkommen);
				sscanf(parts[3].cString() , "anstrengung:%f" , &anstrengung);
				sscanf(parts[4].cString() , "startArbeit:%f" , &startArbeit);
				sscanf(parts[5].cString() , "arbeitsdauer:%f" , &arbeitsdauer);
				
				vector <awx_string> parts2;
				parts[6].splitExact('#' , parts2);
				
				for(int i = 0; i < parts2.size()-1;i++)
				{
					vector <awx_string> parts3;
					parts2[i].split(':' , parts3);
					
					setProdukt(parts3[0],parts3[1].floatValue());
				}
			}
		}
		
		bool isVergeben()
		{
			return (unitIndex != -1);
		}
		
		float getAttractivity()
		{	
			if(fuzzyLogicContainers == NULL)return 0;
			fuzzyLogicContainers[1].setValue("Arbeitsbeginn",startArbeit);
			fuzzyLogicContainers[1].setValue("Arbeitszeit",arbeitsdauer);
			fuzzyLogicContainers[1].setValue("Anstrengung",anstrengung);
			fuzzyLogicContainers[1].setValue("Einkommen",einkommen);
			
			vector <awx_string> namen;
			vector <float> values;
			
			fuzzyLogicContainers[1].calculate(namen , values);
			
			if(values.size() > 0)
			{
				return values[0];
			}
			else return 0;
		}
		
		void setProdukt(awx_string name , float menge)
		{
			int index = -1;
			for(int i = 0; i < produkte.size();i++)
			{
				if(produkte[i] == name)
				{
					index = i;
					break;
				}
			}
			if(index > -1)
			{
				produktIndices.push_back(index);
				produktMenge.push_back(menge);
			}
			else cout << "Produkt \"" << name << "\" konnte nicht gefunden werden" << endl;
		}
		
		void setProdukt(int index , float menge)
		{
			if(index > -1 && index < produkte.size())
			{
				produktIndices.push_back(index);
				produktMenge.push_back(menge);
			}
			else cout << "Produkt[" << index << "] konnte nicht gefunden werden" << endl;
		}
};