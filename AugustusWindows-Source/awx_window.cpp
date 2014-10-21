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

#include "awx_window.h"

#include "awx_tga.h"
#include "awx_opengl_utilities.h"

void awx_window::setTexture(char* texpath){
    char *temp = new char[ strlen(interfacePath) + strlen(texpath) + 1 ];
    sprintf(temp , "%s%s" , interfacePath , texpath);
    load_tga(temp , currentID, false, false);
    texID = currentID;
    currentID++;
    
    specialTexture = false;
	delete[] temp;
}

void awx_window::setSpecialTexture(char * texpath){
    char *temp = new char[ strlen(interfacePath) + strlen(texpath) + 1 ];
    sprintf(temp , "%s%s" , interfacePath , texpath);
    load_tga(temp , currentID, false, false);
    texID = currentID;
    currentID++;
    
    specialTexture = true;
	delete[] temp;
}

void awx_window::render()
{ 
    if(!hidden){
        if(!specialTexture){
            glBindTexture(GL_TEXTURE_2D , textur_liste[texID]);

            if(!resize){
                glBegin(GL_POLYGON);
                    glTexCoord2f(0.0,0.0);
                    glVertex3f(x , y , 0);
                    glTexCoord2f(1.0,0.0);
                    glVertex3f(x + width , y , 0);
                    glTexCoord2f(1.0, 1.0);
                    glVertex3f(x + width , y + height , 0);
                    glTexCoord2f(0.0, 1.0);
                    glVertex3f(x , y + height , 0);
                    glTexCoord2f(0.0,0.0);
                    glVertex3f(x , y , 0);
                glEnd();
            }
            else{
                glBegin(GL_POLYGON);
                    glTexCoord2f(0.0,0.0);
                    glVertex3f(x , y , 0);
                    glTexCoord2f(1.0,0.0);
                    glVertex3f(x + width , y , 0);
                    glTexCoord2f(1.0, 1.0/project.aspect);
                    glVertex3f(x + width , y + height , 0);
                    glTexCoord2f(0.0, 1.0/project.aspect);
                    glVertex3f(x , y + height , 0);
                    glTexCoord2f(0.0,0.0);
                    glVertex3f(x , y , 0);
                glEnd();
            }
        }
        else{
            float rand = 0.12;
            float randW = 0.023;
            
            glBindTexture(GL_TEXTURE_2D , textur_liste[texID]);
            
            glBegin(GL_POLYGON);
                glTexCoord2f(0.0,0.0);
                glVertex3f(x , y , 0);
                glTexCoord2f( rand ,0.0);
                glVertex3f(x + randW , y , 0);
                glTexCoord2f( rand , rand);
                glVertex3f(x + randW , y + randW , 0);
                glTexCoord2f( 0.0 , rand);
                glVertex3f(x  , y + randW , 0);
                glTexCoord2f( 0.0 ,0.0);
                glVertex3f(x , y , 0);
            glEnd();
            
            glBegin(GL_POLYGON);
                glTexCoord2f(rand,0.0);
                glVertex3f(x + randW , y , 0);
                glTexCoord2f( 1.0 - rand ,0.0);
                glVertex3f(x + width - randW , y , 0);
                glTexCoord2f( 1.0 - rand , rand);
                glVertex3f(x + width - randW , y + randW , 0);
                glTexCoord2f( rand , rand);
                glVertex3f(x + randW  , y + randW , 0);
                glTexCoord2f( rand ,0.0);
                glVertex3f(x + randW , y , 0);
            glEnd();
            
            glBegin(GL_POLYGON);
                glTexCoord2f(1.0 - rand ,0.0);
                glVertex3f(x + width - randW , y , 0);
                glTexCoord2f( 1.0 ,0.0);
                glVertex3f(x + width, y , 0);
                glTexCoord2f( 1.0 , rand);
                glVertex3f(x + width , y + randW , 0);
                glTexCoord2f( 1.0 - rand , rand);
                glVertex3f(x + width - randW , y + randW , 0);
                glTexCoord2f( 1.0 - rand ,0.0);
                glVertex3f(x + width - randW , y , 0);
            glEnd();
            
            glBegin(GL_POLYGON);
                glTexCoord2f( 0.0 , rand);
                glVertex3f(x , y + randW , 0);
                glTexCoord2f( rand , rand);
                glVertex3f(x + randW , y + randW , 0);
                glTexCoord2f( rand , 1.0 - rand);
                glVertex3f(x + randW , y + height - randW , 0);
                glTexCoord2f( 0.0 , 1.0 - rand);
                glVertex3f(x, y + height - randW , 0);
                glTexCoord2f( 0.0  , rand);
                glVertex3f(x , y + randW , 0);
            glEnd();
            
            glBegin(GL_POLYGON);
                glTexCoord2f( 0.0 , 1.0 - rand);
                glVertex3f(x , y + height - randW , 0);
                glTexCoord2f( rand , 1.0 - rand);
                glVertex3f(x + randW , y + height - randW , 0);
                glTexCoord2f( rand , 1.0);
                glVertex3f(x + randW , y + height , 0);
                glTexCoord2f( 0.0 , 1.0);
                glVertex3f(x, y + height , 0);
                glTexCoord2f( 0.0  , 1.0 -  rand);
                glVertex3f(x , y + height - randW , 0);
            glEnd();
            
            glBegin(GL_POLYGON);
                glTexCoord2f( rand , 1.0 - rand);
                glVertex3f(x + randW , y + height - randW , 0);
                glTexCoord2f( 1.0 - rand , 1.0 - rand);
                glVertex3f(x + width - randW , y + height - randW , 0);
                glTexCoord2f( 1.0 - rand , 1.0);
                glVertex3f(x + width - randW , y + height , 0);
                glTexCoord2f( rand , 1.0);
                glVertex3f(x + randW, y + height , 0);
                glTexCoord2f( rand , 1.0 - rand);
                glVertex3f(x + randW , y + height - randW , 0);
            glEnd();
            
            glBegin(GL_POLYGON);
                glTexCoord2f( 1.0 - rand , 1.0 - rand);
                glVertex3f(x + width - randW , y + height - randW , 0);
                glTexCoord2f( 1.0 , 1.0 - rand);
                glVertex3f(x + width , y + height - randW , 0);
                glTexCoord2f( 1.0 , 1.0);
                glVertex3f(x + width , y + height , 0);
                glTexCoord2f( 1.0 - rand , 1.0);
                glVertex3f(x + width - randW , y + height , 0);
                glTexCoord2f( 1.0 - rand , 1.0 - rand);
                glVertex3f(x + width - randW , y + height - randW , 0);
            glEnd();
            
            glBegin(GL_POLYGON);
                glTexCoord2f( 1.0 - rand , rand );
                glVertex3f(x + width - randW , y + randW , 0);
                glTexCoord2f( 1.0 , rand );
                glVertex3f(x + width , y + randW , 0);
                glTexCoord2f( 1.0 , 1.0 - rand);
                glVertex3f(x + width , y + height - randW , 0);
                glTexCoord2f( 1.0 - rand , 1.0 - rand);
                glVertex3f(x + width - randW , y + height - randW , 0);
                glTexCoord2f( 1.0 - rand , rand );
                glVertex3f(x + width - randW , y + randW , 0);
            glEnd();
            
            glBegin(GL_POLYGON);
                glTexCoord2f( rand , rand );
                glVertex3f(x + randW , y + randW  , 0);
                glTexCoord2f( 1.0 - rand , rand );
                glVertex3f(x + width - randW , y + randW , 0);
                glTexCoord2f( 1.0 - rand , 1.0 - rand);
                glVertex3f(x + width - randW , y + height - randW , 0);
                glTexCoord2f( rand , 1.0 - rand);
                glVertex3f(x + randW , y + height - randW , 0);
                glTexCoord2f( rand , rand );
                glVertex3f(x + randW , y + randW  , 0);
            glEnd();
            
        }
        
        glPushMatrix();
            glTranslatef(x , y , 0);
        
            for(int i = 0; i < (int)buttons.size();i++)
            {
                buttons[i].render();
            }
        
            for(int i = 0; i < (int)checkboxes.size();i++){
                checkboxes[i].render();
            }
        
            for(int i = 0; i < (int)radio_buttons.size();i++){
                radio_buttons[i].render();
            }
        
            for(int i = 0; i < (int)sliders.size();i++){
                sliders[i].render();
            }
                
            for(int i = 0; i < (int)progress_indicators.size();i++){
                progress_indicators[i].render();
            }
			
			for(int i = 0; i < (int)textFields.size();i++){
                textFields[i].render();
            }
            
            map.render();
                
        glPopMatrix();
    }
    
}

