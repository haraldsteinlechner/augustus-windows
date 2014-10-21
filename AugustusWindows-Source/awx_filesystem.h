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

#ifndef AWX_FILESYSTEM_H
#define AWX_FILESYSTEM_H


#include <iostream>
#include <vector>
using namespace std;

#include "awx_string.h"

vector <awx_string> deleteEntriesContaining( vector <awx_string> source , char delimiter);

vector <awx_string> deleteEntriesNotContaining( vector <awx_string> source , char delimiter);

int subpaths(char *dir , vector <awx_string> &elements);

int subdirs(char* dir , vector <awx_string> &elements);

int subfiles(char* dir , vector <awx_string> &elements);

int subpaths(awx_string dir , vector <awx_string> &elements);

int subfiles(awx_string dir , vector <awx_string> &elements);

int subdirs(awx_string dir , vector <awx_string> &elements);

#endif