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

/*					Alle Rendering Funktionen/Idles							 */

awx_text info;


float relMouse_y,relMouse_x;

void checkVisibilitiesUnits()
{
	for(int u=0;u<(int)player.size();u++)
	{
		for(int i=0;i<(int)player[u].units.size();i++)
		{
			player[u].units[i].checkVisibility();
		}
	}
}

void renderBlobs()
{
		// Alle Blobs malen
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	
	
	if((int)controls.selection.size()==1&&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].units.size()+(int)player[controlIndex].buildings.size())
	{
		awx_building* temp = &player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()];
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_LIGHTING);
		//glDisable(GL_BLEND);
		glPushMatrix();
			//glLoadIdentity();
			glTranslatef(temp->x , temp->y , temp->z);
			glRotatef(temp->winkel_y , 0 , 1 , 0);
			glBindTexture(GL_TEXTURE_2D,textur_liste[selectedQuadID]);
			
			float width = temp->data[2].xmax - temp->data[2].xmin;
			float height = temp->data[2].zmax - temp->data[2].zmin;
			float rand = 1.5;
			
			
			// SCHORSCHED
			
			//HINTEN RECHTS
			glBegin(GL_POLYGON);
				glTexCoord2f(0,0);
				glVertex3f(temp->data[2].xmin -0.5, 0 , temp->data[2].zmin -0.5);
				glTexCoord2f(0.25,0);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin -0.5);
				glTexCoord2f(0.25,0.25);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin -0.5 + rand);
				glTexCoord2f(0,0.25);
				glVertex3f(temp->data[2].xmin -0.5, 0 , temp->data[2].zmin -0.5 + rand);
				glTexCoord2f(0,0);
				glVertex3f(temp->data[2].xmin -0.5, 0 , temp->data[2].zmin -0.5);
			glEnd();
			
			//HINTEN
			glBegin(GL_POLYGON);
				glTexCoord2f(0.25,0);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin -0.5);
				glTexCoord2f(0.75,0);
				glVertex3f(temp->data[2].xmin + width - rand + 0.5, 0 , temp->data[2].zmin -0.5);
				glTexCoord2f(0.75,0.25);
				glVertex3f(temp->data[2].xmin + width - rand + 0.5, 0 , temp->data[2].zmin -0.5 + rand);
				glTexCoord2f(0.25,0.25);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin -0.5 + rand);
				glTexCoord2f(0.25,0);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin -0.5);
			glEnd();
			
			//VORNE RECHTS
			glBegin(GL_POLYGON);
				glTexCoord2f(0,0.75);
				glVertex3f(temp->data[2].xmin -0.5, 0 , temp->data[2].zmin -0.5 + height);
				glTexCoord2f(0.25,0.75);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin -0.5 + height);
				glTexCoord2f(0.25,1);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin -0.5 + rand + height);
				glTexCoord2f(0,1);
				glVertex3f(temp->data[2].xmin -0.5, 0 , temp->data[2].zmin -0.5 + rand + height);
				glTexCoord2f(0,0.75);
				glVertex3f(temp->data[2].xmin -0.5, 0 , temp->data[2].zmin -0.5 + height);
			glEnd();
			
			//VORNE
			glBegin(GL_POLYGON);
				glTexCoord2f(0.25,0.75);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin - 0.5 + height);
				glTexCoord2f(0.75,0.75);
				glVertex3f(temp->data[2].xmin + 0.5 + width - rand, 0 , temp->data[2].zmin - 0.5 + height);
				glTexCoord2f(0.75,1);
				glVertex3f(temp->data[2].xmin + 0.5 + width - rand, 0 , temp->data[2].zmin - 0.5 + height + rand);
				glTexCoord2f(0.25,1);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin - 0.5 + height + rand);
				glTexCoord2f(0.25,0.75);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin - 0.5 + height);
			glEnd();
			
			//VORNE LINKS
			glBegin(GL_POLYGON);
				glTexCoord2f(0.75,0.75);
				glVertex3f(temp->data[2].xmin + 0.5 + width - rand, 0 , temp->data[2].zmin - 0.5 + height);
				glTexCoord2f(1.0,0.75);
				glVertex3f(temp->data[2].xmin + 0.5 + width , 0 , temp->data[2].zmin - 0.5 + height);
				glTexCoord2f(1.0,1.0);
				glVertex3f(temp->data[2].xmin + 0.5 + width , 0 , temp->data[2].zmin - 0.5 + rand + height);
				glTexCoord2f(0.75,1.0);
				glVertex3f(temp->data[2].xmin + 0.5 + width - rand , 0 , temp->data[2].zmin - 0.5 + rand + height);
				glTexCoord2f(0.75,0.75);
				glVertex3f(temp->data[2].xmin + 0.5 + width - rand , 0 , temp->data[2].zmin - 0.5 + height);
			glEnd();
			
			//HINTEN RECHTS
			glBegin(GL_POLYGON);
				glTexCoord2f(0.75,0.0);
				glVertex3f(temp->data[2].xmin + 0.5 + width - rand, 0 , temp->data[2].zmin - 0.5);
				glTexCoord2f(1.0,0.0);
				glVertex3f(temp->data[2].xmin + 0.5 + width , 0 , temp->data[2].zmin - 0.5);
				glTexCoord2f(1.0,0.25);
				glVertex3f(temp->data[2].xmin + 0.5 + width , 0 , temp->data[2].zmin - 0.5 + rand);
				glTexCoord2f(0.75,0.25);
				glVertex3f(temp->data[2].xmin + 0.5 + width - rand , 0 , temp->data[2].zmin - 0.5 + rand);
				glTexCoord2f(0.75,0.0);
				glVertex3f(temp->data[2].xmin + 0.5 + width - rand , 0 , temp->data[2].zmin - 0.5);
			glEnd();
			
			//RECHTS
			glBegin(GL_POLYGON);
				glTexCoord2f(0,0.25);
				glVertex3f(temp->data[2].xmin -0.5, 0 , temp->data[2].zmin -0.5 + rand);
				glTexCoord2f(0.25,0.25);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin -0.5 + rand);
				glTexCoord2f(0.25,0.75);
				glVertex3f(temp->data[2].xmin -0.5 + rand, 0 , temp->data[2].zmin +1 + height - rand);
				glTexCoord2f(0,0.75);
				glVertex3f(temp->data[2].xmin -0.5, 0 , temp->data[2].zmin +1 + height - rand);
				glTexCoord2f(0,0.25);
				glVertex3f(temp->data[2].xmin -0.5, 0 , temp->data[2].zmin -0.5 + rand);
			glEnd();
			
			//LINKS
			glBegin(GL_POLYGON);
				glTexCoord2f(0.75,0.25);
				glVertex3f(temp->data[2].xmin +0.5 + width - rand, 0 , temp->data[2].zmin -0.5 + rand);
				glTexCoord2f(1.0,0.25);
				glVertex3f(temp->data[2].xmin +0.5 + width, 0 , temp->data[2].zmin -0.5 + rand);
				glTexCoord2f(1.0,0.75);
				glVertex3f(temp->data[2].xmin +0.5 + width, 0 , temp->data[2].zmin +1 - rand + height);
				glTexCoord2f(0.75,0.75);
				glVertex3f(temp->data[2].xmin +0.5 + width - rand, 0 , temp->data[2].zmin +1 - rand + height);
				glTexCoord2f(0.75,0.25);
				glVertex3f(temp->data[2].xmin +0.5 + width - rand, 0 , temp->data[2].zmin -0.5  + rand);
			glEnd();
			
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}
	
	if(hastToCheckVisibility) checkVisibilitiesUnits();
	hastToCheckVisibility = false;
	

	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(-10.0,-1.0);
	glDepthMask(GL_FALSE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	currentLevel->renderBlobsAll();

	
	for(int u = 0; u < (int)player.size();u++){
		for(int i = 0; i < (int)player[u].units.size();i++)
		{
			player[u].units[i].idle();
			if(!player[u].units[i].invisible && player[u].units[i].fortschritt >= 100)
			{
				if(controls.isSelected(u ,i)) player[u].units[i].renderBlobs(player[u].units[i].position,selectedID,1.2);
				player[u].units[i].renderBlobs(player[u].units[i].position);
			}
		} 
	}

	glDisable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void renderLevelObjects()
{
	/*for(int u=0;u<currentLevel->number_of_objects;u++)
	{
		currentLevel->data[u].render();
	}*/
	currentLevel->renderLevelObjectsOptimized(true);
}

Uint32 startMouseLocateTime;

void render_level()
{
	#ifdef GCCCompile
	#warning zeitgeschichte
	#endif
	float temp_time = playedTime.stunde * 3600 + playedTime.minute * 60 + playedTime.sekunde;
	temp_time = temp_time + (1.0/float(runtime.fps))*zeitfaktor;
	playedTime = awx_time(temp_time);
	//currentLevel->renderFOWBottom();

	glPushAttrib(GL_LIGHTING);
	
	{
		if(project.perPixelLighting)
		{
			GLfloat no_mat[] = {0.05,0.05,0.05,1.0};
			GLfloat diffuse[] = {0.9,0.9,0.9,1.0};
			glMaterialfv(GL_FRONT,GL_AMBIENT,no_mat);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
		}
		else 
		{
			GLfloat no_mat[] = {0.00,0.00,0.00,1.0};
			GLfloat diffuse[] = {0.9,0.9,0.9,1.0};
			glMaterialfv(GL_FRONT,GL_AMBIENT,no_mat);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
		}
		GLfloat specular[] = {0.6,0.6,0.6,1.0};
		GLfloat shininess[] = {120.0};

		glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
		glMaterialfv(GL_FRONT,GL_SHININESS,shininess);
	}

//HERE
	//currentLevel->render();
	
	if(allowTerrainCulling)
	currentLevel->terrain.renderSektorWithIndexVector(currentLevel->terrain.terrainLODVector);
	else currentLevel->render();
	//if(controls.selection.size()>0)
	//currentLevel->terrain.renderSektorWithIndex(currentLevel->terrain.getIndexSectorFromWorldCoordinates(awx_vector3f(player[0].units[controls.selection[0]].position)));

	
	glPopAttrib();
	
	
	if(controls.debugshader)
	{
		glEnable(GL_BLEND);
		glPolygonOffset(-1.0,-1.0);
		glEnable(GL_POLYGON_OFFSET_FILL);
		currentLevel->terrain.renderDebug();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_BLEND);
	}

	//if((SDL_GetTicks()-startMouseLocateTime)>50)
	//{
	//	mouse_locate();
	//	startMouseLocateTime=SDL_GetTicks();
	//1}
	
}

