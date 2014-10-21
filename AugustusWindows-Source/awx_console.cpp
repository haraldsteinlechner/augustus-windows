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

#include "awx_console.h"

#include "awx_tga.h"
#include "awx_opengl_utilities.h"



void awx_shutdown(int code);

int awx_createGame(int argc,char** argv);
void awx_resetScene();
void setFogOfWar(float value);
void setCurrentPlayer(int ID);
void setDebug(bool f);
void setRohstoff(int ID,float val);
void enterEgoMode();
void leaveEgoMode();
void setCameraToEnemy();
void setCameraToPlayer();
void openSteuerWindow();
void setSteuer(float steuer);
void setWeather(float alpha);
void startRain();
void setTerrainRenderMode(bool a);
void saveGameFile(awx_string filename);
void loadGameFile(awx_string filename);
void useAStar(bool aStar);
void makeLevelPart();
void startFire(int houseIndex);

void awx_console::render(){
    if(!hidden){
        glDisable(GL_TEXTURE_2D);
            glBegin(GL_POLYGON);
                glVertex3f(x , y , 0);
                glVertex3f(x + width , y , 0);
                glVertex3f(x + width , y + height , 0);
                glVertex3f(x , y + height , 0);
                glVertex3f(x , y , 0);
            glEnd();

        glEnable(GL_TEXTURE_2D);
        
        glPushMatrix();
            glTranslatef(0 , -160 * 0.00012 , 0);
            awx_stroke_output(x,y + height,1,value);
        glPopMatrix();
    }
}

void awx_console::awx_command(char * command){
    history.push_back(awx_string(command));
    
    int code = -1;
	float fcode=-1;
	int temp1=-1;
	float temp2=-1;
	float fval=0;
	char tempStr[50];
    if(sscanf(command , "exit %d" , &code) == 1){
        awx_shutdown(0);
    }
    if(strcmp(command,"reset")==0) awx_resetScene();
    else if(strcmp(command,"restart")==0) 
    {
        awx_shutdown(5);
        char **lol=NULL;
        awx_createGame(0,lol);
    }
    else if(strcmp(command,"flat")==0) glShadeModel(GL_FLAT);
    else if(strcmp(command,"smooth")==00) glShadeModel(GL_SMOOTH);
    else if(sscanf(command,"switch %i",&code)==1)
    {
        if(code>=0)
        {
            setCurrentPlayer(code);
        }
    }
    else if(sscanf(command,"setfow %f", &fcode)==1)
	{
		setFogOfWar(fcode);
	} 
	else if(strcmp(command,"debug")==0) setDebug(true);
	else if(strcmp(command,"nodebug")==0) setDebug(false);
	else if(sscanf(command,"setrohstoff %i %f",&temp1,&temp2)==2) setRohstoff(temp1,temp2);
	else if(strcmp(command,"god")==0)
	{
		setRohstoff(0,10000);
		setRohstoff(1,10000);
		setRohstoff(2,10000);
		setRohstoff(3,10000);
	}
	else if(strcmp(command,"ego")==0) enterEgoMode();
	else if(strcmp(command,"noego")==0) leaveEgoMode();
	else if(strcmp(command,"enemy")==0) setCameraToEnemy();
	else if(strcmp(command,"player")==0) setCameraToPlayer();
	else if(strcmp(command,"steuer")==0) openSteuerWindow();
	else if(sscanf(command,"setsteuer %f",&temp1)==1) setSteuer(temp1); 
	else if(sscanf(command,"setfog %f",&fval)==1) setWeather(fval);
	else if(strcmp(command,"rain")==0) startRain();
	else if(strcmp(command,"terraincull")==0)setTerrainRenderMode(true);
	else if(strcmp(command,"noterraincull")==0)setTerrainRenderMode(false);
	else if(sscanf(command,"save %s",tempStr)==1) saveGameFile(string(tempStr));
	else if(sscanf(command,"load %s",tempStr)==1) loadGameFile(string(tempStr));
	else if(sscanf(command,"astar %i",&temp1)==1) useAStar(temp1);
	else if(strcmp(command,"dupl")==0) makeLevelPart();
	else if(sscanf(command,"startfire %i",&temp1)==1) startFire(temp1);
}


void awx_console::keyDown(int code){
    if(!hidden){
        if(code == 1125){
            if((int)history.size() > depth){
				delete[] value;
                value = new char[history[history.size() - depth - 1].length() + strlen(newLine)];
                sprintf(value , "%s%s", newLine , history[history.size() - depth -1].cString());
                depth++;
            }
        }
        else if(code == 1127){
			delete[] value;
            value = new char[strlen(newLine) +1];
            strcpy(value , newLine);
            depth = 0;
        }
        else if(code == 127){
		
            value[strlen(value)-1] = '\0';
        }
    
        else if(code == 13){
            depth = 0;
            char* befehl;
            int tempint;
            
            tempint = 0;
            for(int i = (int)strlen(value) -1;i > 0; i--){
                if(value[i] == ' ' && value[i -1] == newLine[strlen(newLine) - 2]){
                    tempint = i;
                    break;
                }
            }
        
            befehl = new char[strlen(value) - tempint];

            for(int i = tempint +1 ;i < (int)strlen(value);i++){
                befehl[i - tempint -1] = value[i];
            }
            befehl[strlen(value) - tempint -1] = '\0';
        
            if(strlen(befehl) > 0){
                awx_command(befehl);
            }
            
			delete[] value;
            value = new char[strlen(newLine) +1];
            strcpy(value , newLine);
            hidden = true;
        }
        else if(code < 1024){
            char* temp = new char[strlen(value) +1];
            strcpy(temp , value);
        
            value = new char[strlen(value) +2];
            sprintf(value , "%s%c" , temp , code);
			
			delete[] temp;
        }
    }
}

void awx_console::getusername(char *&username){
    system("echo $USER > temp.awx");
    
    FILE* file = fopen("temp.awx" , "r");
    
    char temp[128];
    int i = 0;
    while(!feof(file)){
        temp[i] = fgetc(file);
        i++;
    }
    temp[i-2] = '\0';
    
    username = new char[strlen(temp) +1];
    strcpy(username , temp);
    
    unlink("temp.awx");
	
}

awx_console::awx_console(){
    char* username;
    getusername( username );
    value = new char[50];
    sprintf(newLine , "World-Of-AWX:/ %s$ " , username);
    
    strcpy(value , newLine);
    hidden = true;
    depth = 0;
}