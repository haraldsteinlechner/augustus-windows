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
#include <cstdio>

#include "awx_array.h"
		
void awx_array::readline(FILE* file , char* str){
    int i = 0;
    char temp = fgetc(file);
    while(temp != '\n' && !feof(file)){
        str[i] = temp;
        temp = fgetc(file);
        i++;
    }
    str[i] = '\0';
}

template <class a> void awx_array::add(a value){
    if(typeid(value) == typeid(int)){
        values.push_back(awx_string(value));
        typ.push_back(1);
    }
    if(typeid(a) == typeid(bool)){
        values.push_back(awx_string(value));
        typ.push_back(2);
    }
    if(typeid(value) == typeid(float) || typeid(value) == typeid(double)){
        values.push_back(awx_string(value));
        typ.push_back(3);
    }
    if(typeid(value) == typeid(char*) || typeid(value) == typeid(const char*)){
        values.push_back(awx_string(value));
        typ.push_back(5);
    }
    if(typeid(value) == typeid(awx_string)){
        values.push_back(value);
        typ.push_back(5);
    }
}

void awx_array::set(awx_array a){
    values = a.values;
}

void awx_array::set(vector <awx_string> a){
    values = a;
}

template <class a> bool awx_array::setObjectAtIndex(int index , a value){
    if(index < values.size()){
        if(typeid(a) == typeid(awx_string)){
            values[index] = value;
            typ[index] = 5;
        }
        else if(typeid(a) == typeid(int)){
            values[index] = awx_string(value);
            typ[index] = 1;
        }
        else if(typeid(a) == typeid(bool)){
            values[index] = awx_string(value);
            typ[index] = 2;
        }
        else if(typeid(a) == typeid(float) || typeid(a) == typeid(double)){
            values[index] = awx_string(value);
            typ[index] = 3;
        }
        else if(typeid(a) == typeid(char*) || typeid(a) == typeid(const char*)){
            values[index] = awx_string(value);
            typ[index] = 5;
        }
        
        return true;
    }
    else return false;
}

void awx_array::writeToFile(char* path){

    FILE* file = fopen(path , "w");
    
    if(file != NULL){
        fprintf(file , "<array>\n");
        for(int i = 0; i < (int)values.size();i++){
            if(typ[i] == 5){
                fprintf(file , "	<string>%s</string>\n" , values[i].cString());
            }
            else if(typ[i] == 1){
                fprintf(file , "	<integer>%s</integer>\n" , values[i].cString());
            }
            else if(typ[i] == 2){
                if(values[i].intValue() == 1){
                    fprintf(file , "	<true/>\n");
                }
                else{
                    fprintf(file , "	<false/>\n");
                }
            }
            else if(typ[i] == 3){
                fprintf(file , "	<real>%s</real>\n" , values[i].cString());
            }
        }
        fprintf(file , "</array>");
        fclose(file);
    }
}
/*
template <class a> bool awx_array::initWithFile(a path){
    
    values.clear();
    typ.clear();
    
    FILE* file = fopen(path , "r");
    char *line  = new char[256];
    
    int start = -1;
    int end = -1;
    
    if(file != NULL){
        while(!feof(file)){
            readline(file , line);
            
            for(int i = 0; i <  strlen(line);i++){
                if(line[i] == '<'){
                    start = i;
                    break;
                }
            }
            for(int u = start; u < strlen(line);u++){
                if(line[u] == '>'){
                    end = u;
                    break;
                }
            }
            char *temp = new char[256];
            char *temp2 = new char[256];
            
            for(int o = start; o  < end;o++){
                temp[o-start] = line[o+1];
            }
            temp[end - start -1] = '\0';
            
            if(strcmp(temp , "string") == 0){
                for(int a = end+2;a <  strlen(line);a++){
                    if(line[a] == '<'){
                        start = a;
                        break;
                    }
                }
                
                for(int o = end; o  < start;o++){
                    temp[o-end] = line[o+1];
                }
                temp[start - end -1] = '\0';
                typ.push_back(5);
                values.push_back(awx_string(temp));
            }
            else if(strcmp(temp , "real") == 0){
                for(int a = end+2;a <  strlen(line);a++){
                    if(line[a] == '<'){
                        start = a;
                        break;
                    }
                }
                
                for(int o = end; o  < start;o++){
                    temp2[o-end] = line[o+1];
                }
                temp2[start - end -1] = '\0';
                typ.push_back(3);
                values.push_back(awx_string(temp2));
            }
            else if(strcmp(temp , "integer") == 0){
                for(int a = end+2;a <  strlen(line);a++){
                    if(line[a] == '<'){
                        start = a;
                        break;
                    }
                }
                
                for(int o = end; o  < start;o++){
                    temp2[o-end] = line[o+1];
                }
                temp2[start - end -1] = '\0';
                typ.push_back(1);
                values.push_back(awx_string(temp2));
            }
            else if(strcmp(temp , "true/") == 0){
                typ.push_back(2);
                values.push_back(awx_string("1"));
            }
            else if(strcmp(temp , "false/") == 0){
                typ.push_back(2);
                values.push_back(awx_string("0"));
            }
            
        }
        return true;
    }
    else return false;
}*/

void awx_array::alloc(int size){
    for(int i = 0; i < size;i++){
        values.push_back(awx_string(""));
    }
}

awx_array awx_array::plus(awx_string a){
    awx_array temp;
    temp.set(values);
    
    temp.values.push_back(a);
    
    if(typeid(a) == typeid(awx_string)){
        temp.typ.push_back(5);
    }
    else if(typeid(a) == typeid(int)){
        temp.typ.push_back(1);
    }
    else if(typeid(a) == typeid(bool)){
        temp.typ.push_back(2);
    }
    else if(typeid(a) == typeid(float) || typeid(a) == typeid(double)){
        temp.typ.push_back(3);
    }
    else if(typeid(a) == typeid(char*) || typeid(a) == typeid(const char*)){
        temp.typ.push_back(5);
    }
    
    return temp;
}

awx_array awx_array::plus(awx_array a){
    awx_array temp;
    temp.set(values);
    
    for(int i = 0; i < (int)a.values.size();i++){
        temp.values.push_back(a.values[i]);
        temp.typ.push_back(a.typ[i]);
    }
    
    return temp;
}

awx_string awx_array::getObjectAtIndex(int index){
    return values[index];
}

template <class a> awx_array &awx_array::operator+=(a value)
{
    add(value);
    return *this;
}

awx_string awx_array::operator [] (int index)
{
    return values[index];
}
		
		
template <class a> awx_array operator + (awx_array array, a value){
	if(typeid(a) == typeid(awx_string)){
		return array.plus(value);
	}
	else if(typeid(a) == typeid(awx_array)){
		return array.plus(value);
	}
	else{
		return array.plus(awx_string(value));
	}
}

awx_array operator + (awx_array array, awx_array array2){
		return array.plus(array2);
}

ostream &operator << (ostream & out, awx_array a){
	for(int i = 0; i < (int)a.values.size();i++){
		if(a.typ[i] != 2){
			cout << a.getObjectAtIndex(i) << endl;
		}
		else{
			if(a.values[i].compare("1") == 0){
				cout << "true" << endl;
			}
			else{
				cout << "false" << endl;
			}
		}
	}
	return out;
}
