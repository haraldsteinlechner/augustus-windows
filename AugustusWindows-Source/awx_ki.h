///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   ¬© Alphaworx Corporation//  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////

/*					                   KI  	                  				 */

#ifndef AWX_KI_H
#define AWX_KI_H

#include "awx_string.h"
#include "awx_dictionary.h"


typedef enum
{
	AWXLogicalAND,
	AWXLogicalOR,
	AWXLogicalError
} AWXLogical;



awx_string replaceVariables(awx_string bedingung , awx_dictionary &variables);
bool vergleich(awx_string &mystring);
bool ausdruck(awx_string &mystring);

class awx_neuron
{
	public:
		awx_string bedingung;
		awx_string script;
		int IDTrue,IDFalse;
		int myID;
		
		
		awx_neuron();
		
		/*
		~awx_neuron()
		{
		}*/
		
		void processMe(awx_dictionary &dict, vector<awx_neuron> &neurons);
};

class awx_network
{
	public:
	
		vector<awx_neuron> neurons;
	
		awx_network();
		
		~awx_network();
		
		int processNeuron(int ID, awx_dictionary &myDict);
};


void initKI();
void deallocKI();
//void idleKI();

#endif