void awx_window::dealloc()
{
	for(int i = 0; i < (int)textFields.size();i++)
	{
		textFields[i].dealloc();
	}
	
	glDeleteTextures(1,&textur_liste[texID]);
	cout << "Deallocated Texture:" << texID << endl;
}

void awx_window::mouseOver(float x , float y)
{
    if(!hidden){
        for(int i = 0; i < (int)buttons.size();i++)
        {
            buttons[i].mouseOver(x - this->x , y - this->y);
        }
    
        for(int i = 0; i < (int)checkboxes.size();i++)
        {
            checkboxes[i].mouseOver(x - this->x , y - this->y);
        }
    
        for(int i = 0; i < (int)radio_buttons.size();i++)
        {
            radio_buttons[i].mouseOver(x - this->x , y - this->y);
        }
    
        for(int i = 0; i < (int)sliders.size();i++)
        {
            sliders[i].mouseOver(x - this->x , y - this->y);
        }
        
        for(int i = 0; i < (int)progress_indicators.size();i++)
        {
            progress_indicators[i].mouseOver(x - this->x , y - this->y);
        }
		
		for(int i = 0; i < (int)textFields.size();i++)
		{
			textFields[i].mouseOver(x - this->x , y - this->y);
		}
		
        map.mouseOver(x - this->x , y - this->y);
    }
}