void checkVisibilities()
{
	currentLevel->checkVisibilityAll();	
	
	for(int u=0;u<(int)player.size();u++)
	{
	
		for(int i=0;i<(int)player[u].buildings.size();i++)
		{
			player[u].buildings[i].myIndex = i;
			player[u].buildings[i].checkVisibility();
		}
		for(int i=0;i<(int)player[u].units.size();i++)
		{
			player[u].units[i].checkVisibility();
		}
	}
}


void renderTTFFonts()
{
	SDL_GL_Enter2DMode();

	if(controls.mouse_down&&!controls.navigate)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glColor4f(1.0,1.0,1.0,0.8);
		glBegin(GL_LINE_STRIP);
		glVertex2i(controls.mouse_start_x,controls.mouse_start_y);
		glVertex2i(controls.mouse_start_x,controls.y);
		glVertex2i(controls.x,controls.y);
		glVertex2i(controls.x,controls.mouse_start_y);
		glVertex2i(controls.mouse_start_x,controls.mouse_start_y);
		glEnd();
		
		glColor4f(1.0,1.0,1.0,0.1);
		glBegin(GL_POLYGON);
		glVertex2i(controls.mouse_start_x,controls.mouse_start_y);
		glVertex2i(controls.mouse_start_x,controls.y);
		glVertex2i(controls.x,controls.y);
		glVertex2i(controls.x,controls.mouse_start_y);
		glVertex2i(controls.mouse_start_x,controls.mouse_start_y);
		glEnd();
		
		controls.auswahlRechteck = true;
		
		awx_vector3f vec;
		vector <int> tempselection;
		for(int i=0;i<(int)player[controlIndex].units.size();i++)
		{
			if(!player[controlIndex].units[i].invisible && player[controlIndex].units[i].fortschritt >= 100){
				vec = player[controlIndex].units[i].bildschirmkoordinaten;
			
				float x = fabs((float)controls.x - controls.mouse_start_x);
				float y = fabs((float)controls.y - controls.mouse_start_y);
			
				float pos_x , pos_y;
			
				if(controls.mouse_start_x < controls.x)pos_x = controls.mouse_start_x;
				else pos_x = controls.x;
			
				if(controls.mouse_start_y > controls.y)pos_y = project.height - controls.mouse_start_y;
				else pos_y = project.height - controls.y;

				if(vec.x >= pos_x && vec.x < pos_x + x && vec.y >= pos_y && vec.y < pos_y + y)
				{
					tempselection.push_back(i);
			
				}
			}
		}
		
		if(tempselection.size() > 0){
			controls.selection.clear();
			controls.selection = tempselection;
		}
	
		glEnable(GL_TEXTURE_2D);
		
	} else controls.auswahlRechteck = false;
	

	glColor4f(1.0,1.0,1.0,1.0);
	for(int i=0;i<(int)ttfFonts.size();i++)
	{
		ttfFonts[i].renderDirect();
	}
	
	SDL_GL_Leave2DMode();
}

