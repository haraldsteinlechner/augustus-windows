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

typedef struct{
	float x;
	float y;
	
	float width;
	float height;
	
	bool hidden;
}auswahl;

char info_text[120];
awx_progress progress1;
awx_button unitButton;
awx_button buildingFlagButton;
awx_progress buildingsProgress;

awx_button defensivButton,aggressivButton,passivButton;
awx_button rechteckFormation, dreieckFormation, ohneFormation;
awx_button holzKopfInfo;

class awx_gui
{
	public:
		
		vector <awx_window> windows;
		awx_console console;
		auswahl selection;
		
		bool hidden;
		bool drag;
		
		awx_gui(){
			hidden = false;
		}
		
		void render()
		{
			glColor4f(1.0,1.0,1.0,1.0);
			
			// Headinfos Ÿber einheiten render
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			/*for(int i=0;i<(int)controls.selection.size();i++)
			{
				// Ist es eine Einheit
				if(controls.selection[i]>=0&&controls.selection[i]<player[controlIndex].units.size())
				{
					if(player[controlIndex].units[controls.selection[i]].carriedUnits>0)
					{
						glPushMatrix();
						glTranslatef(player[controlIndex].units[controls.selection[i]].myKopfPositionInWindowCoords.x/project.width * project.aspect,player[controlIndex].units[controls.selection[i]].myKopfPositionInWindowCoords.y/project.height,0);
						holzKopfInfo.render();
						glPopMatrix();
					}
				}
			}*/
			for(int i=0;i<(int)(int)player[controlIndex].units.size();i++)
			{
				if(player[controlIndex].units[i].unitValues.getrageneRohstoffe>0)
				{
					glPushMatrix();
					glTranslatef(player[controlIndex].units[i].myKopfPositionInWindowCoords.x/project.width * project.aspect,player[controlIndex].units[i].myKopfPositionInWindowCoords.y/project.height,0);
					holzKopfInfo.setTexture(rohstoffe[player[controlIndex].units[i].myRessID].texID);
					holzKopfInfo.render();
					glPopMatrix();
				}
			}

			
			if(!hidden){
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glEnable(GL_ALPHA_TEST);
			
				for(int i = 0; i < (int)windows.size();i++)
				{
					windows[i].render();
				}
				
				glDisable(GL_ALPHA_TEST);
				
				console.render();
				
				//sprintf(info_text,"FPS: %i %s",runtime.fps,mainTime.printToString().c_str());
				sprintf(info_text,"FPS: %i/%i  %i  err=%i c %i %d:%d:%d",runtime.fps,runtime.passiveFPS,rendered_objects,glGetError(),allowTerrainCulling , playedTime.stunde , playedTime.minute , playedTime.sekunde);
				//sprintf(info_text,"");
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_LIGHTING);
				//sprintf(info_text,"Press 'h' for Help");
				
				glPushMatrix();
				awx_stroke_output_simple(0.01,0.65,0.0002,info_text);
				glPopMatrix();
				glEnable(GL_TEXTURE_2D);
			}
			if(!selection.hidden&&1==1.333){
				glDisable(GL_TEXTURE_2D);
					glColor4f(1.0 , 1.0 , 1.0 , 0.8);
					glBegin(GL_POLYGON);
						glVertex3f(selection.x , selection.y , 0);
						glVertex3f(selection.x + selection.width , selection.y , 0);
						glVertex3f(selection.x + selection.width , selection.y + selection.height , 0);
						glVertex3f(selection.x , selection.y + selection.height , 0);
						glVertex3f(selection.x , selection.y , 0);
					glEnd();
				glEnable(GL_TEXTURE_2D);
			}
			
			glEnable(GL_TEXTURE_2D);
			if(controls.selection.size()==1&&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].buildings.size()+(int)player[controlIndex].units.size()&&!hidden)
			{
				player[controlIndex].buildings[controls.selection[0]-(int)player[controlIndex].units.size()].renderButtons();
			}
			else if(controls.selection.size()>0&&controls.selection[0]<(int)player[controlIndex].units.size()&&!hidden)
			{
				player[controlIndex].units[controls.selection[0]].renderButtons();
			}
			player[controlIndex].renderButtons();
			
			//Fortschrittsbuttons von HŠuser
