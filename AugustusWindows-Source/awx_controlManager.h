///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   � Alphaworx Corporation //  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////

// Funktion, die beim Dr체cken von ASCII f채higen Tasten ausgef체hrt wird
void awx_keyboard_down(SDLKey key, int x, int y)
{		
	if(!interface_gui.console.hidden){
		interface_gui.keyDown((int)key);
	}
	else if(controls.selection.size() == 0){
		if(key==controls.f_key) controls.forward_flag=true;  
		else if(key==controls.b_key) controls.backward_flag=true; 
		else if(key==controls.l_key) controls.left_flag=true; 
		else if(key==controls.r_key) controls.right_flag=true; 
		else if(key==SDLK_UP) controls.up_flag=true;
		else if(key==SDLK_DOWN) controls.down_flag=true;
	
		else if(key=='p') awx_ScreenShot();
		else if(key==SDLK_SPACE)
		{
			controls.navigate=true;
		}
		else if(key=='q') awx_shutdown(0);
		else if(key=='h') interface_gui.windows[0].hidden = !interface_gui.windows[0].hidden;
		else if(key=='n') interface_gui.windows[rohstoffwarnungMeldungID].hidden = !interface_gui.windows[rohstoffwarnungMeldungID].hidden;
		else if(key=='k') player[controlIndex].nextMode();
		else if(key=='z') 
		{
			for(int i=0;i<(int)(int)controls.selection.size();i++)
			{
				player[controlIndex].units[controls.selection[i]].data[0].gotoBezeichnung("stechen");
			}
		}
		else if(key=='n')
		{
			awx_resetScene();
		}
		else if(key=='o')
		{
			awx_shutdown(5);
			char **lol=NULL;
			awx_createGame(0,lol);
		}	
		else if(key =='y'){
			if(interface_gui.hidden){
				interface_gui.hidden = false;
			}
			else{
				interface_gui.hidden = true;
			}
		}
		else if(key == 13 && !interface_gui.hidden) interface_gui.console.hidden= false;
		else if(key == '1'){
			player[1].buildings[0].buildUnit("arbeiter",false);
			cout << "arbeiter bauen" << endl;
		}
		else if(key == '2'){
			player[1].buildings[0].buildUnit("soldat",false);
			cout << "soldat bauen" << endl;
		}
		
		if(key != 13){
			interface_gui.keyDown((int)key);
		}
	}
	else
	{
		if(controls.selection.size() > 0 && 
				controls.selection[0] >= (int)player[controlIndex].units.size() && 
				controls.selection[0] < (int)player[controlIndex].buildings.size() + (int)player[controlIndex].units.size() && iAmInInterface)
			{
				player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].keyDown((int)key);
			}
	}
}

// Funktion, die beim Loslassen von ASCII f채higen Tasten ausgef체hrt wird
void awx_keyboard_up(SDLKey key, int x, int y)
{
	if(key==controls.f_key) controls.forward_flag=false;  
	else if(key==controls.b_key) controls.backward_flag=false; 
	else if(key==controls.l_key) controls.left_flag=false; 
	else if(key==controls.r_key) controls.right_flag=false; 	
	else if(key==' ') controls.navigate=false;
	else if(key==SDLK_UP) controls.up_flag=false;
	else if(key==SDLK_DOWN) controls.down_flag=false;
}

// Funktion die immer bei gedr체ckter Maus die (idle) Mausposition bekommt
void awx_motion_mouse(int x, int y)
{
	currentLevel->checkVisibilityAll();
	
	if(controls.mouse_down==false)
	{
		controls.mouse_down=true;
		controls.mouse_start_x = x;
		controls.mouse_start_y = y;
	}
	
	if(controls.navigate)
	{
		controls.drag_dist_x = x-controls.mouse_start_x;
		controls.drag_dist_y = y-controls.mouse_start_y;
	} else
	{
		interface_gui.mouseOver((x/float(project.width))* project.aspect , (y/float(project.height)));
		controls.drag_dist_y=0;
		controls.drag_dist_x=0;
	}
	
	controls.x=x;
	controls.y=y;
}

bool amInInterface()
{
	if(colorOverTerrain[0]==colorOverInterface[0]&&
	   colorOverTerrain[1]==colorOverInterface[1]&&
	   colorOverTerrain[2]==colorOverInterface[2])
	{
		iAmInInterface = false;
		return false;
	}
	
	iAmInInterface = true;
	return true;
}