void render_interface()
{	
	if(player.size()>=controlIndex)
	for(int i=0;i<(int)player[controlIndex].units.size();i++)
	{
		if(player[controlIndex].units[i].unitValues.getrageneRohstoffe>0)
		{
			player[controlIndex].units[i].myWindowHeadPosition();
		}
	}


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D(0.0,project.aspect,0.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDepthFunc(GL_ALWAYS);
	glDisable(GL_LIGHTING);
	glColor4f(1.0,1.0,1.0,1.0);
	
	
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.5);


	float size=0.18;
	camera.real_winkel = camera.winkel_x + camera.calcRotAngle(controls.drag_dist_x);
	
	interface_gui.render();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,textur_liste[kartentextur]);
	
	if(!interface_gui.hidden){
        glPushMatrix();
                
        glTranslatef(0.09370424597*1.6,1 - 0.09370424597*1.6,0);
        
        glRotatef(camera.real_winkel , 0 , 0, 1);
        
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        
        glColor4f(1,0,0,0.8);
        glPointSize(6);
        glBegin(GL_POINTS);
            glVertex3f(-(camera.direction.x/currentLevel->terrain.xmax)*size/2.0 , (camera.direction.z/currentLevel->terrain.zmax)*size/2.0 , 1);
        glEnd();
        
        glColor4f(0.5,0.5,0.5,0.8);
        glPointSize(6);
        glBegin(GL_POINTS);
        for(int i = 0; i < (int)player[controlIndex].buildings.size();i++){
            glVertex3f(-(player[controlIndex].buildings[i].x/currentLevel->terrain.xmax)*size/2.0, ((player[controlIndex].buildings[i].z)/currentLevel->terrain.zmax)*size/2.0 , 1);
        }
        glEnd();
        
    
        
        glPopMatrix();
	}
	
		glDisable(GL_ALPHA_TEST);
	
	// Rohstofftexte render
	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);

	glLineWidth(1.0);
	glColor4f(0.0,0.0,0.0,1.0);
	
	sprintf(info_text,"%i",player[controlIndex].rohstoffvalues[0]);
	awx_stroke_output_simple(project.aspect - 0.4*project.aspect,0.965 ,0.00009,info_text);	
	
	sprintf(info_text,"%i",player[controlIndex].rohstoffvalues[1]);
	awx_stroke_output_simple(project.aspect - 0.3*project.aspect,0.965,0.00009,info_text);
	
	sprintf(info_text,"%i",player[controlIndex].rohstoffvalues[2]);
	awx_stroke_output_simple(project.aspect - 0.2*project.aspect,0.965,0.00009,info_text);
	
	sprintf(info_text,"%i",player[controlIndex].rohstoffvalues[3]);
	awx_stroke_output_simple(project.aspect - 0.1*project.aspect,0.965,0.00009,info_text);

	
	// Spezielle interfacerenderings: Eigentlich sollte in Einheitenbaurendereri geschachtelt werden!
	
	for(int i=0;i<(int)controls.selection.size();i++)
	{
		if(controls.selection[i]>=(int)player[controlIndex].units.size()&&controls.selection[i]<(int)player[controlIndex].units.size()+(int)player[controlIndex].buildings.size())
		{
			if(player[controlIndex].buildings[controls.selection[i]-player[controlIndex].units.size()].art==1&&player[controlIndex].buildings[controls.selection[i]-player[controlIndex].units.size()].unitQue.size()==0)
			{
				sprintf(info_text,"Stadt: Masupilami\n\nEinwohner: %i\ndavon Kriegerische Einheiten: %i\nBilanz: -3.14",player[controlIndex].units.size(),player[controlIndex].units.size());
				if(player[controlIndex].somethingChanged()) ttfFonts[player[controlIndex].hauptHausTextID].calculate(info_text);
				ttfFonts[player[controlIndex].hauptHausTextID].registerRender(0.29*project.width,project.height-project.height*0.145);
				
				// Gamericon rendern
				{
				float size=0.08;
				glEnable(GL_TEXTURE_2D);
				glPushMatrix();
				glTranslatef(0.85803,0.056,0);
				glColor4f(1.0,1.0,1.0,1.0);
				glBindTexture(GL_TEXTURE_2D,textur_liste[player[controlIndex].hauptHausBildID]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0,0.0);
				glVertex3f(0.0,0.0,0.0);
				glTexCoord2f(1.0,0.0);
				glVertex3f(size,0.0,0.0);
				glTexCoord2f(1.0,1.0);
				glVertex3f(size,size,0);
				glTexCoord2f(0.0,1.0);
				glVertex3f(0.0,size,0.0);
				glEnd();
				glPopMatrix();
				glDisable(GL_TEXTURE_2D);
				}
				
				break;
			}
		}
	}

	if(startCounter)
	{
		startCounter = false;
		
		player[controlIndex].startTimeMeldung = mainTime.getTime();
		interface_gui.windows[rohstoffwarnungMeldungID].hidden=false;
	}
	if(!interface_gui.windows[rohstoffwarnungMeldungID].hidden)
	{
		ttfFonts[rohstoffwarnungTexID].registerRender(425,310);
		
		long int a = mainTime.getTime()-player[controlIndex].startTimeMeldung;
		if(a>=2) interface_gui.windows[rohstoffwarnungMeldungID].hidden=true;
	}



	// do occlusion query with cursor
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	
		glDepthFunc(GL_EQUAL);

		glBeginQueryARB(GL_SAMPLES_PASSED_ARB,iAmInInterfaceOcclusionQueryObject);

		glColor4f(1.0,1.0,0.0,1.0);
		glPointSize(0.1);
		glBegin(GL_POINTS);
		glVertex3f(controls.x/(float)project.width*project.aspect,1.0f-controls.y/(float)project.height,0);
		glEnd();

		glEndQueryARB(GL_SAMPLES_PASSED_ARB);

		glPopAttrib();
	}
	
	
	renderTTFFonts();		
	

	glPopAttrib();
	

	/*#ifdef allowReadPixels
	#ifdef GCCDebugCompile
	#warning glFinish found
	#endif
	glReadPixels(controls.x,project.height - controls.y,1,1,GL_RGB,GL_UNSIGNED_BYTE,&colorOverInterface);
	#endif

	amInInterface();*/
}

