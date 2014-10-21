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

#ifndef AWX_CPP_UTILITIES_H
#define AWX_CPP_UTILITIES_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

typedef enum
{
	AWXx86,
	AWXppc
} AWXCPUType;


/*		C++ Utilties wie etwa LittleEndian to BigEndian Swapper				 */


// Spiegelt alle Ã¼bergebenen Bytes von pvData. Funktion aus gamedev.com (User: RedAnt@gmx.de). Also keine AWX Funktion
bool reverseByteOrder( void* pvData, unsigned int nUnitScale, unsigned int nUnitsInArray );

// Funktion wie fread nur wird auf WindowsTargets die ByteFolge gespiegelt
// um das BigEndian des PowerPCs in LittleEndian zu konvertieren

int awx_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int awx_fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream);
int awx_freadT(void *ptr, size_t size, size_t nmemb, FILE *stream);
int awx_fread(void *ptr, size_t size, size_t nmemb, FILE *stream , bool inverted);

AWXCPUType getCPUArchitecture();

#endif
