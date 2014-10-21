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

#include "awx_time.h"

awx_time::awx_time()
{
    jahr = -768;
    monat = 6;
    tag = 12;
    stunde = 12;
    minute = 14;
    sekunde = 23;
}

awx_time::awx_time(int seconds)
{
	monat = seconds / 1036800;
	seconds = seconds % 1036800;
	
	tag = seconds / 86400;
	seconds = seconds % 86400;
	
	stunde = seconds / 3600;
	seconds = seconds % 3600;
	
	minute = seconds / 60;
	seconds = seconds % 60;
	
	sekunde = seconds;
	
	jahr = -768;
}

void awx_time::print()
{
    cout << jahr << "  " << tag << "/" << monat << "  " << stunde << ":" << minute << ":" << sekunde << endl;
}


void awx_time::update()
{
        // Vorübergehnde möglicherweise falsch und nid gehirnet lösung
        
        last = temp;
        sekunde++;
        if(sekunde>60)
        {
            minute++;
            sekunde = 1;
            if(minute>60)
            {
                stunde++;
                minute = 0;
                if(stunde>24)
                {
                    tag++;
                    stunde=0;
                    
                    if(tag>31)
                    {
                        monat++;
                        tag=1;
                        
                        if(monat>12)
                        {
                            jahr++;
                            monat=1;
                        }
                    }
                }
            }
        }
}

string awx_time::printToString()
{
    char ret[80];
    sprintf(ret,"%i  %i/%i %i:%i:%i",jahr,tag,monat,stunde,minute,sekunde);
    
    return string(ret);
}

awx_time &awx_time::operator-(awx_time b)
{
	return *(new awx_time(this->getTime() - b.getTime()));
}


// Gibt Zeit zurück (serialisierte zeit) [nid überkopfter vielleicht oberbledsinn
long int awx_time::getTime()
{
    return monat * 1036800 + tag * 86400 + stunde * 3600 + minute * 60 + sekunde;
}

int awx_time::getDaytime()
{
	return stunde*3600 + minute*60 + sekunde;
}

ostream & operator<<  (ostream & out, awx_time a){
	a.print();
	return out;
}