int renderTop(awx_vector3f pos, awx_vector3f lookAt)
{
    int size=128;
	
	glUseProgramObjectARB(NULL);
	glDisable(GL_BLEND);
    
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glViewport(0, 0, size, size);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(currentLevel->terrain.xmin,currentLevel->terrain.xmax,currentLevel->terrain.zmin,currentLevel->terrain.zmax,10,400);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
	
	//currentLevel->renderFOWBottom();

	glPushAttrib(GL_LIGHTING);
	
	{
		if(project.perPixelLighting)
		{
			GLfloat no_mat[] = {0.05,0.05,0.05,1.0};
			GLfloat diffuse[] = {0.9,0.9,0.9,1.0};
			glMaterialfv(GL_FRONT,GL_AMBIENT,no_mat);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
		}
		else 
		{
			GLfloat no_mat[] = {0.00,0.00,0.00,1.0};
			GLfloat diffuse[] = {0.9,0.9,0.9,1.0};
			glMaterialfv(GL_FRONT,GL_AMBIENT,no_mat);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
		}
		GLfloat specular[] = {0.6,0.6,0.6,1.0};
		GLfloat shininess[] = {120.0};

		glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
		glMaterialfv(GL_FRONT,GL_SHININESS,shininess);

	}

    gluLookAt(pos.x,pos.y,pos.z,
              lookAt.x,lookAt.y,lookAt.z,
              0,0,1);
			  
		glPushMatrix();
	{
		float light_position[]={0.0,500,000.0,0.0};
		glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	}
	glPopMatrix();	
 
    currentLevel->terrain.render();
	
	{
		GLfloat ambient[] = {0.2,0.2,0.2,1.0};
		glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	
	}
	
	currentLevel->renderLevelObjectsOptimized(false);

    glBindTexture(GL_TEXTURE_2D,textur_liste[currentID]);
    glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,0,0,size,size,0);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	
	glEnable(GL_BLEND);
	
	glPopAttrib();
    
    return currentID;
}

