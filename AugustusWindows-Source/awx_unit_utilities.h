typedef enum awx_unit_mode
{
	IDLE,
	ATTACKING,
	RUNNING,
	HARVESTING,
	BUILDING,
	WORKING
};

class awx_unit_modifiers
{
	public:
		//Lebenspunkte:
		float max_leben;
		
		//Energiepunkte:
		float max_mana;

		//Verteidigung:
		float verteidigung;
		
		//Angriff:
		float angriffskraft;
		
		float staerke;
		float geschick;
		float wendigkeit;
		
		float reichweite;
		
		float schaden;
		float geschwindigkeit;
		
		float min_schaden;
		
		float sichtweite;
		
		float min_abbauspeed;
		float abbauspeed;
		int rucksackgroesse;
		
		
		awx_unit_modifiers()
		{
			max_leben = 0;
			
			max_mana = 0;
			
			verteidigung = 0;
			
			angriffskraft = 0;
			
			staerke = 0;
			geschick = 0;
			wendigkeit = 0;
			
			reichweite = 0;
			
			schaden = 0;
			geschwindigkeit = 0;
			
			min_schaden = 0;
			
			sichtweite = 0;
			
			min_abbauspeed = 0;
			abbauspeed = 0;
			rucksackgroesse = 0;

		}
};

class awx_unit_values
{
	friend class awx_unit;
	public:
		vector <awx_string> rohstoffe;
	
		//Lebenspunkte:
		float max_leben;
		float leben;
		
		//Energiepunkte:
		float max_mana;
		float mana;

		//Verteidigung:
		float verteidigung;
		
		//Angriff:
		float angriffskraft;
		
		float staerke;
		float geschick;
		float wendigkeit;
		
		float reichweite;
		
		float schaden;
		float geschwindigkeit;
		
		float min_schaden;
		
		//Allgemeine Eigenschaften:
		float sichtweite;
		float bauzeit;
		
		//Rohstoffabbau:
		float min_abbauspeed;
		float abbauspeed;
		float getrageneRohstoffe;
		int rucksackgroesse;
		
		//Wirtschaftliche Faktoren:
		float geld;
		int wohnhausIndex;
		float durst;
		
		awx_time schlafenszeit;
		
		
		//Equiptment:
		vector <awx_unit_modifiers> modifiers;
		
		
		void setDictionary(awx_dictionary &dict)
		{
			awx_string tempstr = dict.getObjectForKey("rohstoffe");
			tempstr.split(';',rohstoffe);
			
			max_leben			=	dict.getObjectForKey("Lebenspunkte").floatValue();
			if(leben > max_leben)leben = max_leben;
			
			verteidigung		=	dict.getObjectForKey("Ruestung").floatValue();
			angriffskraft		=	dict.getObjectForKey("Angriffswert").floatValue();
			
			bauzeit				=	dict.getObjectForKey("Bauzeit").floatValue();
			geschwindigkeit		=	dict.getObjectForKey("Geschwindigkeit").floatValue();
			
			sichtweite			=	dict.getObjectForKey("Sichtweite").floatValue();
		}
		
		awx_unit_values()
		{
			max_leben = 400;
			leben = max_leben;
			
			max_mana = 250;
			mana = max_mana;
			
			verteidigung = 200;
			
			angriffskraft = 300;
			
			staerke = 15;
			geschick = 11;
			wendigkeit = 9;
			
			reichweite = 1;
			
			schaden = 20;
			geschwindigkeit = 1.5;
			
			min_schaden = 0.6;
			
			sichtweite = 5;
			bauzeit = 4;
			
			abbauspeed = 14;
			min_abbauspeed = 0.6;
			getrageneRohstoffe = 0;
			rucksackgroesse = 130;
			
			
			wohnhausIndex = -1;
			
			geld = 0;
			
			durst = 0;
			
			schlafenszeit = awx_time(0);
			
			float hour = 22 + (RANDOM_FLOAT - 0.5)*2*2;
			schlafenszeit.stunde = hour;
			hour -= schlafenszeit.stunde;
			
			schlafenszeit.minute = hour*60;
			hour -= schlafenszeit.minute/60.0;
			
			schlafenszeit.sekunde = hour*3600;
						
			modifiers.clear();
			rohstoffe.clear();
		}
		
