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

#include "awx_dictionary.h"

void awx_dictionary::set(awx_string value , awx_string key , int typeID){
    if(typeID != 0){
        int index = -1;
        for(int i = 0; i < (int)keys.size();i++){
            if(key.compare(keys[i]) == 0){
                index = i;
                break;
            }
        }
        
        if(index == -1){
            values.push_back(value);
            keys.push_back(key);
            typ.push_back(typeID);
        }
        else{
            values[index] = value;
            typ[index] = typeID;
        }
    }
}

void awx_dictionary::readline(FILE* file , char* str){
    int i = 0;
    char temp = fgetc(file);
    while(temp != '\n' && !feof(file)){
        str[i] = temp;
        temp = fgetc(file);
        i++;
    }
    str[i] = '\0';
}


awx_string awx_dictionary::getObjectForKey(char* key){
    int index = -1;
    for(int i = 0;i < (int)keys.size();i++){
        if(keys[i].compare(key) == 0){
            index = i;
            break;
        }
    }
    if(index == -1){
        return awx_string("");
    }
    else{
        /*if(typ[index] == 1){
            int returnInt;
            sscanf(values[index].value , "%d" , &returnInt);
            return returnInt;
        }
        else if(typ[index] == 2){
            bool returnBool;
            sscanf(values[index].value , "%d" , &returnBool);
            return returnBool;
        }
        else if(typ[index] == 3){
            float returnFloat;
            sscanf(values[index].value , "%f" , &returnFloat);
            return returnFloat;
        }
        else if(typ[index] == 4){
            double returnDouble;
            sscanf(values[index].value , "%lf" , &returnDouble);
            return returnDouble;
        }*/
            return values[index];
    }
}

bool awx_dictionary::writeToFile(char* path){

    awx_string typen[6];

    typen[1].set("integer");
    typen[2].set("boolean");
    typen[3].set("real");
    typen[4].set("real");
    typen[5].set("string");
    typen[6].set("string");
    
    FILE* file = fopen(path , "w");
    
    if(file != NULL){
        char temp[50];
        sprintf(temp , "<plist version=%c1.0%c>\n<dict>\n" , '"' , '"');
        fprintf(file, temp);
        
        for(int i = 0; i < (int)keys.size();i++){
            fprintf(file , "	<key>%s</key>\n" , keys[i].cString());
            //typen[typ[i]].value
            int type = typ[i];
            
            if(type != 2){
                fprintf(file , "	<%s>%s</%s>\n" , typen[type].cString() ,values[i].cString(), typen[type].cString());
            }
            else{
                if(strcmp(values[i].cString() , "1") == 0){
                    fprintf(file , "	<true/>\n");
                }
                else{
                    fprintf(file , "	<false/>\n");
                }
            }
        }
        
        fprintf(file , "</dict>\n</plist>");
        fclose(file);
        return true;
    }
    else return false;
    
}

void awx_dictionary::clear()
{
	keys.clear();
	values.clear();
	typ.clear();
}