class intPair
{
	public:
	
	int x,y;
	awx_vector3f position;
	int hitTriangle;
	
	bool operator==(intPair &p)
	{
		if(x==p.x&&y==p.y) return true;
		return false;
	}
};

void updateFogOfWarForAllUnits()
{
	//Uint32 start = SDL_GetTicks();

	vector<intPair> updateIndices;
	intPair temp;
	bool flag=false;
	
	for(int i=0;i<player[controlIndex].units.size();i++)
	{
		if(player[controlIndex].units[i].updateFogOfWarFlag)
		{
			player[controlIndex].units[i].updateFogOfWarFlag=false;
			currentLevel->terrain.getSektor(player[controlIndex].units[i].position,temp.x,temp.y);
			temp.position = player[controlIndex].units[i].position;
			temp.hitTriangle = player[controlIndex].units[i].hitTriangle;
			/*if(find(updateIndices.begin(),updateIndices.end(),temp)==updateIndices.end())
				updateIndices.push_back(temp);*/
			flag=false;
			for(int u=0;u<updateIndices.size();u++)
			{
				if(updateIndices[u]==temp) flag=true;
			}
			if(!flag)
			{
				updateIndices.push_back(temp);			
			}
		}
	}
	
	if(updateIndices.size()>0) 	currentLevel->terrain.startUpdateFogOfWar();
	
	for(int i=0;i<updateIndices.size();i++)
	{
		currentLevel->terrain.doFogOfWarForTriangleInSektor(updateIndices[i].hitTriangle,updateIndices[i].x,updateIndices[i].y,updateIndices[i].position,1,10,false);
	}
	
	if(updateIndices.size()>0) 	currentLevel->terrain.stopUpdateFogOfWar();
	
	updateIndices.clear();
	
	//cout << "update in " << (SDL_GetTicks()-start)/1000.0 << endl;
}