		awx_unit_values(const awx_unit_values &x)
		{
			max_leben = x.max_leben;
			
			max_mana = x.max_mana;
			
			verteidigung = x.verteidigung;
			
			angriffskraft = x.angriffskraft;
			
			staerke = x.staerke;
			geschick = x.geschick;
			wendigkeit = x.wendigkeit;
			
			reichweite = x.reichweite;
			
			schaden = x.schaden;
			geschwindigkeit = x.geschwindigkeit;
			
			min_schaden = x.min_schaden;
			
			sichtweite = x.sichtweite;
			bauzeit = x.bauzeit;
			
			abbauspeed = x.abbauspeed;
			min_abbauspeed = x.min_abbauspeed;
			rucksackgroesse = x.rucksackgroesse;
			getrageneRohstoffe = 0;
			
			geld = 0;
			durst = 0;
			
			wohnhausIndex = x.wohnhausIndex;
			
			float hour = 22 + (RANDOM_FLOAT - 0.5)*2*2;
			schlafenszeit.stunde = hour;
			hour -= schlafenszeit.stunde;
			
			schlafenszeit.minute = hour*60;
			hour -= schlafenszeit.minute/60.0;
			
			schlafenszeit.sekunde = hour*3600;
			
			rohstoffe.clear();
			for(int i = 0; i < x.rohstoffe.size();i++)
			{
				rohstoffe.push_back(x.rohstoffe[i]);
			}
			
			modifiers.clear();
			for(int i = 0; i < x.modifiers.size();i++)
			{
				modifiers.push_back(x.modifiers[i]);
			}
		}
		
		awx_unit_values getValues()
		{
			awx_unit_values temp(*this);
			for(int i = 0; i < modifiers.size();i++)
			{
				temp.max_leben += modifiers[i].max_leben;
				
				temp.max_mana += modifiers[i].max_mana;
				
				temp.verteidigung += modifiers[i].verteidigung;
				
				temp.angriffskraft += modifiers[i].angriffskraft;
				
				temp.staerke += modifiers[i].staerke;
				temp.geschick += modifiers[i].geschick;
				temp.wendigkeit += modifiers[i].wendigkeit;
				
				temp.schaden += modifiers[i].schaden;
				temp.geschwindigkeit += modifiers[i].geschwindigkeit;
				
				temp.sichtweite += modifiers[i].sichtweite;
				
				temp.min_schaden += modifiers[i].min_schaden;
			}
			
			return temp;
		}
		
		void addModifier(const awx_unit_modifiers &mod)
		{
			modifiers.push_back(mod);
		}
		
		float calcSchaden(awx_unit_values &otherUnit)
		{
			awx_unit_values temp = getValues();
			awx_unit_values temp2 = otherUnit.getValues();
			
			float percentStaerke = temp.staerke/150.0;
			float percentGeschick = temp.geschick/150.0;
			
			float percentVerteidigung = temp2.verteidigung/1200.0;
			float percentWendigkeit = temp2.wendigkeit/150.0;
			
			float trefferwahrscheinlichkeit = 1 - (percentVerteidigung + percentWendigkeit) + percentGeschick;
			
			if(RANDOM_FLOAT < trefferwahrscheinlichkeit)
			{
				return temp.schaden * (1 + percentStaerke) *(temp.min_schaden + RANDOM_FLOAT*(1-temp.min_schaden));
			}
			else
			{
				return 0;
			}
		}
		
		float calcHouseSchaden()
		{
			awx_unit_values temp = getValues();
			float percentStaerke = temp.staerke/100.0;
			float percentGeschick = temp.geschick/150.0;
			
			float trefferwahrscheinlichkeit = percentGeschick + 2*percentStaerke;
			//cout << "Haus: " << trefferwahrscheinlichkeit << endl;
			if(RANDOM_FLOAT < trefferwahrscheinlichkeit)
			{
				return temp.schaden * (1 + percentStaerke) *(temp.min_schaden + RANDOM_FLOAT*(1-temp.min_schaden));
			}
			else
			{
				return 0;
			}
		}
		
		float calcForce()
		{
			awx_unit_values temp = getValues();

			float percentStaerke = temp.staerke/150.0;
			float percentGeschick = temp.geschick/150.0;
			
			float trefferwahrscheinlichkeit = 1 + percentGeschick;
			
			return temp.schaden * (1 + percentStaerke) *((1+temp.min_schaden)/2.0) * trefferwahrscheinlichkeit;
		}
		
		void verdient(float betrag)
		{
			geld += betrag*(1-einkommenssteuer);
		}
	
};

class fuzzyLogicValueContainer
{
	public:
	
	awx_unit_values* unitValues;
	
	/////////////////////////////////////////////
    //	      	Muedigkeitsbehandlung		   //
    /////////////////////////////////////////////

	float muedigkeit;
	
	float dmuedigkeit_pos_building;
	float dmuedigkeit_pos_harvest;
	float dmuedigkeit_pos_running;
	float dmuedigkeit_pos_fighting;
	float dmuedigkeit_pos_working;
	