bool awx_dictionary::initWithFile(char* path){

    keys.clear();
    values.clear();
    typ.clear();

    FILE* file = fopen(path , "r");
    
    if(file  != NULL){
        char tempor[50];
        sprintf(tempor , "<plist version=%c%s%c>\n<dict>\n" , '"' , "%f" , '"');
        
        char* tempstr = new char[256];
        char* temptype;
        
        int startType , endType;
        
        while(!feof(file)){
            readline(file , tempstr);
            
            for(int i = 0;  i < (int)strlen(tempstr);i++){
                if(tempstr[i] == '<'){
                    startType = i + 1;
                    break;
                }
            }
            
            for(int i = 0;  i < (int)strlen(tempstr);i++){
                if(tempstr[i] == '>'){
                    endType = i;
                    break;
                }
            }
            
            temptype  = new char[256];
            int length = endType - startType;
            for(int u = 0; u < length;u++){
                temptype[u] = tempstr[u +  startType];
            }
            temptype[length] = '\0';
            
            if(strcmp(temptype , "key") == 0){
                startType = 0;
                for(int o = endType+1;o < (int)strlen(tempstr);o++){
                    if(tempstr[o] == '<'){
                        startType = o;
                        break;
                    }
                }
                length = startType - endType-1;
                
                for(int i = 0;  i< (int)strlen(temptype);i++){
                    temptype[i] = ' ';
                }
                
                for(int u = 0; u < length;u++){
                    temptype[u] = tempstr[u +  endType +1];
                }
                temptype[length] = '\0';

                keys.push_back(awx_string(temptype));
            }
            
            else if(strcmp(temptype , "integer") == 0){
                for(int o = endType+1;o < (int)strlen(tempstr);o++){
                    if(tempstr[o] == '<'){
                        startType = o;
                        break;
                    }
                }
                
                for(int u = 0; u < startType - endType -1;u++){
                    temptype[u] = tempstr[u +  endType +1];
                }
                temptype[startType - endType -1] = '\0';
                
                values.push_back(awx_string(temptype));
                typ.push_back(1);
            }
            
            else if(strcmp(temptype , "true/") == 0){
                values.push_back(awx_string(1));
                typ.push_back(2);
            }
            
            else if(strcmp(temptype , "false/") == 0){
                values.push_back(awx_string(0));
                typ.push_back(2);
            }
            
            else if(strcmp(temptype , "real") == 0){
                for(int o = endType+1;o < (int)strlen(tempstr);o++){
                    if(tempstr[o] == '<'){
                        startType = o;
                        break;
                    }
                }
                
                for(int u = 0; u < startType - endType -1;u++){
                    temptype[u] = tempstr[u +  endType +1];
                }
                temptype[startType - endType -1] = '\0';
                
                values.push_back(awx_string(temptype));
                typ.push_back(3);
            }
                       else if(strcmp(temptype , "string") == 0){
                for(int o = endType;o < (int)strlen(tempstr);o++){
                    if(tempstr[o] == '<'){
                        startType = o;
                        break;
                    }
                }
                
                for(int u = 0; u < startType - endType -1;u++){
                    temptype[u] = tempstr[u +  endType +1];
                }
                temptype[startType - endType -1] = '\0';
                
                values.push_back(awx_string(temptype));
                typ.push_back(6);
            }
			
			else if(strcmp(temptype , "vector") == 0){
                for(int o = endType;o < (int)strlen(tempstr);o++){
                    if(tempstr[o] == '<'){
                        startType = o;
                        break;
                    }
                }
                
                for(int u = 0; u < startType - endType -1;u++){
                    temptype[u] = tempstr[u +  endType +1];
                }
                temptype[startType - endType -1] = '\0';
                
                values.push_back(awx_string(temptype));
                typ.push_back(6);
            }
        }
        
        return true;        
        fclose(file);
    }
    
    else return false;
    
}

float awx_dictionary::getFloatForKey(char* key)
{
	awx_string temp = getObjectForKey(key);
	
	return temp.floatValue();
}

int awx_dictionary::getIntForKey(char* key)
{
	awx_string temp = getObjectForKey(key);
	
	return temp.intValue();
}

awx_vector3f awx_dictionary::getVectorForKey(char* key)
{
	awx_string temp = getObjectForKey(key);
	
	awx_vector3f ret;

	if(sscanf(temp.cString(),"%f/%f/%f",&ret.x,&ret.y,&ret.z)==3) return ret;
	
	cout << "dict parseError (getVectorForKey)" << endl;
	
	return ret;
}

void awx_dictionary::plot(){
    for(int i = 0; i < (int)keys.size();i++){
        cout << keys[i] << "   " << values[i] << endl;
    }
}

int awx_dictionary::length(){
    return (int)values.size();
}

awx_string awx_dictionary::operator [] (char* key)
{
    awx_string temp  = getObjectForKey(key);
    return temp;
}
awx_string awx_dictionary::operator [] (awx_string key)
{
    awx_string temp  = getObjectForKey(key.cString());
    return temp;
}

ostream &operator << (ostream & out, awx_dictionary a){
	a.plot();
	return out;
}