// Funktion die bei Mausreaktion den Button, den Status und die Position liefert
void awx_mouse(int button, int state, int x, int y)
{	
	intersectViewRayWithTerrain();

	if(button==SDL_BUTTON_LEFT&&state==SDL_PRESSED && controls.draggingFlag) 		// Fahne wird nichtmehr gedragt
	{
		controls.draggingFlag = false;
		return;
	}
	
	if(button==SDL_BUTTON_LEFT&&state==SDL_PRESSED)
	{
		if(y<project.height*0.75)
		calculateSelection();
		
		
		if(controls.currentLevelPartDragName!=NULL)
		{
			currentLevel->addLevelPartFromBuilding(*controls.currentLevelPartDragName);
			
			delete controls.currentLevelPartDragName;
			controls.currentLevelPartDragName = NULL;
		}
	}

	interface_gui.mouse(button,state,x,y);
	
	//if(controls.currentDragID==-1 && button == SDL_BUTTON_LEFT)calculateSelection(); // Nur wenn nichts gedragt wird selection berechnen
	
	vector <int> oldselection;
	for(int u = 0; u < (int)controls.selection.size();u++){
		oldselection.push_back(controls.selection[u]);
	}

	if(controls.preselection!=-1){
		if(controls.player_preselection == controlIndex){
			if((int)controls.selection.size() > 0 && controls.selection[0] >= 0 && controls.selection[0] < (int)player[controlIndex].units.size() &&
			   controls.preselection >= (int)player[controlIndex].units.size() && controls.preselection < (int)player[controlIndex].units.size() + (int)player[controlIndex].buildings.size() &&
			   player[controlIndex].buildings[controls.preselection - player[controlIndex].units.size()].fortschritt < 1)
			{
				// Wir wollten die Bedingung nicht "NICHTEN"
			}
			else if(button == SDL_BUTTON_LEFT && state == SDL_PRESSED){
				controls.selection.clear();
				controls.selection.push_back(controls.preselection);
			}
		}
		else{
			if((int)oldselection.size() > 0 && oldselection[0] >= 0 && oldselection[0] < (int)player[controlIndex].units.size()){
				for(int i = 0; i < (int)oldselection.size();i++){
					player[controlIndex].units[oldselection[i]].attack(controls.player_preselection , controls.preselection);
				}
			}
		}
	}
	

	if(button==SDL_BUTTON_LEFT&&state==SDL_RELEASED)
	{
		controls.mouse_down=false;
		camera.winkel_x+=camera.calcRotAngle(controls.drag_dist_x);
		camera.winkel_y+=camera.calcRotAngle(controls.drag_dist_y);
		controls.drag_dist_y=0;
		controls.drag_dist_x=0;
		
		if((int)controls.selection.size()>0&&controls.selection[0] < (int)player[controlIndex].units.size() && controls.selection[0] >= 0){
			player[controlIndex].units[controls.selection[0]].mouseUp(x,y);
		}
		
		if((int)controls.selection.size()>0&&controls.selection[0] >= (int)player[controlIndex].units.size() && controls.selection[0] < (int)player[controlIndex].units.size() + (int)player[controlIndex].buildings.size()){
			player[controlIndex].buildings[controls.selection[0] - player[controlIndex].units.size()].mouseUp(x,y);
		}
	}
	else if(button==SDL_BUTTON_RIGHT&&state==SDL_PRESSED&&!iAmInInterface)
	{
		// Fahne platzieren
		// Feststellen ob Haus markiert ist
		if((int)controls.selection.size()==1&&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].buildings.size()+(int)player[controlIndex].units.size())
		{
			player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].fahne = awx_vector3f(wx,wy,wz);
			return;
		}
		
		
		
