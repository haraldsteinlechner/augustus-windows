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

#ifndef AWX_TIME_H
#define AWX_TIME_H

#include <iostream>
using namespace std;

class awx_time
{
	public:
	
	int jahr,monat,tag,stunde,minute,sekunde;
	long int last;
	long int temp;

	
	awx_time();
	awx_time(int seconds);
	void print();
	void update();
	string printToString();	
	// Gibt Zeit zurück (serialisierte zeit) [nid überkopfter vielleicht oberbledsinn
	long int getTime();
	int getDaytime();
	
	awx_time &operator-(awx_time b);
};


ostream & operator<<  (ostream & out, awx_time a);

#endif