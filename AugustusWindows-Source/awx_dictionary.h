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

#ifndef AWX_DICTIONARY_H
#define AWX_DICTIONARY_H

#include <iostream>
#include <vector>
using namespace std;

#include "awx_string.h"
#include "awx_vector.h"

class awx_dictionary{
	public:
		vector <awx_string> values;
		vector <awx_string> keys;
		vector <int> typ;
		
		void set(awx_string value , awx_string key , int typeID);
		void readline(FILE* file , char* str);
		
		void setObject(awx_vector3f value,char*key)
		{
			char unsafe[129];
			
			sprintf(unsafe,"%.3f/%.3f/%.3f",value.x,value.y,value.z);
			
			set(awx_string(unsafe),awx_string(key),7);
		}
	
		template <class a> void setObject(a value , char* key)
		{
		    int typeId;
    
            if(typeid(a) == typeid(int)){
                typeId = 1;
            }
            else if(typeid(a) == typeid(bool)){
                typeId = 2;
            }
            else if(typeid(a) == typeid(float)){
                typeId = 3;
            }
            else  if(typeid(a) == typeid(double)){
                typeId = 4;
            }
            else if(typeid(a) == typeid(char*) || typeid(a) == typeid(const char*)){
                typeId = 5;
            }
            else if(typeid(a) == typeid(awx_string)){
                typeId = 6;
            }
            else{
                typeId = 0;
            }
            
            if(typeid(a) != typeid(awx_string)){
                set(awx_string(value) , awx_string(key) , typeId);
            }
            else{
                set(value , awx_string(key)  , typeId);
            }
		}
		awx_string getObjectForKey(char* key);
		bool writeToFile(char* path);
		bool initWithFile(char* path);
		
		float getFloatForKey(char* key);
		int getIntForKey(char* key);
		awx_vector3f getVectorForKey(char* key);
		
		void plot();
		int length();
		awx_string operator [] (char* key);
		awx_string operator [] (awx_string key);
		void clear();
		
};

ostream &operator << (ostream & out, awx_dictionary a);

#endif