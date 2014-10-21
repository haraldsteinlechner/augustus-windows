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

#include "awx_ki.h"

#ifdef _WIN32
#include "awx_fuzzy_rule_container.h"
#endif

#ifndef _WIN32
#include "awx_fuzzy_logic/fuzzy.h"
#endif

extern awx_fuzzy_rule_container *fuzzyLogicContainers;

awx_string replaceVariables(awx_string bedingung , awx_dictionary &variables)
{
	/*awx_string lookupBedingung = bedingung; 
	
	int i=0;
    int end=-1;
    for(i=0;i<bedingung.length();i++)
    {
        if(bedingung[i]=='$')
        {
            int u=i+1;
            for(;u<bedingung.length();u++)
            {
                char temp = bedingung[u];
                
                switch(temp)
                {
                    case ' ': end=u-1; break;
                    case '$': end=u-1; break;
                    case '=': end=u-1; break;
                    case '!': end=u-1; break;
                    case '<': end=u-1; break;
                    case '>': end=u-1; break;
                    default: end=-1; break;
                }
                if(end!=-1)
                {
                    break;
                }
            }
            if(end==-1)
            {
                end=u;
            }
            
            awx_string result;
            awx_string lookup=bedingung.substring(i,end);
			
			
            if((result=variables.getObjectForKey(lookup.cString()))=="")
            {
                cout << "KIInterpreterError in conditional:" << lookup << " is not known." << endl;
            }
            else
            {				
                string l = lookupBedingung.cString();
                l.erase(l.begin()+i,l.begin()+end+1);
                l.insert(i,result.cString());
                lookupBedingung=l.c_str();
            }
        }
    }
	
	return lookupBedingung;*/


	string lookupBedingung = bedingung.cString();

	bool nochzuErsetzen=true;
	
	char tokens[] = " =!><";
	int start,end;
	int result=0;

	do
	{

		start=lookupBedingung.find('$');

		if(start==-1) 
		{
			nochzuErsetzen=false;
			break;
		}

		int i;
		result = -1;
		for(i=0;i<strlen(tokens);i++)
		{
			// Gefunden
			string tempTest = lookupBedingung.substr(start,lookupBedingung.length());
			result = tempTest.find(tokens[i]);
			if(result!=-1)
			{
				end=result;
				break;
			}
		}

		if(result==-1)
		{
			end = lookupBedingung.length();
			nochzuErsetzen=false;
		}

		// Lookup
		string variable = lookupBedingung.substr(start,end);
		awx_string resultString;
		if((resultString=variables.getObjectForKey((char*)variable.c_str()))=="")
        {
			cout << "KIInterpreterError in conditional:" << variable << " is not known." << endl;
        }

		// Löschen
		lookupBedingung.erase(lookupBedingung.begin()+start,lookupBedingung.begin()+end);

		// Einfügen
		lookupBedingung = lookupBedingung.insert(start,resultString.cString());
	} while(nochzuErsetzen);

	return lookupBedingung;
}

bool vergleich(awx_string &mystring)
{
	float val1,val2;
	int ret = sscanf(mystring.cString(),"%f==%f",&val1,&val2);
	if(ret!=2)
	{
		ret = sscanf(mystring.cString(),"%f<%f",&val1,&val2);
		
		if(ret!=2)
		{
			ret = sscanf(mystring.cString(),"%f>%f",&val1,&val2);
			if(ret!=2)
			{
				ret = sscanf(mystring.cString(),"%f>=%f",&val1,&val2);
				if(ret!=2)
				{
					ret = sscanf(mystring.cString(),"%f<=%f",&val1,&val2);
					if(ret!=2)
					{
						ret = sscanf(mystring.cString(),"%f!=%f",&val1,&val2);
						if(ret!=2) 
						{
							ret = sscanf(mystring.cString(),"%f&&%f",&val1,&val2);
							if(ret != 2)
							{
								ret = sscanf(mystring.cString(),"%f||%f",&val1,&val2);
								if(ret == 2)return (val1 == 1 || val2 == 1);
							}
							else return (val1 == 1 && val2 == 1);
						}
						else return val1!=val2;
					}
					
					else return val1<=val2;
					
				} return val1>=val2;
			
			} else return val1>val2;
		} else return val1<val2;
	} else return val1==val2;
	
	return false;
}