//BEWEGEN:
		awx_vector3f dir;
				
		if(controls.selection.size()>0){
			for(int i = 0; i < (int)controls.selection.size();i++){
				dir = dir + player[controlIndex].units[controls.selection[i]].position;
				if(player[controlIndex].units[controls.selection[i]].workingOnBuildingID!=-1) player[controlIndex].units[controls.selection[i]].workingOnBuildingID = -1;
			}
			dir = dir/float(controls.selection.size());

			awx_vector3f ziel(wx,0,wz);
			dir = ziel - dir;
			dir = awx_vector3f(-dir.z , 0 , dir.x);
			dir = dir.normalize() * 2;
		
			int size_x = int((float)sqrt((float)controls.selection.size()));
			vector <awx_vector3f> targets;
			
			if(player[controlIndex].units[controls.selection[0]].formation == AWX_SQUARE){
				int xi = 0;
				int yi = 0;
			
			    for(int i = 0; i < (int)controls.selection.size();i++){
                    awx_vector3f temp = ziel + dir * (xi - size_x/2.0) + awx_vector3f(-dir.z , 0 , dir.x) * yi +
                                        awx_vector3f((RANDOM_FLOAT*2 - 1)*0.2 , 0 , (RANDOM_FLOAT*2 - 1)*0.2);
                    if(xi == size_x){
                        yi++;
                        xi = 0;
                    }
                    else xi++;
                    targets.push_back(temp);
                }
			}
			else if(player[controlIndex].units[controls.selection[0]].formation == AWX_TRIANGLE){
				int xi = 0;
				int yi = 0;
			
			    for(int i = 0; i < (int)controls.selection.size();i++){
                    awx_vector3f temp = ziel + dir * (xi - yi/2.0) + awx_vector3f(-dir.z , 0 , dir.x) * yi +
                                        awx_vector3f((RANDOM_FLOAT*2 - 1)*0.2 , 0 , (RANDOM_FLOAT*2 - 1)*0.2);
                    if(xi == yi){
                        yi++;
                        xi = 0;
                    }
                    else xi++;
                    targets.push_back(temp);
                }
			}
			else{
				int xi = 0;
				int yi = 0;
			
			    for(int i = 0; i < (int)controls.selection.size();i++){
                    awx_vector3f temp = ziel + dir * (xi - size_x/2.0) + awx_vector3f(-dir.z , 0 , dir.x) * yi +
                                        awx_vector3f((RANDOM_FLOAT*2 - 1)*1.0 , 0 , (RANDOM_FLOAT*2 - 1)*1.0);
                    if(xi == size_x){
                        yi++;
                        xi = 0;
                    }
                    else xi++;
                    targets.push_back(temp);
                }
			}
			
			
			vector <int> used;
			
			for(int i = 0; i < (int)targets.size();i++){
				float time = 100000;
				int index = 0;
				
				for(int u = 0; u < (int)controls.selection.size();u++){
					float dt = (targets[i] - player[controlIndex].units[controls.selection[u]].position).getLength() / player[controlIndex].units[controls.selection[u]].unitValues.geschwindigkeit;
					bool flag = true;
					for(int x = 0; x < (int)used.size();x++){
						if(u == used[x]){
							flag = false;
							break;
						}
					}
					
					if(flag){
						if(dt < time){
							time = dt;
							index = u;
						}
					}
				}
				used.push_back(index);
				
				player[controlIndex].units[controls.selection[index]].setTarget(targets[i]);
				
			}
		}

		// Haus weiterbauen
		bool flag = false;
		for(int i = 0; i < (int)oldselection.size();i++){
            if(oldselection[i]>=0&&oldselection[i]<(int)player[controlIndex].units.size()&&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].buildings.size()+(int)player[controlIndex].units.size()&&
			   player[controlIndex].units[oldselection[i]].isAbleToBuild(player[controlIndex].buildings[controls.selection[0] - (int)player[controlIndex].units.size()].workingdirectory.lastPathComponent()))
            {
                player[controlIndex].units[oldselection[i]].setTarget(awx_vector3f(wx,wy,wz));
				flag = true;
            }
		}
		if(flag){
			controls.selection.clear();
			for(int i = 0; i < (int)controls.selection.size();i++){
				controls.selection.push_back(oldselection[i]);
			}
		}
	
	//	controls.selection = -1;
	}
	else if(button==SDL_BUTTON_MIDDLE&&state==SDL_PRESSED)
	{
		controls.middle = true;
	}
	else if(button==SDL_BUTTON_MIDDLE&&state==SDL_RELEASED)
	{
		controls.middle = false;
	}
	else if(button==SDL_BUTTON_LEFT&&state==SDL_PRESSED) 
	{
		controls.mouse_start_x = x;
		controls.mouse_start_y = y;
		controls.mouse_down=true;
		
		
		if(!controls.navigate&&controls.currentDragID!=-1)
		{
			// player[currentDragPlayer] ist beim units vergleich nicht richtig da das dragbuilding vom anderen player sein kann
			if((int)controls.selection.size()>0&&controls.selection[0]>=0&&controls.selection[0]<(int)player[controlIndex].units.size())
			{
				player[controlIndex].units[controls.selection[0]].buildBuilding(awx_vector3f(wx,wy,wz));
			//}
			controls.preselection = controls.selection[0];
			controls.currentDragID = -1;
			}
		}
		
		if(controls.currentDragID!=-1) return;

		// Klickverarbeitung f웦 Cursors
		bool tempflag=false;

		// wenn man auf einheit geklict hat hausbauen und ziel setzen
		for(int i=0;i<(int)controls.selection.size();i++)
		{
			if(controls.selection[i]>=0&&controls.selection[i]<(int)player[controlIndex].units.size()&&tempflag&&!iAmInInterface)
			{
				player[controlIndex].units[controls.selection[i]].setTarget(awx_vector3f(wx,wy,wz));
			}
		}
		
		bool result;

		if((int)controls.selection.size()>0&&controls.selection[0]<(int)player[controlIndex].units.size()&&iAmInInterface)
		{
			result = player[controlIndex].units[controls.selection[0]].mouseDown(x,y);
		}
		
		if(controls.selection.size() > 0 && 
		   controls.selection[0] >= (int)player[controlIndex].units.size() && 
		   controls.selection[0] < (int)player[controlIndex].buildings.size() + (int)player[controlIndex].units.size() && iAmInInterface)
		{
			player[controlIndex].buildings[controls.selection[0]-player[controlIndex].units.size()].mouseDown(x,y);
		}
		
		if(controls.preselection==-1&&!tempflag&&!iAmInInterface&&!controls.navigate){
			controls.selection.clear();
		}
		
		
	}
	
	//bool uu = (button==SDL_BUTTON_RIGHT && !(controls.preselection >= player[controlIndex].units.size() && controls.preselection < player[controlIndex].units.size() + player[controlIndex].buildings.size()));
	
}


