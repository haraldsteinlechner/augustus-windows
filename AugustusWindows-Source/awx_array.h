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

#include <iostream>
#include <vector>
using namespace std;

#include "awx_string.h"

#ifndef AWX_ARRAY_H
#define AWX_ARRAY_H

class awx_array
{
	public:
		vector <awx_string> values;
		vector <int> typ;
		
		void readline(FILE* file , char* str);
		template <class a> void add(a value);
		void set(awx_array a);
		void set(vector <awx_string> a);
		template <class a> bool setObjectAtIndex(int index , a value);
		void writeToFile(char* path);		
		template <class a> bool initWithFile(a path);
		void alloc(int size);		
		awx_array plus(awx_string a);
		awx_array plus(awx_array a);
		awx_string getObjectAtIndex(int index);
		template <class a> awx_array &operator+=(a value);
		awx_string operator [] (int index);
};

template <class a> awx_array operator + (awx_array array, a value);
awx_array operator + (awx_array array, awx_array array2);
ostream &operator << (ostream & out, awx_array a);

#endif