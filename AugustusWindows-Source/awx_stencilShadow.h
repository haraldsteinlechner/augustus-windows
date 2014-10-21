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

inline void drawShadowVolumeToStencil()
{
	bool twoSidedStencil = false;
	
	if(twoSidedStencil)
	{
				glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);
				glEnable(GL_CULL_FACE);
			
				glActiveStencilFaceEXT(GL_FRONT);
				glStencilOp(GL_KEEP,GL_KEEP,GL_INCR_WRAP);
				glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
		
						
				glActiveStencilFaceEXT(GL_BACK);
				glStencilOp(GL_KEEP,GL_KEEP,GL_DECR_WRAP);
				glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
			
				for(int u = 0; u < (int)player.size();u++)
				{
					for(int i=0;i<(int)player[u].buildings.size();i++)
					{
						player[u].buildings[i].renderExtrusion();
					}
				}
				
				glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
				glDisable(GL_CULL_FACE);
	}
	else
	{
				glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
			
				glStencilOp(GL_KEEP,GL_KEEP,GL_INCR_WRAP);
				glFrontFace(GL_CCW);
		
				for(int u = 0; u < (int)player.size();u++)
				{
					for(int i=0;i<(int)player[u].buildings.size();i++)
					{
						player[u].buildings[i].renderExtrusion();
					}
				}
		
				glStencilOp(GL_KEEP,GL_KEEP,GL_DECR_WRAP);
				glFrontFace(GL_CW);
		
				for(int u = 0; u < (int)player.size();u++)
				{
					for(int i=0;i<(int)player[u].buildings.size();i++)
					{
						player[u].buildings[i].renderExtrusion();
					}	
				}
	}
}

inline void shadowPass()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);
	
				float pos[] = {0.0,90.0,2.0,1.0};
				glLightfv(GL_LIGHT0,GL_POSITION,pos);
				//glUseProgramObjectARB(shadowshader.p);
		
				glColorMask(0,0,0,0);
				glDepthMask(0);
				glEnable(GL_CULL_FACE);
				glEnable(GL_STENCIL_TEST);
			
				drawShadowVolumeToStencil();
	
			glPopAttrib();
		glEnable(GL_STENCIL_TEST);
		glDepthFunc(GL_EQUAL);
		glStencilFunc(GL_NOTEQUAL,0,0xFFFFFFFFL);
		glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

			glDisable(GL_DEPTH_TEST);
			
			glMatrixMode(GL_PROJECTION);
			
			
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0.0,1.0,0.0,1.0);
			
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glLoadIdentity();
				//glTranslatef(0.0,0.0,-5.0);
				
				glColor4f(0.0,0.0,0.0,0.3);
				
				glDisable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_ALPHA_TEST);
				glDepthMask(0);
				
				glBegin(GL_QUADS);
					glVertex3f(0.0,0.0,0.0);
					glVertex3f(1.0,0.0,0.0);
					glVertex3f(1.0,1.0,0.0);
					glVertex3f(0.0,1.0,0.0);
				glEnd();
				
				glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			
			glMatrixMode(GL_MODELVIEW);
			glEnable(GL_DEPTH_TEST);
	glPopAttrib();
}

inline void shadowPassDynamic()
{

	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_STENCIL_BUFFER_BIT);
	
			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);
	
				float pos[] = {30.0,90.0,2.0,1.0};
				glPushMatrix();
				glLightfv(GL_LIGHT0,GL_POSITION,pos);
				glPopMatrix();
				glUseProgramObjectARB(shadowshader.p);
		
				glColorMask(0,0,0,0);
				glDepthMask(0);
				glEnable(GL_CULL_FACE);
				glEnable(GL_STENCIL_TEST);
			
				glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
			
				glStencilOp(GL_KEEP,GL_KEEP,GL_INCR_WRAP);
				glFrontFace(GL_CCW);
		
				for(int u = 0; u < (int)player.size();u++)
				{
					for(int i=0;i<(int)player[u].buildings.size();i++)
					{
						player[u].buildings[i].renderExtrusion();
					}
				}
		
				glStencilOp(GL_KEEP,GL_KEEP,GL_DECR_WRAP);
				glFrontFace(GL_CW);
		
				for(int u = 0; u < (int)player.size();u++)
				{
					for(int i=0;i<(int)player[u].buildings.size();i++)
					{
						player[u].buildings[i].renderExtrusion();
					}	
				}
				
				glUseProgramObjectARB(NULL);
	
			glPopAttrib();
	
		glEnable(GL_STENCIL_TEST);
		glDepthFunc(GL_EQUAL);
		glStencilFunc(GL_NOTEQUAL,0,0xFFFFFFFFL);
		glStencilFunc(GL_KEEP,GL_KEEP,GL_KEEP);

			glDisable(GL_DEPTH_TEST);
			
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0.0,1.0,0.0,1.0);
			
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glLoadIdentity();
				//glTranslatef(0.0,0.0,-5.0);
				
				glColor4f(0.0,0.0,0.0,0.3);
				
				glDisable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
				glDepthMask(0);
				
				glBegin(GL_QUADS);
					glVertex3f(0.0,0.0,0.0);
					glVertex3f(1.0,0.0,0.0);
					glVertex3f(1.0,1.0,0.0);
					glVertex3f(0.0,1.0,0.0);
				glEnd();
				
				glPopMatrix();
			
			glPopMatrix();
			
			glEnable(GL_DEPTH_TEST);
	
	glPopAttrib();
}