bool ausdruck(awx_string &mystring)
{
	awx_string tempstr = mystring;
	//tempstr.set(mystring.cString());
	
	while(1)
	{
        int klammerOffen=0;
        int indexA=0;
        int indexE=0;
        bool klammerFlag=false;
        
        int i=0;
        for(i=0;i<tempstr.length();i++)
        {
            if(tempstr[i]=='(') 
            {
                klammerOffen++;
                if(klammerOffen==1) indexA=i;
                klammerFlag=true;
            }
            if(tempstr[i]==')')
            {
                klammerOffen--;
                indexE=i;
				//cout << "val = " << tempstr.value[i-1] << " index, " << indexE << " tempstr=" << tempstr << endl;
            }	
            
            if(klammerOffen==0&&klammerFlag)
            {
                awx_string newString = tempstr.substring(indexA+1,indexE-1);
				//cout << "str=" << newString << endl;
                
                int length = tempstr.length() - (indexE - indexA) + 1;
                char* tempChar = new char[length];
                int zaehler = 0;
                
                
				//cout << "Vorher: #" << newString << "#" << endl;
                for(int x = 0; x < indexA;x++)
                {
                    tempChar[zaehler] = tempstr.cString()[x];
                    zaehler++;
                }
				
                char* temp = new char[2];
                sprintf(temp , "%d" , ausdruck(newString));
                tempChar[zaehler] = temp[0];
                delete[] temp;
                zaehler++;
               // cout << "wildung=" << tempstr.cString()[indexE] << "  " << indexA << "   " << indexE<<endl;
                for(int x = indexE+1; x < tempstr.length();x++)
                {
                    tempChar[zaehler] = tempstr.cString()[x];
                    zaehler++;
                }
                
                tempChar[zaehler] = '\0';
                
                
                tempstr.set(tempChar);
                
			//	cout << "newString = #" << tempstr << "#" << endl;
				
                delete[] tempChar;
                
                klammerFlag= false;
				break;
                //ergebnisse.push_back(ausdruck(newString));
            }
		}
	
		if(!klammerFlag && i == tempstr.length())
		{
			return vergleich(tempstr);
		}
	}
	
}

awx_neuron::awx_neuron()
{
    bedingung = "null";
    script = "null";
    IDTrue=-1;
    IDFalse=-1;
    myID=-1;
}

void awx_neuron::processMe(awx_dictionary &dict, vector<awx_neuron> &neurons)
{
    if(bedingung.compare("null")==0)
    {
        if(script.compare("null")==0)
        {
            // do something
        } else 
        {
            // Script verarbeiten
            cout << "Script is:" << script << endl;
        }
        
        return;
    }
    
    awx_string lookupBedingung = bedingung; 
    
    int i=0;
    int end=-1;
    for(i=0;i<bedingung.length();i++)
    {
        if(bedingung[i]=='$')
        {
            int u=i+1;
            for(;u<bedingung.length();u++)
            {
                char temp = bedingung[u];
                
                switch(temp)
                {
                    case ' ': end=u-1; break;
                    case '$': end=u-1; break;
                    case '=': end=u-1; break;
                    case '!': end=u-1; break;
                    case '<': end=u-1; break;
                    case '>': end=u-1; break;
                    default: end=-1; break;
                }
                if(end!=-1)
                {
                    break;
                }
            }
            if(end==-1)
            {
                end=u;
            }
            
            awx_string result;
            awx_string lookup=bedingung.substring(i,end);
            if((result=dict.getObjectForKey(lookup.cString()))=="")
            {
                cout << "KIInterpreterError in conditional:" << lookup << " is not known." << endl;
            }
            else
            {				
                string l = lookupBedingung.cString();
                l.erase(l.begin()+i-1,l.begin()+end);
                l.insert(i,result.cString());
                lookupBedingung=l.c_str();
            }
        }
    }
    
    cout << "resultBed=" << lookupBedingung << endl;

    bool resultAusdruck=ausdruck(lookupBedingung);
    if(resultAusdruck&&IDTrue>0&&IDTrue<neurons.size())
    {
        neurons[IDTrue].processMe(dict,neurons);
    } else if(IDFalse>0&&IDFalse<neurons.size()) neurons[IDFalse].processMe(dict,neurons);
}

awx_network::awx_network()
{
    awx_neuron pushNeuron;
    pushNeuron.bedingung="$unitcount<6";
    pushNeuron.IDTrue=1;
    pushNeuron.IDFalse=2;
    pushNeuron.myID=0;
    neurons.push_back(pushNeuron);
    
    pushNeuron.bedingung="null";
    pushNeuron.script="print wildTrue";
    pushNeuron.IDTrue=-1;
    pushNeuron.IDFalse=-1;
    pushNeuron.myID=1;
    neurons.push_back(pushNeuron);
    
    pushNeuron.bedingung="null";
    pushNeuron.script="print wildFalse";
    pushNeuron.IDTrue=-1;
    pushNeuron.IDFalse=-1;
    pushNeuron.myID=2;
    neurons.push_back(pushNeuron);
}

awx_network::~awx_network()
{
    neurons.clear();
}

int awx_network::processNeuron(int ID, awx_dictionary &myDict)
{
    if(ID<0||ID>=neurons.size()) return -1;
    
    neurons[ID].processMe(myDict,neurons);
    
    return 0;
}


void initKI()
{
	const int fuzzySetMax=10;
	fuzzyLogicContainers = new awx_fuzzy_rule_container[fuzzySetMax];
	for(int i = 0; i < fuzzySetMax;i++)
	{
		fuzzyLogicContainers[i].debug = false;
	}
	
	
	fuzzyLogicContainers[0].readFromFile("Data/ki/bauen.afsx");
	fuzzyLogicContainers[1].readFromFile("Data/ki/attractivity.afsx");
}

void deallocKI()
{
	delete[] fuzzyLogicContainers;
}


