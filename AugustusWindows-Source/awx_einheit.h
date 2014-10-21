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

class einheit
{
	public:
	
		int ID;
		awx_vector3f position;
		awx_vector3f target;
		awx_vector3f direction;
		int action;
		float matrix[16];
		
		float speed;
		float t;
		
		float center_front;
		int feet;
		
		float newY;
		bool newTest;
		
		awx_vector3f x1;
		awx_vector3f y1;
		awx_vector3f z1;
		
		void setPosition(float x , float y , float z){
			position.setX(x);
			position.setY(y);
			position.setZ(z);
			
			action = 0;
		}
		
		/*int hit(awx_vector3f point , float &hit , bool &test){
			point = point.substract(global_correction);
			int x = locatePointX(point.getX());
			int z = locatePointZ(point.getZ());
		
			int loop;
			
				for(loop=0; loop<sektoren[x][z].polyCount; loop+=9)
				{
					hit = awx_triangle_hitTest_2D(sektoren[x][z].triangles[loop], sektoren[x][z].triangles[loop+2],sektoren[x][z].triangles[loop+1],
								 sektoren[x][z].triangles[loop+3], sektoren[x][z].triangles[loop+5],sektoren[x][z].triangles[loop+4],
								 sektoren[x][z].triangles[loop+6], sektoren[x][z].triangles[loop+8],sektoren[x][z].triangles[loop+7],
								 point.getX(), point.getZ(), point.getY());
				
					if(hit!=AWXFalse)
					{
						if(awx_triangle_hitTest(sektoren[x][z].triangles[loop], sektoren[x][z].triangles[loop+2],sektoren[x][z].triangles[loop+1],
								 sektoren[x][z].triangles[loop+3], sektoren[x][z].triangles[loop+5],sektoren[x][z].triangles[loop+4],
								 sektoren[x][z].triangles[loop+6], sektoren[x][z].triangles[loop+8],sektoren[x][z].triangles[loop+7],
								 point.getX(), point.getZ(), point.getY()))
						{
							test = true;
						}
					
						else{
							test = false;
						}
						
						break;
					}
					else{
						test = false;
					}

				}
				//cout << sektoren[x][z].IDsequenz[int(floor(loop/3.0))] << endl;

				return loop;
		}*/
		
		void idle(){
			t = (1/float(runtime.fps));
			
			//Gehen:
			
			if(action == 1){
				
				direction = (target.substract(position));
				direction.setY(0);
				
				if(direction.getLength() > speed*t){
					position = position.addition(direction.normalize().product(speed*t));
				
				//Keine Gelaendeanpassung: z.B. für Menschen und andere Zweibeiner
					if(feet == 2){
						currentLevel->terrain.hit(position , newY , newTest);
						position.setY(newY);
					}
					
				//Zwei-Punkt Gelaendeanpassung: z.B. für Fahrzeuge mit 2 Raedern (Moped)
				
					else if(feet == 2){
						//Kommt noch
					}
					
				//Drei-Punkt Gelaendeanpassung: z.B. für Fahrzeuge mit vier Raedern oder Tiere mit vier Beinen
					
					else if(feet == 3){
						
						y1.setY(1);
						
						//Neues Z:
						
							z1 = (direction.normalize()).product(center_front);
						
							currentLevel->terrain.hit(position.addition(z1) , newY , newTest);
						
							z1.setY(newY);
						
						//Neues X:
						
							x1 = y1.crossProduct(z1);
						
							currentLevel->terrain.hit(position.addition(x1) , newY , newTest);
						
							x1.setY(newY);
						
						//Neues Y:
						
							y1 = x1.crossProduct(z1);
				
						//In Matrix schreiben:
							matrix[0] = x1.getX()/x1.getLength();
							matrix[1] = x1.getY()/x1.getLength();
							matrix[2] = x1.getZ()/x1.getLength();
	
							matrix[4] = y1.getX()/y1.getLength();
							matrix[5] = y1.getY()/y1.getLength();
							matrix[6] = y1.getZ()/y1.getLength();
	
							matrix[8] = z1.getX()/z1.getLength();
							matrix[9] = z1.getY()/z1.getLength();
							matrix[10] = z1.getZ()/z1.getLength();		
					}
					
				}
				else{
					currentLevel->terrain.hit(target , newY , newTest);
					target.setY(newY);
					position = target;
					action = 0;
				}
			}
		}
		
		void calcRoute(awx_vector3f ziel)
		{
			target = ziel;
			
			action = 1;
			
			if(feet == 2){
			
				direction = (target.substract(position));
				direction.setY(0);
			
				y1.setY(1);
					
				z1 = direction;
				x1 = y1.crossProduct(z1);
				
				matrix[0] = x1.getX()/x1.getLength();
				matrix[1] = x1.getY()/x1.getLength();
				matrix[2] = x1.getZ()/x1.getLength();
	
				matrix[4] = y1.getX()/y1.getLength();
				matrix[5] = y1.getY()/y1.getLength();
				matrix[6] = y1.getZ()/y1.getLength();
	
				matrix[8] = z1.getX()/z1.getLength();
				matrix[9] = z1.getY()/z1.getLength();
				matrix[10] = z1.getZ()/z1.getLength();
			}
		}
		
		einheit()
		{
			speed = 51.61333;
			action = 0;
			
			feet = 2;
			
			for(int i = 0; i <  16;i++){
				matrix[i] = 0;
 			}
			
			matrix[0] = 1;
			matrix[5] = 1;
			matrix[10] = 1;
			matrix[15] = 1;
		}
};