void awx_passive_mouse(int x, int y)
{	
	controls.x=x;
	controls.y=y;
	
	interface_gui.passive_mouse(x,y);


	if((int)controls.selection.size()>0&&controls.selection[0]>=0&&controls.selection[0]<(int)player[controlIndex].units.size())
	{
		player[controlIndex].units[controls.selection[0]].mouseOver(x,y);
	}
	
	if(controls.selection.size()>0&&controls.selection[0]>=(int)player[controlIndex].units.size()&&controls.selection[0]<(int)player[controlIndex].units.size() + (int)player[controlIndex].buildings.size())
	{
		player[controlIndex].buildings[controls.selection[0] - player[controlIndex].units.size()].mouseOver(x,y);
	}
}


void awx_mouse_idle()
{
	// Um Cameracontrollerei rocken zu lassen
	controls.matrix.setMatrix(camera.rotMatrix);
	controls.matrix = controls.matrix.transpose();


	float t = 1/float(runtime.fps);
	
	
	if(controls.middle)
	{
		if(controls.currentDragID!=-1)
		{
			player[controlIndex].buildings[player[controlIndex].buildings.size() -1].winkel_y += 180*t;
			//player[controls.currentDragPlayer].buildings[controls.currentDragID].winkel_y+=t*180;
		}
	}

	if(controls.x>project.width-10&&!controls.navigate) 
	{
		controls.move = controls.matrix.vectorProduct(controls.move_right.product(t));
		
		camera.position = camera.position.addition(controls.move);
		camera.direction = camera.direction.addition(controls.move);
		checkVisibilities();
	}
	else if(controls.x<10&&!controls.navigate)
	{
		controls.move = controls.matrix.vectorProduct(controls.move_left.product(t));
		
		camera.position = camera.position.addition(controls.move);
		camera.direction = camera.direction.addition(controls.move);
		checkVisibilities();
	}
	
	if(controls.y>project.height-10&&!controls.navigate) 
	{
		controls.move = controls.matrix.vectorProduct(controls.move_down.product(t));
		
		camera.position = camera.position.addition(controls.move);
		camera.direction = camera.direction.addition(controls.move);
		checkVisibilities();
	}
	else if(controls.y<10&&!controls.navigate)
	{
		controls.move = controls.matrix.vectorProduct(controls.move_up.product(t));
		
		camera.position = camera.position.addition(controls.move);
		camera.direction = camera.direction.addition(controls.move);
		checkVisibilities();
	}
}