/*
			if(controls.selection.size()==1&&controls.selection[0]>=units.size()&&controls.selection[0]<buildings.size()+units.size()&&buildings[controls.selection[0]-units.size()].fortschrittsbuttons.size()>0)
			{
					//float fortschritt = units[buildings[controls.selection[0]-units.size()].fortschrittsbuttons[0].unitID].visibleStatus;
					progress1.setProgress(fortschritt/100.0);
					progress1.render();
			
				if(buildings[controls.selection[0]-units.size()].fortschrittsbuttons.size() > 0){
                    unitButton.height=0.06;
                    unitButton.width=0.06;
                    unitButton.texID = units[buildings[controls.selection[0]-units.size()].fortschrittsbuttons[0].unitID].myButtonID;
                    unitButton.x=0.4;
                    unitButton.y=0.08;
                    unitButton.render();
                        
                        
                    unitButton.x=0.4;
                    unitButton.y=0.045;
					unitButton.width=0.03;
					unitButton.height=0.03;
				}
				
				for(int i=1;i<buildings[controls.selection[0]-units.size()].fortschrittsbuttons.size();i++)
				{
					
					unitButton.texID = units[buildings[controls.selection[0]-units.size()].fortschrittsbuttons[i].unitID].myButtonID;
					unitButton.x=0.4 + (i-1)*(unitButton.width + 0.01);
					unitButton.render();
					//cout << units[buildings[controls.selection-units.size()].fortschrittsbuttons[i].unitID].myButtonID << endl;
					//cout << "I=" << i << "  " << buildings[controls.selection-units.size()].fortschrittsbuttons[i].fortschritt << " /" << buildings[controls.selection-units.size()].fortschrittsbuttons[i].unitID << endl;
				}

			}*/
			
			
			// Draw process bars over uncompleted buildings 
			// GLdouble progressX,progressY,progressZ;
			/*for(int i=0;i<(int)buildings.size();i++)
			{
				if(buildings[i].bauStatus>=100)continue;
				buildingsProgress.x=((buildings[i].progressX/float(project.width))*project.aspect-buildingsProgress.width/2.0);
				buildingsProgress.y=buildings[i].progressY/float(project.height)-buildingsProgress.height/2.0;
				buildingsProgress.setProgress(buildings[i].bauStatus/100.0);
				buildingsProgress.render();
			}*/
			
			for(int i=0;i<(int)player[controlIndex].buildings.size();i++)
			{
				if(player[controlIndex].buildings[i].fortschritt>=1.0)continue;
				buildingsProgress.x=((player[controlIndex].buildings[i].progressX/float(project.width))*project.aspect-buildingsProgress.width/2.0);
				buildingsProgress.y=player[controlIndex].buildings[i].progressY/float(project.height)-buildingsProgress.height/2.0;
				buildingsProgress.setProgress(player[controlIndex].buildings[i].fortschritt);
				buildingsProgress.render();
			}
			
			
			if((int)controls.selection.size() > 0 &&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].buildings.size()+(int)player[controlIndex].units.size()&&player[controlIndex].buildings[controls.selection[0]-(int)player[controlIndex].units.size()].unitQue.size()>0&&!hidden)
			{
					float fortschritt = player[controlIndex].units[player[controlIndex].buildings[controls.selection[0] - player[controlIndex].units.size()].unitQue[0]].fortschritt;
					progress1.setProgress(fortschritt/100.0);
					progress1.render();
			
					if(player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].unitQue.size() > 0){
						unitButton.height=0.06;
						unitButton.width=0.06;
						unitButton.texID = player[controlIndex].units[player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].unitQue[0]].myButtonID;
						unitButton.x=0.4;
						unitButton.y=0.08;
						unitButton.render();
                        
						unitButton.x=0.4;
						unitButton.y=0.045;
						unitButton.width=0.03;
						unitButton.height=0.03;
					}
				
					for(int i=1;i<(int)player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].unitQue.size();i++)
					{
						unitButton.texID = player[controlIndex].units[player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].unitQue[i]].myButtonID;
						unitButton.x=0.4 + (i-1)*(unitButton.width + 0.01);
						unitButton.render();
					}

			}
			
			// Verhaltensbuttons rendern
			if((int)controls.selection.size()>0&&controls.selection[0]<(int)player[controlIndex].units.size()&&!hidden)
			{ 
				aggressivButton.render();
				defensivButton.render();
				passivButton.render();
				
				rechteckFormation.render();
				dreieckFormation.render();
				ohneFormation.render();
			}
			
			else if((int)controls.selection.size()==1&&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].buildings.size()+(int)player[controlIndex].units.size())
			{
				buildingFlagButton.render();
			}
		}
		
		void resize(float aspect){
			for(int i = 0; i < (int)windows.size();i++){
				if(windows[i].resize){
					windows[i].width = aspect;
					windows[i].aspect = aspect;
				}
			}
		}
		
		void mouseOver(float x , float y)
		{
			if(!hidden){
				for(int i = 0; i < (int)windows.size();i++)
				{
					windows[i].mouseOver(x , y);
				}
				if(drag){
					selection.width = x - selection.x;
					selection.height = -(y - selection.y);
				}
			}	
			
			if((int)controls.selection.size()>0&&controls.selection[0]<(int)player[controlIndex].units.size())
			{ 
				aggressivButton.mouseOver(x,y);
				defensivButton.mouseOver(x,y);
				passivButton.mouseOver(x,y);
				rechteckFormation.mouseOver(x,y);
				dreieckFormation.mouseOver(x,y);
				ohneFormation.mouseOver(x,y);
			} else if((int)controls.selection.size()==1&&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].buildings.size()+(int)player[controlIndex].units.size())
			{
				buildingFlagButton.mouseOver(x,y);
			}
		}
		
		void mouseDown(float x , float y , int button)
		{
			if(!hidden){
				for(int i = 0; i < (int)windows.size();i++)
				{
					if(x >= windows[i].x && y >= windows[i].y && x <= windows[i].x + windows[i].width && y <= windows[i].y + windows[i].height)
					{
						windows[i].mouseDown(x , y , button);
						break;
					}
				}
				if(x >= console.x && y >= console.y && x <= console.x + console.width && y <= console.y + console.height){
					console.mouseDown(x , y , button);
				}
				
				if(!drag){
					drag = true;
					selection.x = x;
					selection.y = y;
					selection.hidden = false;
				}
				
				if((int)controls.selection.size()>0&&controls.selection[0]<(int)player[controlIndex].units.size())
				{ 
					if(aggressivButton.mouseOverFlag) aggressivButton.mouseDown(x,y,button);
					if(defensivButton.mouseOverFlag) defensivButton.mouseDown(x,y,button);
					if(passivButton.mouseOverFlag) passivButton.mouseDown(x,y,button);
					
					if(rechteckFormation.mouseOverFlag) rechteckFormation.mouseDown(x,y,button);
					if(dreieckFormation.mouseOverFlag) dreieckFormation.mouseDown(x,y,button);
					if(ohneFormation.mouseOverFlag) ohneFormation.mouseDown(x,y,button);
				}
				else if((int)controls.selection.size()==1&&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].buildings.size()+(int)player[controlIndex].units.size())
				{
					if(buildingFlagButton.mouseOverFlag)  buildingFlagButton.mouseDown(x,y,button);
				}
			}
		}
		
		void mouseUp(float x , float y , int button)
		{
			if(!hidden){
				for(int i = 0; i < (int)windows.size();i++)
				{
					windows[i].mouseUp(x , y , button);
				}
				console.mouseUp(x , y , button);
				if(drag){
					drag = false;
					selection.width = 0;
					selection.height = 0;
					selection.hidden = true;
				}
				
				if(controls.selection.size()>0&&controls.selection[0]<(int)player[controlIndex].units.size())
				{ 
					aggressivButton.mouseUp(x,y,button);
					defensivButton.mouseUp(x,y,button);
					passivButton.mouseUp(x,y,button);
					
					rechteckFormation.mouseUp(x,y,button);
					dreieckFormation.mouseUp(x,y,button);
					ohneFormation.mouseUp(x,y,button);
				}
				else if((int)controls.selection.size()==1&&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].buildings.size()+(int)player[controlIndex].units.size())
				{
					buildingFlagButton.mouseUp(x,y,button);
				}
			}
		}
		
		void keyDown(int code){
			if(!hidden){
				if(code == 13 && console.hidden){
					console.hidden = false;
				}
				else if(!console.hidden){
					console.keyDown(code);
				}
				else if(code == 27){
					if(windows[2].hidden){
						windows[2].hidden = false;
					}
					else windows[2].hidden = true;
				}
			}
		}
		 
		void keyUp(int code){
			if(!hidden){
				console.keyUp(code);
			}
		}
		
		void passive_mouse(int x , int y){
			float relX = (x/float(project.width))*project.aspect;
			float relY = (project.height - y)/float(project.height);
			mouseOver(relX,relY);
		}	

		void mouse(int button , int state , int x , int y){
			float relX = (x/float(project.width))*project.aspect;
			float relY = (project.height - y)/float(project.height);
			if(state == SDL_PRESSED){
				mouseDown(relX,relY,button);
			}
			else{
				mouseUp(relX , relY , button);
			}
		}
};

