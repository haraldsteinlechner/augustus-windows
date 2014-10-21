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


#include "awx_filesystem.h"


vector <awx_string> deleteEntriesContaining( vector <awx_string> source , char delimiter){

	vector <awx_string> temp;

	bool copy;


	for(int i = 0;i < (int)source.size();i++){
		copy = true;
		for(int u = 0; u < (int)source[i].length(); u++){
			if(source[i][u] == delimiter){
				copy = false;
			}
		}
		
		if(copy){
			temp.push_back(source[i]);
		}
		
	}
	
	return temp;
}

vector <awx_string> deleteEntriesNotContaining( vector <awx_string> source , char delimiter){

	vector <awx_string> temp;

	bool copy;


	for(int i = 0;i < (int)source.size();i++){
		copy = false;
		for(int u = 0; u < (int)source[i].length(); u++){
			if(source[i][u] == delimiter){
				copy = true;
			}
		}
		
		if(copy){
			temp.push_back(source[i]);
		}
		
	}
	
	return temp;
}

int subpaths(char *dir , vector <awx_string> &elements){
	
	elements.clear();
	char befehl[256];
	
	awx_string tempfile(dir);
	tempfile = tempfile.stringByDeletingLastPathComponent();
	tempfile = tempfile.stringByAppendingPathComponent("temp.awx");
	
	awx_string data;
	
	#ifdef _WIN32
		sprintf(befehl,"dir /b \"%s\" > %s" , dir , tempfile.cString());
	#endif
	
	#ifndef _WIN32
		sprintf(befehl,"ls -m '%s' > %s" , dir , tempfile.cString());
	#endif
	
	system(befehl);
	
	data.readFromFile(tempfile);
	unlink(tempfile.cString());
	
	#ifdef _WIN32
		//data = data.replaceChar('\n',' ');
		data.split('\n' , elements);
		for(int i=0;i<elements.size();i++)
		{
			if(elements[i].length()<2) 
			{
				elements.erase(elements.begin()+i);
				i--;
			}
		}
	#endif
	
	#ifndef  _WIN32
		data = data.replaceChar('\n',' ');
		data.split(',' , elements);	
	#endif
	
	return (int)elements.size();
}

int subdirs(char* dir , vector <awx_string> &elements){
	subpaths(dir , elements);
	elements = deleteEntriesContaining(elements , '.');
	
	return (int)elements.size();
}

int subfiles(char* dir , vector <awx_string> &elements){
	subpaths(dir , elements);
	elements = deleteEntriesNotContaining(elements , '.');
	
	return (int)elements.size();
}

int subpaths(awx_string dir , vector <awx_string> &elements){
	return subpaths(dir.cString() , elements);
}

int subfiles(awx_string dir , vector <awx_string> &elements){
	return subfiles(dir.cString() , elements);
}

int subdirs(awx_string dir , vector <awx_string> &elements){
	return subdirs(dir.cString() , elements);
}