void awx_window::mouseDown(float x , float y , int button)
{
    if(!hidden){
        for(int i = 0; i < (int)buttons.size();i++)
        {
            if(x - this->x >= buttons[i].x &&
               y - this->y >= buttons[i].y &&
               x - this->x <= buttons[i].x + buttons[i].width &&
               y - this->y <= buttons[i].y + buttons[i].height)
            {
                buttons[i].mouseDown(x - this->x , y - this->y , button);
                break;
            }	
        }	
    
        for(int i = 0; i < (int)checkboxes.size();i++)
        {
            if(x - this->x >= checkboxes[i].x &&
               y - this->y >= checkboxes[i].y &&
               x - this->x <= checkboxes[i].x + checkboxes[i].width &&
               y - this->y <= checkboxes[i].y + checkboxes[i].height)
            {
                checkboxes[i].mouseDown(x - this->x , y - this->y , button);
                break;
            }	
        }
    
        for(int i = 0; i < (int)radio_buttons.size();i++)
        {
            if(x - this->x >= radio_buttons[i].x &&
               y - this->y >= radio_buttons[i].y &&
               x - this->x <= radio_buttons[i].x + radio_buttons[i].width &&
               y - this->y <= radio_buttons[i].y + radio_buttons[i].height + radio_buttons[i].mouseOverFlag.size() * radio_buttons[i].dif)
            {
                radio_buttons[i].mouseDown(x - this->x , y - this->y , button);
                break;
            }	
        }
    
        for(int i = 0; i < (int)sliders.size();i++)
        {
            if(x - this->x >= sliders[i].x - sliders[i].height/2.0 &&
               y - this->y >= sliders[i].y &&
               x - this->x <= sliders[i].x + sliders[i].width + sliders[i].height/2.0 &&
               y - this->y <= sliders[i].y + sliders[i].height)
            {
                sliders[i].mouseDown(x - this->x , y - this->y , button);
                break;
            }	
        }
        
        for(int i = 0; i < (int)progress_indicators.size();i++)
        {
            if(x - this->x >= progress_indicators[i].x &&
               y - this->y >= progress_indicators[i].y &&
               x - this->x <= progress_indicators[i].x + progress_indicators[i].width &&
               y - this->y <= progress_indicators[i].y + progress_indicators[i].height)
            {
                sliders[i].mouseDown(x - this->x , y - this->y , button);
                break;
            }	
        }
		
		for(int i = 0; i < (int)textFields.size();i++)
        {
            if(x - this->x >= textFields[i].x &&
               y - this->y >= textFields[i].y &&
               x - this->x <= textFields[i].x + textFields[i].width &&
               y - this->y <= textFields[i].y + textFields[i].height &&
			   textFields[i].mouseOverFlag)
            {
                textFields[i].mouseDown(x - this->x , y - this->y , button);
                break;
            }	
        }
        
        map.mouseDown(x - this->x , y - this->y , button);				
    }
}

void awx_window::mouseUp(float x , float y , int button)
{
    if(!hidden){
        for(int i = 0; i < (int)buttons.size();i++)
        {
            buttons[i].mouseUp(x - this->x , y - this->y , button);	
        }
    
        for(int i = 0; i < (int)checkboxes.size();i++)
        {
            checkboxes[i].mouseUp(x - this->x , y - this->y , button);	
        }
    
        for(int i = 0; i < (int)radio_buttons.size();i++)
        {
            radio_buttons[i].mouseUp(x - this->x , y - this->y , button);	
        }
    
        for(int i = 0; i < (int)sliders.size();i++)
        {
            sliders[i].mouseUp(x - this->x , y - this->y , button);	
        }
        
        for(int i = 0; i < (int)progress_indicators.size();i++)
        {
            progress_indicators[i].mouseUp(x - this->x , y - this->y , button);	
        }
		
		for(int i = 0; i < (int)textFields.size();i++)
        {
            textFields[i].mouseOver(x - this->x , y - this->y);	
        }
    }
}