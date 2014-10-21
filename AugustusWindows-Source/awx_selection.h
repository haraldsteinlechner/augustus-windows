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

#define BUFSIZE 4096

GLuint selectBuf[BUFSIZE];
GLint hits;

/*
Selection bei units = units[selection]
Selection bei buildings = buidings[selection+units.size()]
*/


void selection_render()
{
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	
	glLoadIdentity();
	glLoadMatrixd(mvmatrix);
	
	for(int u = 0; u < (int)player.size();u++){
		for(int i=0;i<(int)player[u].units.size();i++)
		{
			if(player[u].units[i].fortschritt >= 100){
				glPushName(u*1048576 + i);
				player[u].units[i].render();
				glPopName();
			}
		}
	
		for(int i=0;i<(int)player[u].buildings.size();i++)
		{
			glPushName(u * 1048576 + i+(int)player[u].units.size());
			player[u].buildings[i].render();
			glPopName();
		}
	}

	
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}


void processHits (GLint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

   ptr = (GLuint *) buffer;
   minZ = 0xffffffff;
   
   for (i = 0; i < (unsigned int)hits; i++) { 
      names = *ptr;
          ptr++;
          if (*ptr < minZ) {
                  numberOfNames = names;
                  minZ = *ptr;
                  ptrNames = ptr+2;
          }
          
          ptr += names+2;
        }
		
  ptr = ptrNames;
  
  if(numberOfNames<10)
  {
	for (j = 0; j < numberOfNames; j++) {
		if(((int)*ptr % 1048576)<10000){
			int temp = (int) *ptr;
			controls.preselection = temp % 1048576;
			controls.player_preselection = temp / 1048576;
		}
		ptr++;
	}
  } else if((int)*ptr<10000)  1;
}

inline void calculateSelection()
{ 
   glSelectBuffer(BUFSIZE, selectBuf);
   (void) glRenderMode(GL_SELECT);

   glInitNames();
   glPushName(0);

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   
   gluPickMatrix((GLdouble) controls.x, (GLdouble) (project.height-controls.y),
                2.0, 2.0, project.viewport);
				 
   glMultMatrixd(project.projmatrix);

   glMatrixMode(GL_MODELVIEW);
   	   	
   selection_render();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glFlush();

   hits = glRenderMode(GL_RENDER);
   
   if(hits!=0)
   {
	  processHits(hits, selectBuf);
   } else controls.preselection=-1;
   
   	glMatrixMode(GL_MODELVIEW);
}