	float dmuedigkeit_neg;

	float getSpeed(awx_unit_mode mode)
	{
		float speed = 0;
    
        switch(mode)
        {
            case IDLE:
                speed = 0;
                break;
            
            case ATTACKING:
                speed = dmuedigkeit_pos_fighting;
                break;
            
            case RUNNING:
                speed = dmuedigkeit_pos_running;
                break;
            
            case HARVESTING:
                speed = dmuedigkeit_pos_harvest;
                break;
        
            case BUILDING:
                speed = dmuedigkeit_pos_building;
                break;
				
			case WORKING:
                speed = dmuedigkeit_pos_working;
                break;
        }
		
		return speed;
	}

    void increaseMuedigkeit(float t , awx_unit_mode mode)
    {
		float speed = getSpeed(mode);
    
        if(muedigkeit + speed*t < 1)muedigkeit += speed*t;
        else muedigkeit = 1;
    }
	
	void increaseMuedigkeit(float t , awx_unit_mode mode , float faktor)
	{
		float speed = getSpeed(mode)*faktor;
    
        if(muedigkeit + speed*t < 1)muedigkeit += speed*t;
        else muedigkeit = 1;		
	}

    void decreaseMuedigkeit(float t)
    {
        if(muedigkeit - dmuedigkeit_neg*t > 0)muedigkeit -= dmuedigkeit_neg*t;
        else muedigkeit = 0;
    }
	
	void processMuedigkeit(float t , awx_unit_mode mode)
	{
		
		if(mode == IDLE)
		{
			//cout << "weniger" << endl;
			decreaseMuedigkeit(t);
		}
		else
		{
			//cout << "mode: " << mode << endl;
			increaseMuedigkeit(t,mode);
		}
	}

	/////////////////////////////////////////////
    //	      	Motivation & Wetter            //
    /////////////////////////////////////////////
	
	float motivation;
	float wetter;
	
	
	/////////////////////////////////////////////
    //	      	Baugeschwindigkeit  		   //
    /////////////////////////////////////////////
		
	float baugeschwindigkeit;
	
	float calcBaugeschwindigkeit()
	{
		fuzzyLogicContainers[0].setValue("muedigkeit" , muedigkeit);
		fuzzyLogicContainers[0].setValue("motivation" , motivation);
		fuzzyLogicContainers[0].setValue("wetter" , wetter);
		
		vector <awx_string> namen;
		vector <float> werte;
		
		fuzzyLogicContainers[0].calculate(namen , werte);
		
		if(namen.size() > 0 && werte.size() > 0)
		{
			baugeschwindigkeit = werte[0];
			werte.clear();
			namen.clear();
			return baugeschwindigkeit;
		}
		else
		{
			baugeschwindigkeit = 1;
			return 1;
		}
	}
	
	float calcAbbauspeed()
	{
		if(unitValues != NULL)
		{
			float tempf = calcBaugeschwindigkeit();
			baugeschwindigkeit = unitValues->min_abbauspeed + tempf*(1-unitValues->min_abbauspeed);
			return baugeschwindigkeit;
		}
		else
		{
			return 0;
		}
	}
	
	
	/////////////////////////////////////////////
    //	      	Konstruktoren:				   //
    /////////////////////////////////////////////
	
	fuzzyLogicValueContainer()
	{
		muedigkeit=0;
		
		dmuedigkeit_pos_building = 0.06;
		dmuedigkeit_pos_harvest = 0.04;
		dmuedigkeit_pos_running = 0.015;
		dmuedigkeit_pos_fighting = 0.06;
		dmuedigkeit_pos_working = 0.025;
		
		dmuedigkeit_neg = 0.06;
		
		baugeschwindigkeit = 1.0;
		
		motivation = 0.5;
		wetter = 0.5;
	}
	
	fuzzyLogicValueContainer(const fuzzyLogicValueContainer& x)
	{
		muedigkeit=0;
		
		dmuedigkeit_pos_building = x.dmuedigkeit_pos_building;
		dmuedigkeit_pos_harvest = x.dmuedigkeit_pos_harvest;
		dmuedigkeit_pos_running = x.dmuedigkeit_pos_running;
		dmuedigkeit_pos_fighting = x.dmuedigkeit_pos_fighting;
		dmuedigkeit_pos_working = x.dmuedigkeit_pos_working;
		
		dmuedigkeit_neg = x.dmuedigkeit_neg;
		
		baugeschwindigkeit = 1.0;
		
		motivation = 0.5;
		wetter = 0.5;
	}
	
};