awx_gui interface_gui;
awx_window wild , wild2 , wild3;
awx_button but;
awx_window win;

void awx_game_gui(int mode){
	float height = 0.25*project.aspect;

	progress1.x = 0.4 + 0.07;
	progress1.y = 0.11 - progress1.height;
	progress1.setTexture("progress.tga");
	progressTexID = progress1.texID;
	
	buildingsProgress.texID=progress1.texID;
	
	if(mode==0)
	{
		load_tga("Data/Interface/stadtIcon.tga",currentID,false,false);
		cout << "Fehler=" << controlIndex << endl;
		player[controlIndex].hauptHausBildID=currentID;
		currentID++;
	}

//Oberes Fenster:

	awx_window oben;
	
	oben.height = height;
	oben.width = project.aspect;
	
	oben.x = 0.0;
	oben.y = 1 - height;

	awx_map karte;
	karte.width = 0.18;
	karte.height = 0.18;
	karte.x = 0.09370424597*1.6;
	karte.y = oben.height - 0.09370424597*1.6;
	karte.hasPushedState = false;
	karte.hasRollOverState = false;
	karte.texID = kartentextur;
	oben.map = karte;

//Unteres Fenster:	
	
	awx_window unten;
	
	unten.height = height;
	unten.width = project.aspect;
	
	unten.x = 0.0;
	unten.y = 0.0;
	
	awx_button temp;
	
	temp.setTexture("button.tga");
	
	temp.x = 0.0175 * 1.33;
	
	temp.width = 0.0457;
	temp.height = 0.0457;
	
	temp.hasRollOverState = false;
	temp.hasPushedState = false;
	for(int i = 0; i < 5;i++){
		temp.y = 0.05;
		for(int u = 0;u < 3;u++){
			unten.buttons.push_back(temp);
			temp.y += 0.0053333333333 + temp.height;
		}
		temp.x += 0.004 * project.aspect + temp.width;
	}
	
//Texturen festlegen:

	if(project.aspect >= 1.6&&project.texturedetail!=1){
		oben.setTexture("interface_oben_breit_2048.tga");
		unten.setTexture("interface_unten_breit_2048.tga");
	}
	else if(project.texturedetail==1&&project.aspect >= 1.6)
	{
			oben.setTexture("interface_oben_breit_1024.tga");
		unten.setTexture("interface_unten_breit_1024.tga");
	}
	else{
		if(project.width <= 1024||project.texturedetail==1){
			oben.setTexture("interface_oben_1024.tga");
			unten.setTexture("interface_unten_1024.tga");
		}
		else{
			oben.setTexture("interface_oben_2048.tga");
			unten.setTexture("interface_unten_2048.tga");
		}
	}
	
//MenŸfenster:
	
	awx_window menu;
	
	menu.width = 0.3;
	menu.height = 0.4;
	
	menu.x = (project.aspect - menu.width)/2.0;
	menu.y = (1 - menu.height)/2.0 + 0.075;
	
	menu.hidden = true;
	
	menu.setSpecialTexture("window.tga");
	
	awx_button close;
	close.width = 0.22;
	close.height = 0.025;
	
	close.x = 0.04;
	close.y = 0.04;
	
	close.setMouseDownAction(closeButtonDown);
	
	close.setTexture("button.tga");
	
	menu.buttons.push_back(close);
	
	
// Warndialog

	awx_window warningDialog;
	
	warningDialog.width = 0.3;
	warningDialog.height = 0.1;
	
	warningDialog.x = (project.aspect - warningDialog.width)/2.0;
	warningDialog.y = (1 - warningDialog.height)/2.0 + 0.075;
	
	warningDialog.hidden = true;
	warningDialog.setSpecialTexture("window.tga");
	
	awx_window steuerFenster;
	
	steuerFenster.width = 0.4;
	steuerFenster.height = 0.4;
	
	steuerFenster.x = (project.aspect - steuerFenster.width)/2.0;
	steuerFenster.y = (1 - steuerFenster.height)/2.0 + 0.075;
	steuerFenster.hidden = true;
	steuerFenster.texID = warningDialog.texID;
	
	awx_slider slider;
	slider.x=0.08;
	slider.y=0.22;
	slider.width=0.23; //0.13
	steuerFenster.sliders.push_back(slider);
	
	awx_button rohstoff;
	
	rohstoff.hasPushedState = false;
	rohstoff.hasRollOverState = false;
	
	rohstoff.setTexture("holz.tga");
	int holzTexturID = rohstoff.texID;
	
	rohstoff.width = 0.025;
	rohstoff.height = 0.025;
	
	float picTans = rohstoff.width*0.79*project.aspect;
	
	rohstoff.x = project.aspect - 0.4*project.aspect - picTans;
	rohstoff.y = oben.height - rohstoff.height - 0.018;
	
	rohstoff.setMouseOverAction(mouseOverHolz);
	
	oben.buttons.push_back(rohstoff);
	
	rohstoff.x = project.aspect - 0.3*project.aspect - picTans;
	rohstoff.setTexture("gold.tga");
	rohstoff.setMouseOverAction(mouseOverStein);
	
	oben.buttons.push_back(rohstoff);
	
	rohstoff.x = project.aspect - 0.2*project.aspect - picTans;
	rohstoff.setTexture("steine.tga");
	rohstoff.setMouseOverAction(mouseOverGold);
	
	oben.buttons.push_back(rohstoff);
	
	rohstoff.x = project.aspect - 0.1*project.aspect - picTans;
	rohstoff.setTexture("nahrung.tga");
	rohstoff.setMouseOverAction(mouseOverNahrung);
	
	oben.buttons.push_back(rohstoff);
	
	awx_window steuerung;
	steuerung.width = 0.45;
	steuerung.height = 0.45;
	steuerung.x = project.aspect - steuerung.width;
	steuerung.y = 0.3; 
	steuerung.setTexture("controls.tga");
	steuerung.hidden=true;
	
	
	// Kopfinfo buttons initialisieren
	holzKopfInfo.setTexture(holzTexturID); 
	rohstoffe[0].texID = holzTexturID + 2;
	rohstoffe[1].texID = holzTexturID;
	rohstoffe[2].texID = holzTexturID + 1;  
	rohstoffe[3].texID = holzTexturID + 3; 
	holzKopfInfo.width = 0.025;
	holzKopfInfo.height = 0.025;
	holzKopfInfo.x = 0;
	holzKopfInfo.y = 0;
	holzKopfInfo.hidden = false;
	
	
//Fenster hinzufŸgen:

	interface_gui.windows.clear();
	
	interface_gui.windows.push_back(steuerung);
	interface_gui.windows.push_back(oben);
	interface_gui.windows.push_back(unten);
	interface_gui.windows.push_back(menu);
	interface_gui.windows.push_back(warningDialog);
	rohstoffwarnungMeldungID = (int)interface_gui.windows.size()-1;
	interface_gui.windows.push_back(steuerFenster);
	steuerFensterID = (int)interface_gui.windows.size()-1;
	
//Konsole:
	
	interface_gui.console.x = 0.0;
	interface_gui.console.y = 0.3;
	
	interface_gui.console.width = 0.6;
	interface_gui.console.height = 0.02;
	
	// Init font
	awx_trueTypeFont tempFont;
	tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,150,1,true);
	tempFont.init();
	tempFont.calculate("\n#b Nahrung");
	nahrungsTextID = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);
	
	tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,150,1,true);
	tempFont.init();
	tempFont.calculate("\n#b Steine");
	steinTextID = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);
	
	tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,150,1,true);
	tempFont.init();
	tempFont.calculate("\n#b Holz");
	holzTextID = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);

	tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,150,1,true);
	tempFont.init();
	tempFont.calculate("\n#b Gold");
	goldTextID = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);
	
	tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/imttf.ttf",15,0.7,150,1,true);
	tempFont.init();
	tempFont.calculate("Einheit ist sehr angriffslustig.");
	aggressivTextID = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);
	
	tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/im-tfotb.ttf",15,0.7,150,1,true);
	tempFont.init();
	tempFont.calculate("Einheit ist eher defensiv.");
	defensivTextID = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);
	
	tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,150,1,true);
	tempFont.init();
	tempFont.calculate("Einheit ist vollkommen passiv.");
	passivTextID = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);
	
	tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",15,0.7,1000,1,true);
	tempFont.init();
	tempFont.calculate("Es sind nicht genug\n Rohstoffe vorhanden.");
	rohstoffwarnungTexID = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);
	
	tempFont.setFormat(false,false,false,0,0,0,"Data/fonts/arial.ttf",12,0.7,1000,1,true);
	tempFont.init();
	passivTextID = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);
	if(mode==0)
	player[controlIndex].hauptHausTextID = (int)ttfFonts.size()-1;

	// Verhaltensbuttons
	
	defensivButton.setTexture("defensiv.tga");
	defensivButton.width = 0.0457;
	defensivButton.height = 0.0457;
	defensivButton.x = 1.06;
	defensivButton.y = 0.05;
	defensivButton.setMouseDownAction(changeAttitudeDefensive);
	defensivButton.setMouseOverAction(mouseOverDefensive);
	
	aggressivButton.setTexture("aggressiv.tga");
	aggressivButton.width = 0.0457;
	aggressivButton.height = 0.0457;
	aggressivButton.x = 1.11;
	aggressivButton.y = 0.05;
	aggressivButton.setMouseDownAction(changeAttitudeAggressive);
	aggressivButton.setMouseOverAction(mouseOverAggressive);
	
	passivButton.setTexture("passiv.tga");
	passivButton.width = 0.0457;
	passivButton.height = 0.0457;
	passivButton.x = 1.16;
	passivButton.y = 0.05;
	passivButton.setMouseDownAction(changeAttitudePassive);
	passivButton.setMouseOverAction(mouseOverPassive);
	
	
	rechteckFormation.setTexture("quadrat.tga");
	rechteckFormation.width = 0.0457;
	rechteckFormation.height = 0.0457;
	rechteckFormation.x = 1.06;
	rechteckFormation.y = 0.1;
	rechteckFormation.setMouseDownAction(changeFormationSquare);
	
	dreieckFormation.setTexture("dreieck.tga");
	dreieckFormation.width = 0.0457;
	dreieckFormation.height = 0.0457;
	dreieckFormation.x = 1.11;
	dreieckFormation.y = 0.1;
	dreieckFormation.setMouseDownAction(changeFormationTriangle);
	
	ohneFormation.setTexture("ohneFormation.tga");
	ohneFormation.width = 0.0457;
	ohneFormation.height = 0.0457;
	ohneFormation.x = 1.16;
	ohneFormation.y = 0.1;
	ohneFormation.setMouseDownAction(changeFormationNone);
	
	
	buildingFlagButton.setTexture("fahne.tga");
	buildingFlagButton.width = 0.0457;
	buildingFlagButton.height = 0.0457;
	
	if(project.aspect >= 1.5)buildingFlagButton.x = 1.323;
	else buildingFlagButton.x = 1.055;
	
	buildingFlagButton.y=0.05;
	
	buildingFlagButton.setMouseDownAction(useFlagPositioning);
}

void awx_menu(){
	interface_gui.windows.clear();
}

void initGUI(int mode)
{	
	infoWindow.setSpecialTexture("window.tga");
	awx_game_gui(mode);
	
	//awx_menu();
}